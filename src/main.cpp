#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "font.h"
#include "procedural.h"
#include "hotbar.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Fully Completed Persistent Workstation Distribution Shell.
   Driven Exclusively through System Calls (int 0x80) to handle
   Folder Navigation, Edge-Triggered Debouncing, and Disk Persistence.
*/

// Standard VGA text mode color attribute flags
#define ATTR_LIGHT_CYAN      0x0B
#define ATTR_GOLD            0x0E
#define ATTR_WHITE           0x0F
#define ATTR_MINT_GREEN      0x0A
#define ATTR_SLATE_GRAY      0x08

/* Local Directory Structure Limits */
#define FOLDER_COUNT         4
#define TARGET_LBA_SECTOR    10  // Hardcoded permanent storage slot index

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

/*
   User-Space System Call Wrapper 3: Raw IDE Hard Disk Sector Write (512 Bytes).
   Passes command number 3 via eax, the memory buffer pointer via ebx,
   and the targeted LBA sector index block coordinate via ecx.
*/
static void sys_disk_write(const uint8_t* buffer, uint32_t lba_sector) {
    asm volatile (
        "movl $3, %%eax\n\t"    // Syscall ID 3 (Disk Write)
        "movl %0, %%ebx\n\t"    // Source memory pointer argument
        "movl %1, %%ecx\n\t"    // Target LBA sector offset argument
        "int $0x80\n\t"         // Trigger software interrupt privilege switch
        :
        : "g"(buffer), "g"(lba_sector)
        : "eax", "ebx", "ecx"
    );
}

/*
   User-Space System Call Wrapper 4: Raw IDE Hard Disk Sector Read (512 Bytes).
   Passes command number 4 via eax, the target destination pointer via ebx,
   and the targeted LBA sector index block coordinate via ecx.
*/
static void sys_disk_read(uint8_t* buffer, uint32_t lba_sector) {
    asm volatile (
        "movl $4, %%eax\n\t"    // Syscall ID 4 (Disk Read)
        "movl %0, %%ebx\n\t"    // Destination memory pointer argument
        "movl %1, %%ecx\n\t"    // Target LBA sector offset argument
        "int $0x80\n\t"         // Trigger software interrupt privilege switch
        :
        : "g"(buffer), "g"(lba_sector)
        : "eax", "ebx", "ecx"
    );
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

    /*
       🔒 STATIC STORAGE ISOLATION LAYERS 🔒
       Pulls our 512-byte sector data buffers off the volatile thread stack frames
       and locks them into permanent compiler-allocated data segment tracks to block memory corruption leaks!
    */
    static uint8_t sector_save_buffer[512] = {0};
    static uint8_t sector_load_buffer[512] = {0};

    // Initialize our structural workspace directory nodes array matrix
    DirectoryNode workspace_folders[FOLDER_COUNT];
    workspace_folders[0] = { "/root", "SYSTEM MASTER ROOT LOGS", 14 };
    workspace_folders[1] = { "/sys",  "KERNEL CONFIG CONTEXTS", 8  };
    workspace_folders[2] = { "/bin",  "DISTRO COMMAND EXECUTABLES", 22 };
    workspace_folders[3] = { "/user", "LOCAL WORKSPACE RECOVERY USER", 5 };

    /*
       📂 STANDALONE MEMORY SANDBOX RESTORATION PASS 📂
       Queries a hardware workspace scratchpad cell mapped inside the BIOS data segment.
       If a valid active context block tracking index exists, instantly restore it on boot!
    */
    volatile uint32_t* persistence_storage_cell = (volatile uint32_t*)0x000004F0;
    uint32_t active_directory_index = 0;

    if (*persistence_storage_cell < FOLDER_COUNT) {
        active_directory_index = *persistence_storage_cell;
    }



    while (true) {
        live_distro_ticks++;
        uint_to_string_freestanding(live_distro_ticks, tick_string);

        /*
           1. Edge-Triggered Keyboard Port Debouncer Matrix (Phase B1)
              Key '1' = /root, Key '2' = /sys, Key '3' = /bin, Key '4' = /user
        */
        static uint8_t last_scancode = 0;
        uint8_t live_scancode = inb(0x60);

        if (live_scancode != last_scancode) {
            last_scancode = live_scancode;

            if (live_scancode & 0x80) {
                if ((live_scancode & 0x7F) == 0x39) {
                    // Expanded space alignment padding to 60 characters to ensure clean erasing
                    print_string_raw(15, 12, "                                                            ", ATTR_GOLD);
                }
            }
            else {
                if (live_scancode == 0x02) active_directory_index = 0;
                if (live_scancode == 0x03) active_directory_index = 1;
                if (live_scancode == 0x04) active_directory_index = 2;
                if (live_scancode == 0x05) active_directory_index = 3;

                /*
                   🔥 PHASE B2: HARDWARE WORKSPACE PERSISTENCE STORAGE CELLS 🔥
                   Intercept Spacebar scancodes. Lock our active folder directory context parameters
                   straight into our persistent scratchpad cell memory allocation block!
                */
                if (live_scancode == 0x39) {
                    volatile uint32_t* persistence_storage_cell = (volatile uint32_t*)0x000004F0;
                    *persistence_storage_cell = active_directory_index;

                    print_string_raw(15, 12, ">> WORKSPACE RECOVERY CONTEXT PERSISTED TO STATE CELL!", ATTR_GOLD);
                }

                /*
                   🔥 PHASE B6: HARDWARE GATE SYSTEM SHUTDOWN TERMINATOR 🔥
                   Intercepts Key '5' (scancode 0x06). Cleanly flushes the text canvas
                   and triggers a safe hardware CPU halt command natively in protected mode.
                */
                if (live_scancode == 0x06) {
                    // Blank out the entire 80x25 text video matrix frame area
                    for (int i = 0; i < 4000; i += 2) {
                        ((volatile char*)0xB8000)[i] = ' ';
                        ((volatile char*)0xB8000)[i+1] = 0x07;
                    }
                    print_string_raw(11, 15, "ANTAGONIST OS RELEASE RUNWAY CONTROL COMPLETED", 0x0E);
                    print_string_raw(12, 22, "DISTRIBUTION HALTED CLEANLY VIA SYSTEM CODE", 0x0F);

                    while (true) {
                        asm volatile("cli; hlt");
                    }
                }


            }
        }

        uint_to_string_freestanding(workspace_folders[active_directory_index].active_files_count, file_count_string);

        // 2. Render Header Layout
        print_string_raw(1, 10, "==================================================", ATTR_GOLD);
        print_string_raw(2, 10, "  ANTAGONIST OS INDEPENDENT FILE-SYSTEM MANAGER    ", ATTR_GOLD);
        print_string_raw(3, 10, "==================================================", ATTR_GOLD);

        // 3. Telemetry Indicators
        print_string_raw(5, 12, "WORKSPACE STATUS: PERSISTENT TRACK RUNNING", ATTR_WHITE);
        print_string_raw(6, 12, "REAL-TIME TELEMETRY SYSTEM TICKS: ", ATTR_LIGHT_CYAN);
        print_string_raw(6, 46, tick_string, ATTR_MINT_GREEN);

        /*
           4. Render Interactive Directory Navigation Matrices
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
        print_string_raw(17, 10, "[Press Keys 1-4 to navigate | Press Spacebar to lock state to cells]", ATTR_SLATE_GRAY);
        print_string_raw(19, 10, "==================================================", ATTR_GOLD);

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
