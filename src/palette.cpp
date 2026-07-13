#include "palette.h"

/*
   The Core Color Bit-Packer.
   Takes raw 8-bit Red, Green, and Blue variables and packs them into a single
   continuous 32-bit word integer layout via high-speed binary bit-shifting.
   Format Result Layout: 0x00RRGGBB
*/
extern "C" uint32_t create_rgb(uint8_t r, uint8_t g, uint8_t b) {
    /*
       Shift Red left by 16 bits, Green left by 8 bits, and layer Blue flat.
       Combines the bit streams using binary bitwise OR operators.
    */
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}
