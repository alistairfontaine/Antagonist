#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdint.h>
#include "math3d.h"
#include "buffer.h"
#include "palette.h"

/*
   Virtual World Voxel Matrix Grids.
   Allocates a 16x16 horizontal map grid layout path for voxel blocks.
*/
#define MAP_WIDTH       16
#define MAP_HEIGHT      16

/* Expose our raw 3D software rendering pipelines wrapped in C-linkage maps */
extern "C" {
    void init_voxel_map();
    void render_voxel_world(Vec3 player_pos, float player_angle);
}

#endif
