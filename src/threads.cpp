#include "threads.h"

/*
   The Master Distribution Thread Pool.
   Allocates three completely independent 4KB context containers inside our data segment.
   Strictly aligned to 16 bytes to satisfy modern compiler optimization stack alignment constraints.
*/
static ThreadControlBlock thread_pool[MAX_DISTRO_THREADS] __attribute__((aligned(16)));
static uint32_t current_running_thread_id = 0;

/*
   Initializes the multi-thread pool state parameters at boot time.
*/
extern "C" void init_distro_threads() {
    for (int i = 0; i < MAX_DISTRO_THREADS; i++) {
        thread_pool[i].thread_id = (uint32_t)i;
        thread_pool[i].esp_stack_pointer = 0;
        thread_pool[i].active_state = 0;

        // Zero out the private 4KB workspace array to clean out stale data frames
        for (int j = 0; j < THREAD_STACK_SIZE; j++) {
            thread_pool[i].stack_buffer[j] = 0;
        }
    }
    current_running_thread_id = 0;
}

/*
   Fabricates an independent, isolated CPU execution stack frame inside private 4KB RAM.
   Injects fake starting register flags to safely trick the CPU assembly pop instructions.
*/
extern "C" void create_isolated_thread(uint32_t id, void (*entry_point)()) {
    // Safety boundary constraints checking
    if (id >= MAX_DISTRO_THREADS || !entry_point) {
        return;
    }

    /*
       Locate the top edge boundary address of our private 4KB stack buffer array.
       In x86 architecture, the stack grows downwards in memory from high addresses to low.
    */
    uint32_t* stack_top = (uint32_t*)&thread_pool[id].stack_buffer[THREAD_STACK_SIZE];

    // 1. Inject the EFLAGS register state code (0x0202 enables hardware interrupts natively)
    stack_top--;
    *stack_top = 0x0202;

    // 2. Inject the Code Segment Selector code (0x08 = Kernel Code descriptor selector)
    stack_top--;
    *stack_top = 0x08;

    // 3. Inject the execution entry point address of the destination engine routine
    stack_top--;
    *stack_top = (uint32_t)entry_point;

    /*
       4. Inject 8 generic placeholder values to represent the pushed general registers layout matrix.
          Mimics the exact structural data dump executed by an x86 pusha/pushad loop instruction.
          EAX, ECX, EDX, EBX, Temporary ESP, EBP, ESI, EDI
    */
    for (int r = 0; r < 8; r++) {
        stack_top--;
        *stack_top = 0x00000000;
    }

    // 5. Securely register the final calculated stack pointer address to our tracking blocks
    thread_pool[id].esp_stack_pointer = (uint32_t)stack_top;
    thread_pool[id].active_state = 1; // Mark the isolated thread track as fully live and bootable
}
