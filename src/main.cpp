#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"
#include "threads.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Orchestrates core hardware clocks, multi-thread allocation states,
   telemetry profiler overlays, and drives the system shutdown gateways.
*/

// Master 32-Bit Hexadecimal True Color Constant Index
#define COLOR_DARK_SLATE    0x001A1E24  // Deep background slate tone (#1A1E24)
#define COLOR_GOLD          0x00FFD700  // Brilliant gold accent frame (#FFD700)
#define COLOR_CYAN          0x0000FFFF  // High-intensity electric cyan (#00FFFF)
#define COLOR_MINT_GREEN    0x0000FF00  // Vibrant tracking green (#00FF00)
#define COLOR_WHITE         0x00FFFFFF  // Crisp diagnostic readouts (#FFFFFF)
#define COLOR_BLACK         0x00000000  // Zero-mask space (#000000)

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
   Fixed: Enforced strict 16-byte buffer sizing rules to completely protect
   multi-digit telemetry strings from triggering stack frame memory overflows.
*/
static void uint_to_string_freestanding(uint32_t value, char* dest_buffer) {
    int i = 0;
    if (value == 0) {
        dest_buffer[i++] = '0';
        dest_buffer[i] = '\0';
        return;
    }

    char staging[16]; // Explicitly sized matrix array boundary protection
    int s_idx = 0;
    while (value > 0 && s_idx < 15) {
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
    char tick_str_buffer[16]; // Secure localized tracking matrix array container
    uint_to_string_freestanding(active_ticks, tick_str_buffer);

    draw_string_gfx(10, 20, "SYS METRICS:", COLOR_CYAN);
    draw_string_gfx(110, 20, "TICKS:", COLOR_WHITE);
    draw_string_gfx(165, 20, tick_str_buffer, COLOR_MINT_GREEN);
    draw_string_gfx(10, 35, "ACTIVE THREADS: 3", COLOR_WHITE);
}

/*
   The System Halt Goodbye Gateway (Phase Z).
*/
static void execute_system_shutdown_gateway() {
    clear_back_buffer(COLOR_BLACK);

    draw_string_gfx(280, 280, "ANTAGONIST OS DISTRIBUTION POWERED DOWN safely", COLOR_GOLD);
    draw_string_gfx(330, 300, "SYSTEM CORE TERMINATED HALT", COLOR_WHITE);

    swap_graphics_buffers();

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
*/
extern "C" void antagonist_main() {
    init_pit_overclock();
    init_vbe_graphics();
    init_distro_threads();

    create_isolated_thread(1, display_server_thread_loop);
    create_isolated_thread(2, cognitive_engine_thread_loop);

    uint32_t local_supervisor_ticks = 0;

    while (true) {
        local_supervisor_ticks++;

        // Render our real-time telemetry based on the clean local loop tracker
        render_performance_profiler(local_supervisor_ticks);

        // Safe simulation check: trigger shutdown screen smoothly after 15,000 counts
        if (local_supervisor_ticks >= 15000) {
            execute_system_shutdown_gateway();
        }

        switch_task();
        asm volatile("hlt");
    }
}
