#ifndef SPATIAL_H
#define SPATIAL_H

#include <stdint.h>
#include "math3d.h"
#include "raycast.h"

/*
   Spatial Grid Scaling Constraints.
   Defines the sizing boundary for our macro room cells.
   With a 16x16 world, integer dividing by 4 splits our grid into a clean 4x4 matrix of sectors.
*/
#define CELL_SCALE            4
#define SECTOR_GRID_LIMIT     4

/* Structure mapping out a discrete spatial room sector bounding box anchor */
struct SpatialCell {
    uint8_t sector_id;
    uint8_t total_active_objects;
    uint32_t cell_color_mask;
};

/* Expose our raw broad-phase spatial lookup loops cleanly to C linkage maps */
extern "C" {
    void init_spatial_grid();
    uint32_t calculate_cell_index(Vec3 position);
    SpatialCell get_cell_metrics(Vec3 position);
}

#endif
