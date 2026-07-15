#include "threads.h"
#include "raycast.h"
#include "particle.h"
#include "tensor.h"
#include "tokenizer.h"

/*
   The Master Distribution Thread Pool.
   Allocates three completely independent 4KB context containers inside our data segment.
   Strictly aligned to 16 bytes to satisfy modern compiler optimization stack alignment constraints.
*/
static ThreadControlBlock thread_pool[MAX_DISTRO_THREADS] __attribute__((aligned(16)));
static uint32_t current_running_thread_id = 0;

// Tracking state variables for player view matrices inside our distro environment
static Vec3 local_player_position = {4.5f, 4.5f, 0.0f};
static float local_player_angle = 0.0f;

/*
   Thread 1: The Monolithic Display Server Execution Loop.
   Computes 3D voxel transformations and updates visual tracers at maximum frame rates.
*/
void display_server_thread_loop() {
    while (true) {
        // 1. Flood our staging back-buffer with our signature slate background
        clear_back_buffer(0x001A1E24);

        // 2. Render your engine's custom 3D voxel projections onto the canvas
        render_voxel_world(local_player_position, local_player_angle);

        // 3. Compute vector movements and physics updates for all active particles
        update_and_render_particles();

        // 4. Flush the complete staging canvas straight out to the physical VGA display panel
        swap_graphics_buffers();

        // Cooperatively yield execution back to the scheduler to let Thread 2 compute predictions
        switch_task();
    }
}

/*
   Thread 2: The Background Cognitive Network Layer Execution Loop.
   Evaluates your model weights and runs tensor forward-propagation continuously.
*/
void cognitive_engine_thread_loop() {
    float prediction_probabilities[FONTANA_VOCAB_SIZE] = {0.0f};

    while (true) {
        // 1. Fetch a constant pointer reference tracking your active tokenizer history window
        const uint32_t* active_context = get_active_context_reference();

        // 2. Execute forward propagation matrix math loops directly over the data streams
        evaluate_tensor_forward(active_context, prediction_probabilities);

        // Cooperatively yield execution back to the scheduler to maintain visual frame integrity
        switch_task();
    }
}

/*
   Initializes the multi-thread pool state parameters at boot time.
*/
extern "C" void init_distro_threads() {
    for (int i = 0; i < MAX_DISTRO_THREADS; i++) {
        thread_pool[i].thread_id = (uint32_t)i;
        thread_pool[i].esp_stack_pointer = 0;
        thread_pool[i].active_state = 0;

        for (int j = 0; j < THREAD_STACK_SIZE; j++) {
            thread_pool[i].stack_buffer[j] = 0;
        }
    }
    current_running_thread_id = 0;
}

/*
   Fabricates an independent, isolated CPU execution stack frame inside private 4KB RAM.
*/
extern "C" void create_isolated_thread(uint32_t id, void (*entry_point)()) {
    if (id >= MAX_DISTRO_THREADS || !entry_point) {
        return;
    }

    uint32_t* stack_top = (uint32_t*)&thread_pool[id].stack_buffer[THREAD_STACK_SIZE];

    stack_top--;
    *stack_top = 0x0202; // EFLAGS: Interrupts Enabled

    stack_top--;
    *stack_top = 0x08;   // CS: Kernel Code Segment

    stack_top--;
    *stack_top = (uint32_t)entry_point;

    for (int r = 0; r < 8; r++) {
        stack_top--;
        *stack_top = 0x00000000; // Fake pushed registers matrix layout (pusha)
    }

    thread_pool[id].esp_stack_pointer = (uint32_t)stack_top;
    thread_pool[id].active_state = 1;
}

/*
   The Master Cooperative Scheduler State Machine.
   Cycles execution focus between Thread 1 (Graphics) and Thread 2 (Cognition) continuously.
*/
extern "C" void switch_task() {
    // Round-Robin tracking state pointer calculator loops
    uint32_t next_thread_id = (current_running_thread_id + 1) % MAX_DISTRO_THREADS;

    // Skip Thread 0 (Supervisor boot thread) once multi-tracking loops ignite
    if (next_thread_id == 0) {
        next_thread_id = 1;
    }

    // Guard to ensure the destination thread track is fully active and initialized
    if (thread_pool[next_thread_id].active_state == 1) {
        current_running_thread_id = next_thread_id;

        /*
           Note: The underlying assembly routine inside FontaineOS's boot.s
           intercepts the updated current_running_thread_id index, updates
           the hardware ESP register pointer, and pops the target register stack frame!
        */
    }
}
