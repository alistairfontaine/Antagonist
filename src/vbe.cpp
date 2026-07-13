#include "vbe.h"

/*
   Low-level hardware bus communications.
   Outputs a 16-bit word value directly to a specified machine register port.
*/
inline void outw(uint16_t port, uint16_t value) {
    asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

/*
   BGA Register Port Writer Helper.
   Selects a specific controller register line index and flashes the data payload onto it.
*/
static void vbe_write_register(uint16_t index, uint16_t value) {
    outw(VBE_DISPI_IOPORT_INDEX, index);
    outw(VBE_DISPI_IOPORT_DATA, value);
}

/*
   Direct Linear Framebuffer Reference Pointer.
   Points straight to the hardware pixel color allocation segment array in RAM.
*/
static uint32_t* lfb_buffer = (uint32_t*)VBE_LFB_PHYSICAL_ADDRESS;

/*
   Initializes the VESA VBE Hardware Graphics Adapter Subsystem.
   Gracefully shuts down text-mode matrices and locks the GPU into high-resolution graphics space.
*/
extern "C" void init_vbe_graphics() {
    // 1. Forcefully disable the graphics adapter to safe-lock the register configurations
    vbe_write_register(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);

    // 2. Inject our custom widescreen canvas resolution width bounds
    vbe_write_register(VBE_DISPI_INDEX_XRES, SCREEN_WIDTH);

    // 3. Inject our custom canvas resolution height bounds
    vbe_write_register(VBE_DISPI_INDEX_YRES, SCREEN_HEIGHT);

    // 4. Set our data density to 32 bits per pixel (4 bytes per single pixel)
    vbe_write_register(VBE_DISPI_INDEX_BPP, SCREEN_BPP);

    /*
       5. Re-enable the hardware device and forcefully activate the
          Linear Framebuffer layer mapping tracks simultaneously!
    */
    vbe_write_register(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);
}

/*
   Blasts a single pixel color block directly onto the raw physical LFB space.
   Calculates coordinates via flat row-major matrix projection: Index = Y * Width + X.
*/
extern "C" void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    // Boundary check safety guard: ignore coordinates that spill past our screen constraints
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return;
    }

    // Direct physical RAM address translation plot execution
    lfb_buffer[(y * SCREEN_WIDTH) + x] = color;
}
