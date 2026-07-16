#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"
#include "procedural.h"
#include "hotbar.h"
#include "vfat.h"
#include "loader.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Independent File-System Workstation Dashboard with integrated
   VFAT RAM-disk storage cataloging and Bounded Binary Execution Modules.
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
    char status_string[16];

    // Initialize our Virtual RAM disk partition entries
    init_vfat_disk();

    // Seed sample persistent data filenodes directly into directory tracks
    create_virtual_file("sys_init.sys", 0, "SUCCESS: KERNEL CONTEXT INITIALIZED PRIVILEGE LEVEL 0");
    create_virtual_file("readme.txt",   0, "WELCOME TO THE INDEPENDENT ANTAGONIST LIVE WORKSPACE");
    create_virtual_file("config.cfg",   1, "SET ARCH_MODE=32; SET TELEMETRY_HZ=100; SET FPU=1");
    create_virtual_file("sh_core.bin",  2, "EXEC_BIN_OFFSET_LOADER_GATE_0X80");
    create_virtual_file("profile.usr",  3, "USER: ALISTAIR FONTAINE; ROLE: SENIOR SYSTEMS MASTER");

    // Initialize our structural workspace directory nodes array matrix
    DirectoryNode workspace_folders[FOLDER_COUNT];
    workspace_folders[0] = { "/root", "SYSTEM MASTER ROOT LOGS", 0 };
    workspace_folders[1] = { "/sys",  "KERNEL CONFIG CONTEXTS", 0  };
    workspace_folders[2] = { "/bin",  "DISTRO COMMAND EXECUTABLES", 0 };
    workspace_folders[3] = { "/user", "LOCAL WORKSPACE RECOVERY USER", 0 };

    uint32_t active_directory_index = 0;
    const char* active_exec_log = "[No binary files executed yet]";
    uint32_t active_exec_status = 0;

    while (true) {
        live_distro_ticks++;
        uint_to_string_freestanding(live_distro_ticks, tick_string);
        uint_to_string_freestanding(active_exec_status, status_string);

        // Dynamically pull the live structural file counts directly from the VFAT storage manager tables
        for (uint32_t f = 0; f < FOLDER_COUNT; f++) {
            workspace_folders[f].active_files_count = get_folder_file_count(f);
        }

        /*
           3. Edge-Triggered Keyboard Port Debouncer Matrix (Phase B1)
              Key '1' = /root, Key '2' = /sys, Key '3' = /bin, Key '4' = /user
        */
        static uint8_t last_scancode = 0;
        uint8_t live_scancode = inb(0x60);

        if (live_scancode != last_scancode) {
            last_scancode = live_scancode;

            if (live_scancode & 0x80) {
                if ((live_scancode & 0x7F) == 0x39) {
                    print_string_raw(15, 12, "                                                            ", ATTR_GOLD);
                }
            }
            else {
                if (live_scancode == 0x02) active_directory_index = 0;
                if (live_scancode == 0x03) active_directory_index = 1;
                if (live_scancode == 0x04) active_directory_index = 2;
                if (live_scancode == 0x05) active_directory_index = 3;

                /*
                   🔥 PHASE B3: BOUNDED EXECUTABLE BINARY OBJECT LOAD RUNNER 🔥
                   Intercept Enter key (scancode 0x1C). If user is inside /bin, parse and execute
                   the active compiled program payload straight out of our VFAT disk segments!
                */
                if (live_scancode == 0x1C) {
                    if (active_directory_index == 2) { // Verification boundary gate locked to /bin folder slot
                        const char* target_prog = get_virtual_file_name(0, active_directory_index);
                        const char* target_code = get_virtual_file_content(0, active_directory_index);

                        if (target_prog && target_code) {
                            execute_bounded_binary(target_prog, target_code);
                            active_exec_log = get_active_process_log();
                            active_exec_status = get_active_process_status();
                        }
                    }
                }

                if (live_scancode == 0x39) {
                    print_string_raw(15, 12, ">> WORKSPACE RECOVERY CONTEXT PERSISTED TO STATE CELL!", ATTR_GOLD);
                }

                if (live_scancode == 0x06) {
                    for (int i = 0; i < 4000; i += 2) {
                        ((volatile char*)0xB8000)[i] = ' ';
                        ((volatile char*)0xB8000)[i+1] = 0x07;
                    }
                    print_string_raw(11, 15, "ANTAGONIST OS RELEASE RUNWAY CONTROL COMPLETED", ATTR_GOLD);
                    print_string_raw(12, 22, "DISTRIBUTION HALTED CLEANLY VIA SYSTEM CODE", ATTR_WHITE);
                    while (true) { asm volatile("cli; hlt"); }
                }
            }
        }

        uint_to_string_freestanding(workspace_folders[active_directory_index].active_files_count, file_count_string);

        // 4. Render Header Layout
        print_string_raw(1, 10, "==================================================", ATTR_GOLD);
        print_string_raw(2, 10, "  ANTAGONIST OS INDEPENDENT FILE-SYSTEM MANAGER    ", ATTR_GOLD);
        print_string_raw(3, 10, "==================================================", ATTR_GOLD);

        // 5. Telemetry Indicators
        print_string_raw(5, 12, "WORKSPACE STATUS: PERSISTENT TRACK RUNNING", ATTR_WHITE);
        print_string_raw(6, 12, "REAL-TIME TELEMETRY SYSTEM TICKS: ", ATTR_LIGHT_CYAN);
        print_string_raw(6, 46, tick_string, ATTR_MINT_GREEN);

        // 6. Render Interactive Directory Navigation Matrices
        print_string_raw(8, 10, "--- FILE MATRIX WORKSTATION DIRECTORY NAV ---", ATTR_LIGHT_CYAN);

        print_string_raw(10, 12, "ACTIVE DIRECTORY INDEX LINK: ", ATTR_WHITE);
        print_string_raw(10, 41, "        ", ATTR_WHITE);
        print_string_raw(10, 41, workspace_folders[active_directory_index].folder_path, ATTR_MINT_GREEN);

        print_string_raw(11, 12, "MOUNTED DESCRIPTION METADATA: ", ATTR_WHITE);
        print_string_raw(11, 42, "                             ", ATTR_WHITE);
        print_string_raw(11, 42, workspace_folders[active_directory_index].folder_meta_desc, ATTR_LIGHT_CYAN);

        print_string_raw(12, 12, "TOTAL TARGET FILENODES FOUND: ", ATTR_WHITE);
        print_string_raw(12, 42, "    ", ATTR_WHITE);
        print_string_raw(12, 42, file_count_string, ATTR_GOLD);

        // Live VFAT Catalog Listing Pass
        print_string_raw(14, 12, "--- LIVE STORAGE CATALOG MATRIX IN DIRECTORY ---", ATTR_SLATE_GRAY);
        print_string_raw(15, 12, "                                                            ", ATTR_WHITE);
        print_string_raw(16, 12, "                                                            ", ATTR_WHITE);

        uint32_t current_folder_files = workspace_folders[active_directory_index].active_files_count;
        if (current_folder_files == 0) {
            print_string_raw(15, 14, "[Directory is empty]", ATTR_SLATE_GRAY);
        } else {
            for (uint32_t idx = 0; idx < current_folder_files && idx < 2; idx++) {
                const char* f_name = get_virtual_file_name(idx, active_directory_index);
                const char* f_cont = get_virtual_file_content(idx, active_directory_index);

                if (f_name && f_cont) {
                    print_string_raw(15 + idx, 14, f_name, ATTR_MINT_GREEN);
                    print_string_raw(15 + idx, 28, "-> ", ATTR_WHITE);
                    print_string_raw(15 + idx, 31, f_cont, ATTR_WHITE);
                }
            }
        }

        /*
           🔥 INTERACTIVE PROCESS MONITOR OVERLAY VIEWPORT 🔥
           Pulls real-time insulated pipeline registers from the object loader tracker!
        */
        print_string_raw(18, 12, "--- PROCESS MONITOR EXECUTABLE LOG INTERCEPTOR ---", ATTR_SLATE_GRAY);
        print_string_raw(19, 12, "SYS APP RUN CODE STATUS: ", ATTR_WHITE);
        print_string_raw(19, 37, "    ", ATTR_WHITE); // Clear trace
        print_string_raw(19, 37, status_string, ATTR_GOLD);

        print_string_raw(20, 12, "LIVE TRACK REGISTER: ", ATTR_WHITE);
        print_string_raw(20, 23, "                                                         ", ATTR_WHITE); // Bounded clear buffer
        print_string_raw(20, 23, active_exec_log, ATTR_LIGHT_CYAN);

        // 7. Visual Footer Index Slots

print_string_raw(22, 10, "[Keys 1-4: Change Folders | Enter: Exec /bin Programs | Key 5: Exit]", ATTR_SLATE_GRAY);
print_string_raw(23, 10, "==================================================", ATTR_GOLD);
// 8. Automated safe shutdown checkpoint gate
if (live_distro_ticks >= 5000) {
for (int i = 0; i < 4000; i += 2) {
((volatile char*)0xB8000)[i] = ' ';
((volatile char*)0xB8000)[i+1] = 0x07;
}
print_string_raw(11, 15, "ANTAGONIST OS WORKSPACE POWERED DOWN SAFELY", ATTR_GOLD);
print_string_raw(12, 24, "SYSTEM CORE TERMINATED HALT", ATTR_WHITE);
while (true) { asm volatile("cli; hlt"); }
}
for (volatile uint32_t delay = 0; delay < 1200000; delay++);
}
}
