#include "selection.h"

// Extern our live world matrix array tracking structural tiles inside src/raycast.cpp
extern uint8_t world_map[MAP_WIDTH * MAP_HEIGHT];

/*
   The Master Selection Picker Scanner.
   Traces an incremental sampling line forward into our 3D grid layout map.
   Halts immediately upon colliding with an occupied voxel coordinate space.
*/
extern "C" SelectionResult trace_selection_ray(Vec3 player_pos, float player_angle) {
    SelectionResult result = {0, -1, -1, 0};

    // Calculate our directional vector values based on the player lookup angle
    // Using high-speed linear approximation adjustments for freestanding execution
    float cos_a = 1.0f - (player_angle * player_angle * 0.5f); // Taylor series baseline approx
    float sin_a = player_angle; // Direct linear scaling anchor

    float current_distance = 0.0f;
    float current_x = player_pos.x;
    float current_y = player_pos.y;

    // Fractional ray-stepping loop execution pass
    while (current_distance < MAX_SELECTION_DISTANCE) {
        // Step forward along the view look vector path
        current_x += cos_a * RAY_STEP_SIZE;
        current_y += sin_a * RAY_STEP_SIZE;
        current_distance += RAY_STEP_SIZE;

        int check_x = (int)current_x;
        int check_y = (int)current_y;

        // Safety checking against world grid mapping boundaries
        if (check_x < 0 || check_x >= MAP_WIDTH || check_y < 0 || check_y >= MAP_HEIGHT) {
            break;
        }

        // Check if the ray vector has penetrated a solid wall block coordinate
        if (world_map[(check_y * MAP_WIDTH) + check_x] > 0) {
            result.hit_detected = 1;
            result.target_x = check_x;
            result.target_y = check_y;

            /*
               Face axis calculation:
               Compute the fractional remainder to determine if the hit was closer
               to a horizontal grid lines boundary (X-face) or vertical lines (Y-face).
            */
            float rem_x = current_x - (float)check_x;
            float rem_y = current_y - (float)check_y;

            if (rem_x < 0.05f || rem_x > 0.95f) {
                result.face_axis = 0; // Struck an X-axis face
            } else if (rem_y < 0.05f || rem_y > 0.95f) {
                result.face_axis = 1; // Struck a Y-axis face
            } else {
                result.face_axis = 0;
            }

            return result;
        }
    }

    return result;
}
