#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>

/*
   Total Concurrent System Thread Limits.
   Thread 0 = Distribution Supervisor (Inputs & Scrolling)
   Thread 1 = Display Server Renderer (Montaigne Voxel Engine)
   Thread 2 = Cognitive Network Layer (Fontana Tensor Engine)
*/
#define MAX_DISTRO_THREADS      3

/*
   Isolated Thread Stack Size Boundary.
   Expands standard stack allocations to exactly 4096 bytes (4KB) per task,
   providing a massive safety buffer to absorb asynchronous hardware IRQ frames safely.
*/
#define THREAD_STACK_SIZE       4096

/* Structure mapping the context state of a private machine task execution thread */
struct ThreadControlBlock {
    uint32_t thread_id;
    uint32_t esp_stack_pointer;  // Stores the saved CPU register stack state during switches
    uint8_t stack_buffer[THREAD_STACK_SIZE] __attribute__((aligned(16))); // Private 4KB workspace matrix
    uint8_t active_state;
};

/* Expose our expanded multi-threading context routines cleanly to C linkage maps */
extern "C" {
    void init_distro_threads();
    void create_isolated_thread(uint32_t id, void (*entry_point)());
    void switch_task();
}

#endif
