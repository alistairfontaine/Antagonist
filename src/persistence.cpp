#include "persistence.h"

// Extern our live world matrix array tracking structural tiles inside src/raycast.cpp
extern uint8_t world_map[MAP_WIDTH * MAP_HEIGHT];

// External ATA hardware driver hooks living within our adjacent FontaineOS kernel
extern "C" {
    void ata_write_sector(uint32_t lba, const uint8_t* buffer);
    void ata_read_sector(uint32_t lba, uint8_t* buffer);
}

/*
   Flushes the entire active 3D voxel sandbox coordinate array directly down
   to virtual Sector 2 of your master disk image plates.
*/
extern "C" void dump_sandbox_to_disk() {
    /*
       Enforce a flat static memory landing pad to satisfy the 512-byte hardware
       sector size constraints required by the motherboard IDE controller pins.
    */
    uint8_t sector_flush_pad[512] __attribute__((aligned(4))) = {0};

    // Pack our active 256-byte world matrix straight into the front of the hardware buffer
    for (uint32_t i = 0; i < (MAP_WIDTH * MAP_HEIGHT); i++) {
        sector_flush_pad[i] = world_map[i];
    }

    /*
       Call your kernel's underlying storage driver directly!
       Blasts the compiled 512-byte state packet instantly over the system I/O ports.
    */
    ata_write_sector(WORLD_SAVE_LBA_SECTOR, sector_flush_pad);
}

/*
   Streams raw data blocks off virtual Sector 2 back into system RAM, instantly
   restoring your complete pre-saved voxel block configurations at boot time.
*/
extern "C" void load_sandbox_from_disk() {
    uint8_t sector_load_pad[512] __attribute__((aligned(4))) = {0};

    /*
       Call your kernel's underlying storage driver directly!
       Pulls 512 bytes right off the disk plates straight into our aligned memory landing zone.
    */
    ata_read_sector(WORLD_SAVE_LBA_SECTOR, sector_load_pad);

    // Unpack the first 256 bytes straight back into our live operational world map array
    for (uint32_t i = 0; i < (MAP_WIDTH * MAP_HEIGHT); i++) {
        world_map[i] = sector_load_pad[i];
    }
}
