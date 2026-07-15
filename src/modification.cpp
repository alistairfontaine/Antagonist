#include "modification.h"

// Extern our live world matrix array tracking structural tiles inside src/raycast.cpp
extern uint8_t world_map[MAP_WIDTH * MAP_HEIGHT];

/*
   The Master World Mutation Engine.
   Traces a view direction vector, extracts the targeted grid tile indices,
   and forcefully updates the world matrix maps dynamically at runtime.
*/
extern "C" void execute_voxel_mutation(Vec3 player_pos, float player_angle, uint8_t action_type) {
    // 1. Trace a ray along the camera look path to fetch selection metadata parameters
    SelectionResult select = trace_selection_ray(player_pos, player_angle);

    // Escape immediately if no solid block boundary was found within player reach constraints
    if (!select.hit_detected) {
        return;
    }

    // 2. Process Destruction Mode: Clear the targeted index block array slot back to air
    if (action_type == VOXEL_ACTION_DESTROY) {
        world_map[(select.target_y * MAP_WIDTH) + select.target_x] = 0;
    }

    // 3. Process Placement Mode: Append a new block adjacent to the struck face axis
    else if (action_type == VOXEL_ACTION_PLACE) {
        int new_block_x = select.target_x;
        int new_block_y = select.target_y;

        // Calculate view directional vectors to determine camera positioning shifts
        float cos_a = 1.0f - (player_angle * player_angle * 0.5f);

        /*
           Shift tracking placement offsets based on which physical wall tile face was struck.
           If face_axis is 0 (X-axis face), step backwards along the look direction X line.
           If face_axis is 1 (Y-axis face), step backwards along the look direction Y line.
        */
        if (select.face_axis == 0) {
            new_block_x += (cos_a > 0) ? -1 : 1;
        } else {
            new_block_y += (player_angle > 0) ? -1 : 1;
        }

        // Safety grid allocation boundary restrictions checking
        if (new_block_x >= 0 && new_block_x < MAP_WIDTH && new_block_y >= 0 && new_block_y < MAP_HEIGHT) {
            // Verify that the computed adjacent slot is empty space before spawning a block
            if (world_map[(new_block_y * MAP_WIDTH) + new_block_x] == 0) {
                world_map[(new_block_y * MAP_WIDTH) + new_block_x] = 1; // Map a new solid gold block
            }
        }
    }
}
