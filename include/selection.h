#ifndef SELECTION_H
#define SELECTION_H

#include <stdint.h>
#include "math3d.h"
#include "raycast.h"

/*
   The Physical Reaching Constraint of the Player.
   Limits the raycast picker line from checking past 4.0 units to prevent selecting
   infinite blocks across the map matrix.
*/
#define MAX_SELECTION_DISTANCE    4.0f
#define RAY_STEP_SIZE             0.05f

/* Structure tracking the absolute data coordinates of a hit event */
struct SelectionResult {
    uint8_t hit_detected;
    int target_x;
    int target_y;
    int face_axis;  // 0 = X-axis face, 1 = Y-axis face struck
};

/* Expose our raw interactive raycast picking routines cleanly to C linkage maps */
extern "C" {
    SelectionResult trace_selection_ray(Vec3 player_pos, float player_angle);
}

#endif
