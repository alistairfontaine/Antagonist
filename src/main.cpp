#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"
#include "procedural.h"
#include "hotbar.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Direct Hardware Port Polling Engine. Bypasses restricted kernel interrupt
   line blocks to read keyboard controller registers natively in protected mode.
*/

// Standard VGA text mode color attribute flags
#define ATTR_LIGHT_CYAN      0x0B
#define ATTR_GOLD            0x0E
#define ATTR_WHITE           0x0F
#define ATTR_MINT_GREEN      0x0A
#define ATTR_SLATE_GRAY      0x08

/* Low-level port input assembly wrapper to query keyboard data lines directly */
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

        /*
           2. DIRECT HARDWARE CHIP PORT POLLING MATRIX
              Bypasses standard interrupt blockages by sampling I/O port 0x60 directly.
              Maps scancodes 0x02, 0x03, 0x04, 0x05 straight to keys 1, 2, 3, 4.
        */
        uint8_t live_scancode = inb(0x60);
        if (live_scancode >= 0x02 && live_scancode <= 0x05) {
            update_hotbar_selection(live_scancode);
        }

        // 3. Render our high-contrast distribution header banners
        print_string_raw(1, 10, "==================================================", ATTR_GOLD);
        print_string_raw(2, 10, "  ANTAGONIST OS LIVE WORKSPACE PLATFORM INSTANCE   ", ATTR_GOLD);
        print_string_raw(3, 10, "==================================================", ATTR_GOLD);

        // 4. Output real-time system performance telemetry indices
        print_string_raw(5, 12, "DISTRO STATUS: RUNNING (MONOLITHIC MASTER)", ATTR_WHITE);
        print_string_raw(6, 12, "REAL-TIME TELEMETRY SYSTEM TICKS: ", ATTR_LIGHT_CYAN);
        print_string_raw(6, 46, tick_string, ATTR_MINT_GREEN);

        /*
           5. Render our Phase A3 Inventory Selection Interface Bar!
        */
        print_string_raw(8, 12, "CREATIVE HOTBAR SELECTION:", ATTR_WHITE);
        print_string_raw(9, 12, "ACTIVE COMPONENT VELD: ", ATTR_LIGHT_CYAN);

        // Wipe old material slot labels quickly to ensure crisp variable layout text
        print_string_raw(9, 35, "              ", ATTR_WHITE);
        print_string_raw(9, 35, get_active_material_name(), get_active_material_attribute());

        // 6. Compute and render our Phase A2 Procedural Terrain Generation Maps
        print_string_raw(11, 10, "--- PROCEDURAL TOPOLOGY STREAM (PHASE A2) ---", ATTR_LIGHT_CYAN);
        generate_procedural_topography(live_distro_ticks, terrain_matrix);

        for (int y = 0; y < 5; y++) {
            char line_temp_buffer[81];
            for (int x = 0; x < 80; x++) {
                line_temp_buffer[x] = terrain_matrix[(y * 80) + x];
            }
            line_temp_buffer[80] = '\0';
            print_string_raw(13 + y, 0, line_temp_buffer, ATTR_SLATE_GRAY);
        }

        print_string_raw(19, 10, "==================================================", ATTR_GOLD);

        // 7. Automated safe shutdown checkpoint simulation gate set to 5,000 ticks
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
