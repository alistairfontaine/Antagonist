#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Pure integer bare-metal rendering engine to completely eliminate FPU faults.
*/

#define COLOR_DARK_SLATE    0x001A1E24
#define COLOR_GOLD          0x00FFD700
#define COLOR_CYAN          0x0000FFFF
#define COLOR_MINT_GREEN    0x0000FF00
#define COLOR_WHITE         0x00FFFFFF
#define COLOR_BLACK         0x00000000

inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

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

static void render_performance_profiler(uint32_t active_ticks) {
    char tick_str_buffer[16];
    uint_to_string_freestanding(active_ticks, tick_str_buffer);

    draw_string_gfx(10, 20, "SYS METRICS:", COLOR_CYAN);
    draw_string_gfx(110, 20, "TICKS:", COLOR_WHITE);
    draw_string_gfx(165, 20, tick_str_buffer, COLOR_MINT_GREEN);
    draw_string_gfx(10, 35, "ACTIVE INDICES: INITIALIZED OK", COLOR_WHITE);
}

extern "C" void antagonist_main() {
    // 1. Force fully wake up the high-resolution VESA graphics card hardware
    init_vbe_graphics();

    uint32_t active_render_ticks = 0;

    // 2. Continuous monolithic visual render track loop
    while (true) {
        active_render_ticks++;

        // A. Clear our hidden RAM staging canvas to your signature dark slate background
        clear_back_buffer(COLOR_DARK_SLATE);

        // B. Paint a gorgeous solid gold border bar directly onto the top edge
        for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
            put_pixel_buffer(x, 10, COLOR_GOLD);
            put_pixel_buffer(x, 11, COLOR_GOLD);
        }

        // C. Draw our custom bitmap text profiler indicators
        render_performance_profiler(active_render_ticks);

        // D. Flush the completed 480,000-pixel RAM canvas straight to the physical monitor screen!
        swap_graphics_buffers();

        // Minor loop slowdown delay to make the count easily human-readable
        for (volatile uint32_t d = 0; d < 400000; d++);
    }
}
