#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include "buffer.h"
#include "palette.h"

/*
   Bitmap Font Geometry Constraints.
   Defines a standardized 8x8 pixel character resolution box layout matrix footprint.
*/
#define FONT_WIDTH     8
#define FONT_HEIGHT    8

/* Expose our raw graphical terminal print routines cleanly to C linkage maps */
extern "C" {
    void draw_char_gfx(uint32_t x, uint32_t y, char c, uint32_t color);
    void draw_string_gfx(uint32_t x, uint32_t y, const char* str, uint32_t color);
}

#endif
