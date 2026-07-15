#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "palette.h"
#include "font.h"
#include "threads.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Orchestrates core hardware clocks, multi-thread allocation states,
   and handles real-time visual system telemetry profiler overlays.
*/

// Low-level port input assembly wrapper
inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/*
   The Host Interrupt Clock Overclock Pass (Phase F).
   Accelerates the Programmable Interval Timer (PIT) frequency to exactly 1000Hz.
*/
static void init_pit_overclock() {
    uint16_t divisor = 1193;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

/*
   Freestanding Integer-To-ASCII Base 10 Converter.
   Translates raw numeric telemetry integers into null-terminated string blocks
   entirely within localized stack matrices without standard library string links.
*/
static void uint_to_string_freestanding(uint32_t value, char* dest_buffer) {
    int i = 0;
    if (value == 0) {
        dest_buffer[i++] = '0';
        dest_buffer[i] = '\0';
        return;
    }

    // Extract base-10 digits backwards into a localized staging buffer array
    char staging[12];
    int s_idx = 0;
    while (value > 0) {
        staging[s_idx++] = '0' + (value % 10);
        value /= 10;
    }

    // Reverse the character layout bytes directly into the target destination pointer
    for (int j = s_idx - 1; j >= 0; j--) {
        dest_buffer[i++] = staging[j];
    }
    dest_buffer[i] = '\0';
}

/*
   The Real-Time System Performance Profiler Interface Overlay (Phase X).
   Queries live kernel runtime clocks and draws crisp telemetry status labels
   directly onto the upper corner layout of the double-buffered graphics screens.
*/
static void render_performance_profiler(uint32_t active_ticks) {
    char tick_str_buffer[12];
    uint_to_string_freestanding(active_ticks, tick_str_buffer);

    // 1. Draw static background status bounding boxes using our crisp Cyan color mask
    draw_string_gfx(10, 20, "SYS METRICS:", COLOR_CYAN);

    // 2. Draw our current running system clock execution ticks value right next to it
    draw_string_gfx(110, 20, "TICKS:", COLOR_WHITE);
    draw_string_gfx(165, 20, tick_str_buffer, COLOR_MINT_GREEN);

    // 3. Render active scheduler thread count flags natively
    draw_string_gfx(10, 35, "ACTIVE THREADS: 3", COLOR_WHITE);
}

// External hardware execution thread entry point anchors
extern "C" {
    void display_server_thread_loop();
    void cognitive_engine_thread_loop();
}

/*
   Master Distro Boot Loop.
   Executed immediately after the micro-kernel initializes the GDT, IDT, and paging segments.
*/
extern "C" void antagonist_main() {
    // 1. Forcefully accelerate system clock interrupt ticks to 1000Hz (1ms intervals)
    init_pit_overclock();

    // 2. Wake up the high-resolution VESA VBE 800x600 32-bit Linear Framebuffer
    init_vbe_graphics();

    // 3. Initialize our multi-thread context configuration tables
    init_distro_threads();

    // 4. Register our parallel engine tasks to their isolated 4KB execution stacks
    create_isolated_thread(1, display_server_thread_loop);
    create_isolated_thread(2, cognitive_engine_thread_loop);

    uint32_t local_supervisor_ticks = 0;

    // 5. Enter the master supervisor execution loop thread track (Thread 0)
    while (true) {
        local_supervisor_ticks++;

        // Render our real-time performance profiler overlay information onto the canvas layers
        render_performance_profiler(local_supervisor_ticks);

        // Yield execution focus cooperatively to let Thread 1 and Thread 2 advance concurrently
        switch_task();
        asm volatile("hlt");
    }
}
