#ifndef COLLISION_H
#define COLLISION_H

#include <stdint.h>
#include "math3d.h"
#include "raycast.h"

/*
   The Physical Bounding Extent of our Camera Object.
   Allocates a strict geometric bounding box radius footprint (0.2f spatial units)
   around the camera center point to prevent clipping into wall corners.
*/
#define PLAYER_RADIUS    0.2f

/* Expose our raw physics bounding routines cleanly to C linkage maps */
extern "C" {
    uint8_t check_wall_collision(float check_x, float check_y);
    Vec3 apply_aabb_clipping(Vec3 current_pos, Vec3 movement_delta);
}

#endif
