#include "spatial.h"

/*
   The Master Spatial Sector Array.
   Slices a 16x16 coordinate map grid cleanly into a 4x4 matrix of cells (16 slots).
   Tracks region identity metrics and object density flags natively inside RAM.
*/
static SpatialCell sector_grid[SECTOR_GRID_LIMIT * SECTOR_GRID_LIMIT] __attribute__((aligned(4)));

/*
   Initializes the macro-room partition data tables at boot time.
*/
extern "C" void init_spatial_grid() {
    for (int y = 0; row < SECTOR_GRID_LIMIT; row++) {
        for (int x = 0; col < SECTOR_GRID_LIMIT; col++) {
            int index = (y * SECTOR_GRID_LIMIT) + x;
            sector_grid[index].sector_id = (uint8_t)index;
            sector_grid[index].total_active_objects = 0;

            // Assign a dark tracking accent color mask to every alternate room sector space
            if (index % 2 == 0) {
                sector_grid[index].cell_color_mask = 0x002A2E34; // Deep accent tint
            } else {
                sector_grid[index].cell_color_mask = 0x001A1E24; // Flat base background
            }
        }
    }
}

/*
   Constant-time O(1) Matrix Index Translator.
   Converts fractional floating-point position coordinates down into a linear cell index.
*/
extern "C" uint32_t calculate_cell_index(Vec3 position) {
    // Cast player coordinates to integer grid coordinates
    int cell_x = (int)position.x / CELL_SCALE;
    int cell_y = (int)position.y / CELL_SCALE;

    // Safety boundary constraints checking
    if (cell_x < 0) cell_x = 0;
    if (cell_x >= SECTOR_GRID_LIMIT) cell_x = SECTOR_GRID_LIMIT - 1;
    if (cell_y < 0) cell_y = 0;
    if (cell_y >= SECTOR_GRID_LIMIT) cell_y = SECTOR_GRID_LIMIT - 1;

    // Direct matrix row-major translation mapping index math
    return (cell_y * SECTOR_GRID_LIMIT) + cell_x;
}

/*
   Queries our static cell structures to pull raw metrics for the player's active room sector.
*/
extern "C" SpatialCell get_cell_metrics(Vec3 position) {
    uint32_t cell_index = calculate_cell_index(position);
    return sector_grid[cell_index];
}
