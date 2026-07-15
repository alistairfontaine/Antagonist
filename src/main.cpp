#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"
#include "raycast.h"
#include "particle.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Orchestrates core hardware clocks, renders 3D software voxel spaces,
   and draws real-time performance profiler graphics on a unified master thread.
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
    char tick_str_buffer[16];
    uint_to_string_freestanding(active_ticks, tick_str_buffer);

    draw_string_gfx(10, 20, "SYS METRICS:", COLOR_CYAN);
    draw_string_gfx(110, 20, "TICKS:", COLOR_WHITE);
    draw_string_gfx(165, 20, tick_str_buffer, COLOR_MINT_GREEN);
    draw_string_gfx(10, 35, "ACTIVE INDICES: MONOLITHIC", COLOR_WHITE);
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

/*
   Master Distro Boot Loop.
   Executed immediately after the micro-kernel hooks link over control.
*/
extern "C" void antagonist_main() {
    // 1. Initialize the motherboard's underlying timing oscillator registers
    init_pit_overclock();

    // 2. Wake up the high-resolution VESA VBE 800x600 32-bit Linear Framebuffer
    init_vbe_graphics();

    // 3. Setup our fixed-boundary visual tracer ring-buffer pool arrays
    init_particle_system();

    uint32_t active_render_ticks = 0;

    // Tracking static vectors to orient our viewport look positions inside the map grid
    Vec3 virtual_player_position = {4.5f, 4.5f, 0.0f};
    float virtual_player_angle = 0.0f;

    // 4. Enter our dedicated, high-speed monolithic visual processing matrix loop
    while (true) {
        active_render_ticks++;

        // A. Flood our staging back-buffer with our signature corporate dark slate tone
        clear_back_buffer(COLOR_DARK_SLATE);

        // B. Cast look-vector rays across the 800-pixel canvas to compute our 3D voxel walls
        render_voxel_world(virtual_player_position, virtual_player_angle);

        // C. Process positions and render all active textured particle tracers
        update_and_render_particles();

        // D. Rasterize our real-time performance profiler overlay on top of the graphics
        render_performance_profiler(active_render_ticks);

        // E. Verification hotkey hook checkpoint: trigger our gold gateway halt at 5000 cycles
        if (active_render_ticks >= 5000) {
            execute_system_shutdown_gateway();
        }

        // F. Forcefully stream the completed 480,000-word RAM canvas out to the video card!
        swap_graphics_buffers();
    }
}
