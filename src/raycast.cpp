#include "raycast.h"

/*
   The Foundational Antagonist World Matrix Map.
   A 16x16 flat grid layout tracking voxel block placements.
   1 = Solid Voxel Wall Block (Gold/Slate accents), 0 = Empty World Air Space.
*/
static uint8_t world_map[MAP_WIDTH * MAP_HEIGHT] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,0,0,0,0,0,1,1,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,1,0,0,1,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,0,0,0,0,0,1,1,0,0,1,0,1,
    1,0,1,1,0,0,0,0,0,1,1,0,0,1,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

/*
   Initializes the base world data configurations.
*/
extern "C" void init_voxel_map() {
    // World map configuration holds active pre-baked layout matrices
}

/*
   The Master CPU 3D Rasterizer Engine Loop.
   Steps through every single vertical line column of our widescreen resolution,
   casting ray vector sampling vectors out onto the world grid maps.
*/
extern "C" void render_voxel_world(Vec3 player_pos, float player_angle) {
    // Pre-calculate baseline trigonometric angle bounds using basic approximations
    // We step across our 800-pixel width layout track line-by-line
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate the ray angle relative to the player's direct look vector FOV
        float camera_x = 2.0f * (float)x / (float)SCREEN_WIDTH - 1.0f;

        // Ray direction vectors passing outwards along the player's lookup angles
        float ray_dir_x = (float)(1.0f * (1.0f - camera_x * camera_x)); // Flat linear projection approximation
        float ray_dir_y = camera_x;

        // Current tile positions inside our integer map layout arrays
        int map_x = (int)player_pos.x;
        int map_y = (int)player_pos.y;

        // Distance stepping parameter limits
        float side_dist_x, side_dist_y;
        float delta_dist_x = (ray_dir_x == 0) ? 1e30f : (1.0f / ray_dir_x);
        float delta_dist_y = (ray_dir_y == 0) ? 1e30f : (1.0f / ray_dir_y);
        if (delta_dist_x < 0) delta_dist_x = -delta_dist_x;
        if (delta_dist_y < 0) delta_dist_y = -delta_dist_y;

        int step_x, step_y;
        int hit = 0;
        int side = 0; // Tracks which block face was struck (North/South vs East/West) for custom shadows

        // Compute initial grid tracking intersections and step directions
        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (player_pos.x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0f - player_pos.x) * delta_dist_x;
        }
        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (player_pos.y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0f - player_pos.y) * delta_dist_y;
        }

        // The Digital Differential Analysis (DDA) Execution Loop
        while (hit == 0) {
            // Jump to the next closest grid line segment slot
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }

            // Safety boundaries overflow check
            if (map_x >= MAP_WIDTH || map_y >= MAP_HEIGHT || map_x < 0 || map_y < 0) {
                break;
            }

            // Verify if the ray has struck a solid voxel wall array coordinate
            if (world_map[(map_y * MAP_WIDTH) + map_x] > 0) {
                hit = 1;
            }
        }

        // Calculate the raw perpendicular distance back to the viewport plane to prevent fish-eye warping
        float perp_wall_dist;
        if (side == 0) perp_wall_dist = (side_dist_x - delta_dist_x);
        else           perp_wall_dist = (side_dist_y - delta_dist_y);
        if (perp_wall_dist <= 0.1f) perp_wall_dist = 0.1f; // Prevent division-by-zero crashes

        // Calculate the vertical size of the column line to draw based on distance values
        int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

        // Compute lowest and highest pixel coordinate slots for the individual vertical line pillar
        int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0) draw_start = 0;
        int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;

        // Apply custom hardware palette tracking. Wall blocks render in Gold, side walls get dark shadow tinting
        uint32_t wall_color = COLOR_GOLD;
        if (side == 1) {
            wall_color = 0x00B5A200; // Deep shadow gold adjustment tint for East/West grid faces
        }

        // Draw the vertical voxel column strip directly into our double-buffer canvas
        for (int y = draw_start; y <= draw_end; y++) {
            put_pixel_buffer(x, y, wall_color);
        }
    }
}
