#ifndef VBE_H
#define VBE_H

#include <stdint.h>

/*
   Primary Bochs/VESA Graphic Adapter (BGA) I/O Hardware Index Ports.
   These ports sit directly on the motherboard system bus to control the video card.
*/
#define VBE_DISPI_IOPORT_INDEX          0x01CE
#define VBE_DISPI_IOPORT_DATA           0x01CF

/* BGA Register Selection Codes */
#define VBE_DISPI_INDEX_ID              0x0
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4

/* BGA Operational Command Flags */
#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_LFB_ENABLED           0x40  // Forces raw Linear Framebuffer mapping mode!

/* Custom Antagonist System Screen Dimensions */
#define SCREEN_WIDTH                    800
#define SCREEN_HEIGHT                   600
#define SCREEN_BPP                      32    // 32-bit true colour depth (RGBA)

/*
   The Physical LFB Memory Boundary.
   Under QEMU's PCI bus architecture emulation mapping layout, the hardware
   graphics adapter maps its flat pixel array buffer starting exactly at 0xFD000000.
*/
#define VBE_LFB_PHYSICAL_ADDRESS        0xFD000000

/* Expose our raw hardware port mapping driver hooks cleanly to C linkage maps */
extern "C" {
    void init_vbe_graphics();
    void put_pixel(uint32_t x, uint32_t y, uint32_t color);
}

#endif
