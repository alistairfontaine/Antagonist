#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

/*
   Total Scancode Map Limit.
   The standard IBM PC/AT PS/2 keyboard interface tracks up to 256 distinct
   make/break keyboard hardware scancode signals.
*/
#define KEYBOARD_MATRIX_SIZE    256

/* Custom True/False Boolean System Flags for Freestanding Space */
#define KEY_RELEASED            0
#define KEY_PRESSED             1

/* Standard Voxel Navigation Scancode Map Indices */
#define SCANCODE_W              0x11
#define SCANCODE_A              0x1E
#define SCANCODE_S              0x1F
#define SCANCODE_D              0x20
#define SCANCODE_SPACE          0x39
#define SCANCODE_ESC            0x01

/* Expose our expanded input tracking routines cleanly to C linkage maps */
extern "C" {
    void init_input_matrix();
    void set_key_state(uint8_t scancode, uint8_t state);
    uint8_t is_key_pressed(uint8_t scancode);
}

#endif
