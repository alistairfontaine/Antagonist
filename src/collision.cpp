#include "collision.h"

// Extern the live world matrix array mapping tracker sitting inside src/raycast.cpp
extern uint8_t world_map[MAP_WIDTH * MAP_HEIGHT];

/*
   Direct Tile Evaluator.
   Translates a raw fractional coordinate check into an absolute world tile index slot.
   Returns 1 if the target coordinate intercepts a solid voxel block boundary.
*/
extern "C" uint8_t check_wall_collision(float check_x, float check_y) {
    int map_x = (int)check_x;
    int map_y = (int)check_y;

    // Safety boundary constraints checking
    if (map_x < 0 || map_x >= MAP_WIDTH || map_y < 0 || map_y >= MAP_HEIGHT) {
        return 1; // Treat out-of-bounds space as a solid wall clip
    }

    return (world_map[(map_y * MAP_WIDTH) + map_x] > 0) ? 1 : 0;
}

/*
   The Master Axis-Aligned Bounding Box (AABB) Sliding Engine.
   Evaluates updates for the X and Y paths independently, allowing the camera
   to slide fluidly along tile faces during diagonal velocity vector shifts.
*/
extern "C" Vec3 apply_aabb_clipping(Vec3 current_pos, Vec3 movement_delta) {
    Vec3 final_pos = current_pos;

    // 1. Calculate and isolate the prospective update target along the X axis
    float target_x = current_pos.x + movement_delta.x;

    // Check both left and right tracking box boundaries against map structures
    if (movement_delta.x > 0) {
        if (!check_wall_collision(target_x + PLAYER_RADIUS, current_pos.y)) {
            final_pos.x = target_x;
        }
    } else if (movement_delta.x < 0) {
        if (!check_wall_collision(target_x - PLAYER_RADIUS, current_pos.y)) {
            final_pos.x = target_x;
        }
    }

    // 2. Calculate and isolate the prospective update target along the Y axis
    float target_y = current_pos.y + movement_delta.y;

    // Check both top and bottom tracking box boundaries against map structures
    if (movement_delta.y > 0) {
        if (!check_wall_collision(final_pos.x, target_y + PLAYER_RADIUS)) {
            final_pos.y = target_y;
        }
    } else if (movement_delta.y < 0) {
        if (!check_wall_collision(final_pos.x, target_y - PLAYER_RADIUS)) {
            final_pos.y = target_y;
        }
    }

    // Preserve the flat height level layer unchanged
    final_pos.z = current_pos.z;

    return final_pos;
}
