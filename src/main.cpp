#include <stdint.h>

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Direct text-mode video engine. Writes character bytes straight to 0xB8000
   to completely avoid VGA register clocking failures and black screens.
*/

// Standard VGA text mode color attribute flags
#define ATTR_LIGHT_CYAN      0x0B
#define ATTR_GOLD            0x0E
#define ATTR_WHITE           0x0F
#define ATTR_MINT_GREEN      0x0A

/*
   Lightweight freestanding string printer.
   Pokes characters and styles directly into the 80x25 text-video memory grid.
*/
static void print_string_raw(int row, int col, const char* str, uint8_t attribute) {
    volatile char* video_memory = (volatile char*)0xB8000;
    // Calculate the linear memory index tracking slot for the 80-column row grid
    int index = (row * 80 + col) * 2;

    int i = 0;
    while (str[i] != '\0') {
        video_memory[index] = str[i];          // Character byte
        video_memory[index + 1] = attribute;   // Style color byte
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

    while (true) {
        live_distro_ticks++;
        uint_to_string_freestanding(live_distro_ticks, tick_string);

        // 1. Render our high-contrast distribution header banners
        print_string_raw(11, 10, "==================================================", ATTR_GOLD);
        print_string_raw(12, 10, "  ANTAGONIST OS LIVE WORKSPACE PLATFORM INSTANCE   ", ATTR_GOLD);
        print_string_raw(13, 10, "==================================================", ATTR_GOLD);

        // 2. Output real-time system performance telemetry indices
        print_string_raw(15, 12, "DISTRO STATUS: RUNNING (MONOLITHIC MASTER)", ATTR_WHITE);
        print_string_raw(16, 12, "REAL-TIME TELEMETRY SYSTEM TICKS: ", ATTR_LIGHT_CYAN);
        print_string_raw(16, 46, tick_string, ATTR_MINT_GREEN);

        // 3. Automated safe shutdown checkpoint simulation gate
        if (live_distro_ticks >= 1200) {
            // Clear the workspace and display our final clean standby metrics
            for (int i = 0; i < 4000; i += 2) {
                ((volatile char*)0xB8000)[i] = ' ';
                ((volatile char*)0xB8000)[i+1] = 0x07;
            }
            print_string_raw(12, 15, "ANTAGONIST OS DISTRIBUTION POWERED DOWN SAFELY", ATTR_GOLD);
            print_string_raw(13, 24, "SYSTEM CORE TERMINATED HALT", ATTR_WHITE);

            // Lock the CPU lanes permanently into standby rest modes
            while (true) {
                asm volatile("cli; hlt");
            }
        }

        // Execution speed control delay loop to keep the counter human-readable
        for (volatile uint32_t delay = 0; delay < 2000000; delay++);
    }
}
