#include "buffer.h"

/*
   The Master Staging Back-Buffer Canvas Array.
   Allocates exactly 480,000 continuous 32-bit pixel blocks inside RAM.
   Enforces a strict 4-byte alignment boundary to guarantee hyper-fast CPU tracking.
*/
static uint32_t back_buffer[TOTAL_SCREEN_PIXELS] __attribute__((aligned(4)));

/* Direct reference pointer to the physical Linear Framebuffer on the video card */
static uint32_t* real_vga_lfb = (uint32_t*)VBE_LFB_PHYSICAL_ADDRESS;

/*
   Clears the entire staging canvas array by flooding it with a single background color.
*/
extern "C" void clear_back_buffer(uint32_t color) {
    for (uint32_t i = 0; i < TOTAL_SCREEN_PIXELS; i++) {
        back_buffer[i] = color;
    }
}

/*
   Writes a pixel color code to the hidden staging array rather than the physical display.
   Calculates coordinates via flat row-major matrix projection: Index = Y * Width + X.
*/
extern "C" void put_pixel_buffer(uint32_t x, uint32_t y, uint32_t color) {
    // Boundary check safety guard: ignore coordinates that spill past our screen constraints
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return;
    }

    // Write to the staging back-buffer matrix
    back_buffer[(y * SCREEN_WIDTH) + x] = color;
}

/*
   The Master Video Synchronizer (Buffer Swap).
   Streams the entire 480,000 word frame out from system RAM straight onto the physical LFB.
*/
extern "C" void swap_graphics_buffers() {
    for (uint32_t i = 0; i < TOTAL_SCREEN_PIXELS; i++) {
        real_vga_lfb[i] = back_buffer[i];
    }
}
