#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include "vbe.h"

/*
   Total Screen Pixel Count.
   800 Width * 600 Height = 480,000 distinct true-colour pixel coordinate slots.
*/
#define TOTAL_SCREEN_PIXELS    (SCREEN_WIDTH * SCREEN_HEIGHT)

/* Expose our secondary double-buffering pipelines wrapped in C-linkage maps */
extern "C" {
    void clear_back_buffer(uint32_t color);
    void put_pixel_buffer(uint32_t x, uint32_t y, uint32_t color);
    void swap_graphics_buffers();
}

#endif
