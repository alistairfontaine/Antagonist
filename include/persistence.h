#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdint.h>
#include "raycast.h"

/*
   The Sandbox Storage Disk Geometry Bounds.
   Maps your active world coordinate data state matrices to live starting exactly
   at virtual sector LBA 2 of your master disk image layout, completely insulated
   from bootloader blocks and neural network weights arrays.
*/
#define WORLD_SAVE_LBA_SECTOR    2

/* Expose our raw sandbox serialization routines cleanly to C linkage maps */
extern "C" {
    void dump_sandbox_to_disk();
    void load_sandbox_from_disk();
}

#endif
