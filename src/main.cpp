#include <stdint.h>
#include "vbe.h"
#include "buffer.h"
#include "palette.h"

/*
   Antagonist OS Distribution Master Initialization Gateway.
   Manages the hardware clock overclock and initial graphics context handshakes.
*/

// Low-level port input assembly wrapper
inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/*
   The Host Interrupt Clock Overclock Pass (Phase F).
   Accelerates the Programmable Interval Timer (PIT) frequency to exactly 1000Hz.
   Divisor Calculation: 1193182 Hz (Internal Oscillator) / 1000 Target Hz = 1193.
*/
static void init_pit_overclock() {
    uint16_t divisor = 1193;

    // Send Command Byte 0x36 to Port 0x43: Select Channel 0, Square Wave Mode, Load LSB then MSB
    outb(0x43, 0x36);

    // Send the Low 8 bits of the divisor payload to Channel 0 Data Port 0x40
    outb(0x40, (uint8_t)(divisor & 0xFF));

    // Send the High 8 bits of the divisor payload to Channel 0 Data Port 0x40
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

// External hardware states living within our adjacent FontaineOS kernel
extern "C" {
    void init_keyboard();
}

/*
   Master Distro Boot Loop.
   Executed immediately after the micro-kernel initializes the GDT, IDT, and paging segments.
*/
extern "C" void antagonist_main() {
    // 1. Forcefully accelerate system clock interrupt ticks to 1000Hz (1ms intervals)
    init_pit_overclock();

    // 2. Wake up the high-resolution VESA VBE 800x600 32-bit Linear Framebuffer
    init_vbe_graphics();

    // 3. Flood our staging back-buffer with our signature corporate dark slate tone
    clear_back_buffer(COLOR_DARK_SLATE);

    // 4. Draw a single high-density Gold tracking line across the top border of our canvas
    for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
        put_pixel_buffer(x, 10, COLOR_GOLD);
    }

    // 5. Blast the rendered staging canvas straight onto the physical display panel
    swap_graphics_buffers();

    // 6. Enter our infinite standby execution trap line
    while (true) {
        asm volatile("hlt");
    }
}
