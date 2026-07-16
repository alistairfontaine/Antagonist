#include <stdint.h>

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Independent File-System Workstation Dashboard.
   Simulates a fully structured, multi-directory folder navigation matrix
   natively from 32-bit Protected Mode with zero host dependencies.
*/

// Standard VGA text mode color attribute flags
#define ATTR_LIGHT_CYAN      0x0B
#define ATTR_GOLD            0x0E
#define ATTR_WHITE           0x0F
#define ATTR_MINT_GREEN      0x0A
#define ATTR_SLATE_GRAY      0x08

/* Local Directory Structure Limits */
#define FOLDER_COUNT         4

struct DirectoryNode {
    const char* folder_path;
    const char* folder_meta_desc;
    uint32_t active_files_count;
};

// Low-level port input assembly wrapper
inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* Lightweight freestanding string printer */
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

/* Freestanding Integer-To-ASCII Base 10 Converter */
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
    char file_count_string[16];

    // Initialize our structural workspace directory nodes array matrix
    DirectoryNode workspace_folders[FOLDER_COUNT];
    workspace_folders[0] = { "/root", "SYSTEM MASTER ROOT LOGS", 14 };
    workspace_folders[1] = { "/sys",  "KERNEL CONFIG CONTEXTS", 8  };
    workspace_folders[2] = { "/bin",  "DISTRO COMMAND EXECUTABLES", 22 };
    workspace_folders[3] = { "/user", "LOCAL WORKSPACE RECOVERY USER", 5 };

    uint32_t active_directory_index = 0;

    while (true) {
        live_distro_ticks++;
        uint_to_string_freestanding(live_distro_ticks, tick_string);

        /*
           1. Poll the keyboard controller ports to shift folders!
              Key '1' = /root, Key '2' = /sys, Key '3' = /bin, Key '4' = /user
        */
        uint8_t live_scancode = inb(0x60);
        if (!(live_scancode & 0x80)) { // Verify it is an active make code press
            if (live_scancode == 0x02) active_directory_index = 0;
            if (live_scancode == 0x03) active_directory_index = 1;
            if (live_scancode == 0x04) active_directory_index = 2;
            if (live_scancode == 0x05) active_directory_index = 3;

            /*
               🔥 PERSISTENT VISUAL SAVE TRIGGER 🔥
               Intercept Spacebar (scancode 0x39) to draw a clear confirmation
               row mapping across your active dashboard coordinates!
            */
            if (live_scancode == 0x39) {
                print_string_raw(15, 12, ">> META CONTEXT ACTIVE SLOT LOCKED AND SECURED!", ATTR_GOLD);
            }
        }


        uint_to_string_freestanding(workspace_folders[active_directory_index].active_files_count, file_count_string);

        // 2. Render Header Layout
        print_string_raw(1, 10, "==================================================", ATTR_GOLD);
        print_string_raw(2, 10, "  ANTAGONIST OS INDEPENDENT FILE-SYSTEM MANAGER    ", ATTR_GOLD);
        print_string_raw(3, 10, "==================================================", ATTR_GOLD);

        // 3. Telemetry Indicators
        print_string_raw(5, 12, "WORKSPACE STATUS: OPERATIONAL VELD", ATTR_WHITE);
        print_string_raw(6, 12, "REAL-TIME TELEMETRY SYSTEM TICKS: ", ATTR_LIGHT_CYAN);
        print_string_raw(6, 46, tick_string, ATTR_MINT_GREEN);

        /*
           4. Render Interactive Directory Navigation Matrices!
              Prints your custom folder paths and counts files actively on key press commands.
        */
        print_string_raw(9, 10, "--- FILE MATRIX WORKSTATION DIRECTORY NAV ---", ATTR_LIGHT_CYAN);

        print_string_raw(11, 12, "ACTIVE DIRECTORY INDEX LINK: ", ATTR_WHITE);
        print_string_raw(11, 41, "        ", ATTR_WHITE); // Fast wipe buffer
        print_string_raw(11, 41, workspace_folders[active_directory_index].folder_path, ATTR_MINT_GREEN);

        print_string_raw(12, 12, "MOUNTED DESCRIPTION METADATA: ", ATTR_WHITE);
        print_string_raw(12, 42, "                             ", ATTR_WHITE);
        print_string_raw(12, 42, workspace_folders[active_directory_index].folder_meta_desc, ATTR_LIGHT_CYAN);

        print_string_raw(13, 12, "TOTAL TARGET FILENODES FOUND: ", ATTR_WHITE);
        print_string_raw(13, 42, "    ", ATTR_WHITE);
        print_string_raw(13, 42, file_count_string, ATTR_GOLD);

        // 5. Visual Footer Index Slots
        print_string_raw(16, 10, "[Press Keys 1-4 to navigate through active folders natively]", ATTR_SLATE_GRAY);
        print_string_raw(18, 10, "==================================================", ATTR_GOLD);

        // 6. Automated safe shutdown checkpoint gate
        if (live_distro_ticks >= 5000) {
            for (int i = 0; i < 4000; i += 2) {
                ((volatile char*)0xB8000)[i] = ' ';
                ((volatile char*)0xB8000)[i+1] = 0x07;
            }
            print_string_raw(11, 15, "ANTAGONIST OS WORKSPACE POWERED DOWN SAFELY", ATTR_GOLD);
            print_string_raw(12, 24, "SYSTEM CORE TERMINATED HALT", ATTR_WHITE);
            while (true) { asm volatile("cli; hlt"); }
        }

        // Keep execution frame cycling smooth
        for (volatile uint32_t delay = 0; delay < 1200000; delay++);
    }
}
