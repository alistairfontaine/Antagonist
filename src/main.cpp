#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Float-free bare-metal rendering engine to completely prevent FPU triple faults.
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
    draw_string_gfx(10, 35, "ACTIVE INDICES: MONOLITHIC RUN", COLOR_WHITE);
}

extern "C" void antagonist_main() {
    // 1. Fire up the high-resolution VESA VBE graphics hardware
    init_vbe_graphics();

    uint32_t active_render_ticks = 0;

    while (true) {
        active_render_ticks++;

        // 2. Clear staging canvas to your signature dark slate background
        clear_back_buffer(COLOR_DARK_SLATE);

        // 3. Render a stunning, float-free 32-bit geometric test grid array
        for (uint32_t y = 100; y < 500; y += 40) {
            for (uint32_t x = 100; x < 700; x++) {
                // Generates a beautiful procedural color blend using zero float math!
                uint32_t procedural_color = ((x ^ y) & 1) ? COLOR_GOLD : 0x002A2E34;
                put_pixel_buffer(x, y, procedural_color);
                put_pixel_buffer(x, y + 1, procedural_color);
            }
        }

        // 4. Draw your custom bitmap font performance overlays
        render_performance_profiler(active_render_ticks);

        // 5. Blast the completed memory blocks right onto the monitor screen!
        swap_graphics_buffers();

        // Loop delay check to keep the counter human-readable
        for (volatile uint32_t d = 0; d < 500000; d++);
    }
}
