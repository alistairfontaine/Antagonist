#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "palette.h"
#include "font.h"
#include "threads.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Orchestrates core hardware clocks, multi-thread allocation states,
   telemetry profiler overlays, and drives the system shutdown gateways.
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
*/
static void uint_to_string_freestanding(uint32_t value, char* dest_buffer) {
    int i = 0;
    if (value == 0) {
        dest_buffer[i++] = '0';
        dest_buffer[i] = '\0';
        return;
    }

    char staging[16];
    int s_idx = 0;
    while (value > 0) {
        staging[s_idx++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = s_idx - 1; j >= 0; j--) {
        dest_buffer[i++] = staging[j];
    }
    dest_buffer[i] = '\0';
}

/*
   The Real-Time System Performance Profiler Interface Overlay (Phase X).
*/
static void render_performance_profiler(uint32_t active_ticks) {
    char tick_str_buffer[16];
    uint_to_string_freestanding(active_ticks, tick_str_buffer);

    draw_string_gfx(10, 20, "SYS METRICS:", COLOR_CYAN);
    draw_string_gfx(110, 20, "TICKS:", COLOR_WHITE);
    draw_string_gfx(165, 20, tick_str_buffer, COLOR_MINT_GREEN);
    draw_string_gfx(10, 35, "ACTIVE THREADS: 3", COLOR_WHITE);
}

/*
   The System Halt Goodbye Gateway (Phase Z).
   Wipes display canvases to neutral zero blocks, prints our corporate farewell signature,
   forcefully masks all asynchronous device interrupts, and locks down the silicon cores.
*/
static void execute_system_shutdown_gateway() {
    // 1. Flood our staging back-buffer with pure zero-mask black space
    clear_back_buffer(COLOR_BLACK);

    // 2. Render our high-contrast distribution final goodbye signature tracking strings
    draw_string_gfx(280, 280, "ANTAGONIST OS DISTRIBUTION POWERED DOWN safely", COLOR_GOLD);
    draw_string_gfx(330, 300, "SYSTEM CORE TERMINATED HALT", COLOR_WHITE);

    // 3. Flush the final shutdown graphics frame straight to the physical display panel
    swap_graphics_buffers();

    /*
       4. Forcefully apply hardware atomic execution shutdown state loops!
          cli disables maskable device interrupts; hlt puts physical silicon to sleep.
          Runs inside an infinite assembly loop wrapper to prevent CPU register drift.
    */
    while (true) {
        asm volatile("cli; hlt");
    }
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

        // Simulation hook checkpoint: Trigger our hardware system halt gateway once supervisor ticks touch 5000ms
        if (local_supervisor_ticks >= 5000) {
            execute_system_shutdown_gateway();
        }

        // Yield execution focus cooperatively to let Thread 1 and Thread 2 advance concurrently
        switch_task();
        asm volatile("hlt");
    }
}
