#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"
#include "procedural.h"
#include "hotbar.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Direct text-mode video dashboard with procedural scrolling landscapes,
   interactive item hotbar arrays, and dynamic day/night vector color loops.
*/

// Standard VGA text mode color attribute flags
#define ATTR_LIGHT_CYAN      0x0B
#define ATTR_GOLD            0x0E
#define ATTR_WHITE           0x0F
#define ATTR_MINT_GREEN      0x0A
#define ATTR_SLATE_GRAY      0x08
#define ATTR_LIGHT_PURPLE    0x0D

// Low-level port input assembly wrapper to query keyboard data lines directly
inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/*
   Lightweight freestanding string printer.
   Pokes characters and styles directly into the 80x25 text-video memory grid.
*/
static void print_string_raw(int row, int col, const char* str, uint8_t attribute) {
    volatile char* video_memory = (volatile char*)0xB8000;
    int index = (row * 80 + col) * 2;

    int i = 0;
    while (str[i] != '\0') {
        video_memory[index] = str[i];
        video_memory[index + 1] = attribute;
        index += 2;
        i++;
    }
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

extern "C" void antagonist_main() {
    uint32_t live_distro_ticks = 0;
    char tick_string[16];
    char terrain_matrix[400];

    // 1. Initialize our local distribution state structures and inventory fields
    init_inventory_hotbar();

    while (true) {
        live_distro_ticks++;
        uint_to_string_freestanding(live_distro_ticks, tick_string);

        // 2. Direct Hardware Port Polling Filter Mask to prevent scancode bouncing
        uint8_t live_scancode = inb(0x60);
        if (!(live_scancode & 0x80)) { // Only update if it's a fresh press (no break code)
            if (live_scancode >= 0x02 && live_scancode <= 0x05) {
                update_hotbar_selection(live_scancode);
            }
        }

        /*
           3. DYNAMIC DAY/NIGHT ENVIRONMENT INTERPOLATOR (PHASE A4)
              Calculates a time phase sector based on our active hardware clock ticks.
              Cycles every 600 ticks through Day, Dusk, Night, and Dawn states.
        */
        uint32_t time_cycle = live_distro_ticks % 600;
        uint8_t environmental_color = ATTR_SLATE_GRAY;
        const char* time_label = "NIGHT";

        if (time_cycle < 150) {
            environmental_color = ATTR_MINT_GREEN; // High-noon brilliant day slate
            time_label = "DAY  ";
        } else if (time_cycle >= 150 && time_cycle < 300) {
            environmental_color = ATTR_GOLD;       // Sunset golden transition phase
            time_label = "DUSK ";
        } else if (time_cycle >= 300 && time_cycle < 450) {
            environmental_color = ATTR_SLATE_GRAY; // Deep night mask tone
            time_label = "NIGHT";
        } else {
            environmental_color = ATTR_LIGHT_PURPLE; // Dawn twilight purple accent
            time_label = "DAWN ";
        }

        // 4. Render our high-contrast distribution header banners
        print_string_raw(1, 10, "==================================================", ATTR_GOLD);
        print_string_raw(2, 10, "  ANTAGONIST OS LIVE WORKSPACE PLATFORM INSTANCE   ", ATTR_GOLD);
        print_string_raw(3, 10, "==================================================", ATTR_GOLD);

        // 5. Output real-time system performance telemetry indices
        print_string_raw(5, 12, "DISTRO STATUS: RUNNING (MONOLITHIC MASTER)", ATTR_WHITE);
        print_string_raw(6, 12, "REAL-TIME TELEMETRY SYSTEM TICKS: ", ATTR_LIGHT_CYAN);
        print_string_raw(6, 46, tick_string, ATTR_MINT_GREEN);

        // Output our active day/night cycle engine telemetry text character states
        print_string_raw(7, 12, "ENVIRONMENT ENVIRONMENT MATRIX INDEX: ", ATTR_LIGHT_CYAN);
        print_string_raw(7, 50, time_label, environmental_color);

        // 6. Render our Phase A3 Inventory Selection Interface Bar
        print_string_raw(9, 12, "CREATIVE HOTBAR SELECTION:", ATTR_WHITE);
        print_string_raw(10, 12, "ACTIVE COMPONENT VELD: ", ATTR_LIGHT_CYAN);
        print_string_raw(10, 35, "              ", ATTR_WHITE);
        print_string_raw(10, 35, get_active_material_name(), get_active_material_attribute());

        // 7. Compute and render our Phase A2 Procedural Terrain Generation Maps
        print_string_raw(12, 10, "--- PROCEDURAL TOPOLOGY STREAM (PHASE A2) ---", ATTR_LIGHT_CYAN);
        generate_procedural_topography(live_distro_ticks, terrain_matrix);

        // Rasterize the scrolling landscape lines utilizing our live dynamic environmental color!
        for (int y = 0; y < 5; y++) {
            char line_temp_buffer[81];
            for (int x = 0; x < 80; x++) {
                line_temp_buffer[x] = terrain_matrix[(y * 80) + x];
            }
            line_temp_buffer[80] = '\0';
            print_string_raw(14 + y, 0, line_temp_buffer, environmental_color);
        }

        print_string_raw(20, 10, "==================================================", ATTR_GOLD);

        // 8. Automated safe shutdown checkpoint simulation gate set to 5,000 ticks
        if (live_distro_ticks >= 5000) {
            for (int i = 0; i < 4000; i += 2) {
                ((volatile char*)0xB8000)[i] = ' ';
                ((volatile char*)0xB8000)[i+1] = 0x07;
            }
            print_string_raw(11, 15, "ANTAGONIST OS DISTRIBUTION POWERED DOWN SAFELY", ATTR_GOLD);
            print_string_raw(12, 24, "SYSTEM CORE TERMINATED HALT", ATTR_WHITE);

            while (true) {
                asm volatile("cli; hlt");
            }
        }

        for (volatile uint32_t delay = 0; delay < 1200000; delay++);
    }
}
