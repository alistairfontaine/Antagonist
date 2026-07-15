#ifndef MODIFICATION_H
#define MODIFICATION_H

#include <stdint.h>
#include "math3d.h"
#include "raycast.h"
#include "selection.h"

/* Voxel World Modification Command Flags */
#define VOXEL_ACTION_DESTROY    0
#define VOXEL_ACTION_PLACE      1

/* Expose our raw block placement and deletion routines cleanly to C linkage maps */
extern "C" {
    void execute_voxel_mutation(Vec3 player_pos, float player_angle, uint8_t action_type);
}

#endif
