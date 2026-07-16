#ifndef VFAT_H
#define VFAT_H

#include <stdint.h>

#define MAX_FILENAME_LEN   12
#define MAX_FILE_ENTRIES   8
#define VRAM_DISK_SIZE     1024

/*
   The Blueprint for a VFAT File Allocation Node Entry.
   Maintains the active file visibility state, directory association indices,
   and character content buffer data payloads without dynamic heap tracking.
*/
struct VFileEntry {
    char filename[MAX_FILENAME_LEN];
    uint32_t folder_index_owner;
    uint32_t file_size_bytes;
    char data_payload[64];
    uint8_t is_active;
};

/* Master tracking structure for our isolated Virtual RAM Disk Partition */
struct VRamDiskState {
    VFileEntry files[MAX_FILE_ENTRIES];
    uint32_t active_files_count;
};

/* Expose our standalone VFAT storage management functions to C++ linkage maps */
extern "C" {
    void init_vfat_disk();
    void create_virtual_file(const char* name, uint32_t folder_idx, const char* content);
    const char* get_virtual_file_name(uint32_t index, uint32_t folder_idx);
    const char* get_virtual_file_content(uint32_t index, uint32_t folder_idx);
    uint32_t get_folder_file_count(uint32_t folder_idx);
}

#endif
