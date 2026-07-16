#include "vfat.h"

/*
   The Master Static RAM Partition Instance.
   Allocates a flat global state tracker container in the data segment area.
*/
static VRamDiskState vram_disk __attribute__((aligned(4)));

/*
   Lightweight freestanding string copier.
   Ensures safe, bounded character array assignment without standard libraries.
*/
static void safe_string_copy(char* dest, const char* src, uint32_t max_len) {
    uint32_t i = 0;
    while (src[i] != '\0' && i < (max_len - 1)) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

/*
   Initializes the VFAT virtual data layers at boot time.
*/
extern "C" void init_vfat_disk() {
    vram_disk.active_files_count = 0;
    for (int i = 0; i < MAX_FILE_ENTRIES; i++) {
        vram_disk.files[i].is_active = 0;
        vram_disk.files[i].filename[0] = '\0';
        vram_disk.files[i].data_payload[0] = '\0';
        vram_disk.files[i].folder_index_owner = 0xFFFFFFFF;
        vram_disk.files[i].file_size_bytes = 0;
    }
}

/*
   Creates a virtual allocation file descriptor and links it to a directory owner index.
*/
extern "C" void create_virtual_file(const char* name, uint32_t folder_idx, const char* content) {
    if (vram_disk.active_files_count >= MAX_FILE_ENTRIES) return;

    // Find the next available empty entry slot loop lane
    uint32_t target_slot = 0xFFFFFFFF;
    for (uint32_t i = 0; i < MAX_FILE_ENTRIES; i++) {
        if (vram_disk.files[i].is_active == 0) {
            target_slot = i;
            break;
        }
    }

    if (target_slot == 0xFFFFFFFF) return;

    // Lock character payload allocations
    safe_string_copy(vram_disk.files[target_slot].filename, name, MAX_FILENAME_LEN);
    safe_string_copy(vram_disk.files[target_slot].data_payload, content, 64);

    vram_disk.files[target_slot].folder_index_owner = folder_idx;
    vram_disk.files[target_slot].is_active = 1;

    // Compute basic byte tracking size
    uint32_t len = 0;
    while (content[len] != '\0' && len < 64) len++;
    vram_disk.files[target_slot].file_size_bytes = len;

    vram_disk.active_files_count++;
}

/*
   Returns the filename matching a specific folder context and index loop lane.
*/
extern "C" const char* get_virtual_file_name(uint32_t index, uint32_t folder_idx) {
    uint32_t structural_matches = 0;
    for (uint32_t i = 0; i < MAX_FILE_ENTRIES; i++) {
        if (vram_disk.files[i].is_active == 1 && vram_disk.files[i].folder_index_owner == folder_idx) {
            if (structural_matches == index) {
                return vram_disk.files[i].filename;
            }
            structural_matches++;
        }
    }
    return nullptr;
}

/*
   Returns the character buffer content matching a folder context index slot.
*/
extern "C" const char* get_virtual_file_content(uint32_t index, uint32_t folder_idx) {
    uint32_t structural_matches = 0;
    for (uint32_t i = 0; i < MAX_FILE_ENTRIES; i++) {
        if (vram_disk.files[i].is_active == 1 && vram_disk.files[i].folder_index_owner == folder_idx) {
            if (structural_matches == index) {
                return vram_disk.files[i].data_payload;
            }
            structural_matches++;
        }
    }
    return nullptr;
}

/*
   Returns the total current count of active files assigned to a directory node.
*/
extern "C" uint32_t get_folder_file_count(uint32_t folder_idx) {
    uint32_t total = 0;
    for (uint32_t i = 0; i < MAX_FILE_ENTRIES; i++) {
        if (vram_disk.files[i].is_active == 1 && vram_disk.files[i].folder_index_owner == folder_idx) {
            total++;
        }
    }
    return total;
}
