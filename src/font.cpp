#include "font.h"

/*
   The Master 8x8 Bitmap Font Index Table.
   Every character is structurally mapped down to exactly 8 bytes of data,
   where every individual bit represents a physical screen pixel pixel color mask flag.
   Contains baseline templates for 'A', 'N', 'T', 'G', 'O', 'S', ':', 'M', 'E', 'U', 'I', and ' '.
*/
static const uint8_t font_bitmap[12][8] = {
    {0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x00}, // 0: 'A'
    {0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x42, 0x00}, // 1: 'N'
    {0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, // 2: 'T'
    {0x3C, 0x42, 0x40, 0x4E, 0x42, 0x42, 0x3C, 0x00}, // 3: 'G'
    {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00}, // 4: 'O'
    {0x3C, 0x42, 0x40, 0x3C, 0x02, 0x42, 0x3C, 0x00}, // 5: 'S'
    {0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00}, // 6: ':'
    {0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x42, 0x00}, // 7: 'M'
    {0x7E, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7E, 0x00}, // 8: 'E'
    {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00}, // 9: 'U'
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, // 10: 'I'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 11: ' ' (Space)
};

/*
   Resolves an ASCII character back to its index slot inside our font bitmap grid array.
*/
static int get_font_index(char c) {
    switch (c) {
        case 'A': return 0;
        case 'N': return 1;
        case 'T': return 2;
        case 'G': return 3;
        case 'O': return 4;
        case 'S': return 5;
        case ':': return 6;
        case 'M': return 7;
        case 'E': return 8;
        case 'U': return 9;
        case 'I': return 10;
        default:  return 11; // Default to space spacer value if char is missing
    }
}

/*
   Draws an individual 8x8 bitmapped character onto our staging back-buffer layout array.
   Steps through every single bit row and shifts bits rightwards to compute masks.
*/
extern "C" void draw_char_gfx(uint32_t x, uint32_t y, char c, uint32_t color) {
    int font_idx = get_font_index(c);

    // Scan every row of our 8-pixel tall font box template matrix
    for (int row = 0; row < FONT_HEIGHT; row++) {
        uint8_t row_byte = font_bitmap[font_idx][row];

        // Sweep across every individual bit within the row byte tracking flag
        for (int col = 0; col < FONT_WIDTH; col++) {
            /*
               Test if the bit at column offset position is hot (active tracking text stroke).
               Checks the MSB (Most Significant Bit, value 0x80) and shifts down continuously.
            */
            if (row_byte & (0x80 >> col)) {
                put_pixel_buffer(x + col, y + row, color);
            }
        }
    }
}

/*
   Loops over raw null-terminated character strings and advances cursor offsets.
*/
extern "C" void draw_string_gfx(uint32_t x, uint32_t y, const char* str, uint32_t color) {
    if (!str) {
        return;
    }

    int current_x_offset = 0;
    int i = 0;

    while (str[i] != '\0') {
        // Render individual character glyphs cleanly
        draw_char_gfx(x + current_x_offset, y, str[i], color);

        // Advance our horizontal tracking layout cursor width boundary by exactly 8 pixels
        current_x_offset += FONT_WIDTH;
        i++;
    }
}
