#include "input.h"

/*
   The Master Keyboard State Matrix.
   Allocates 256 distinct tracking byte flags inside our flat data segment.
   Zeroed out at boot time; completely shielded from stack-frame collisions.
*/
static uint8_t keyboard_state[KEYBOARD_MATRIX_SIZE] __attribute__((aligned(4)));

/*
   Clears out our matrix mapping boundaries at system startup.
*/
extern "C" void init_input_matrix() {
    for (int i = 0; i < KEYBOARD_MATRIX_SIZE; i++) {
        keyboard_state[i] = KEY_RELEASED;
    }
}

/*
   Updates the physical state flag of an individual scancode index slot.
   Flipped asynchronously by the hardware keyboard interrupt handlers.
*/
extern "C" void set_key_state(uint8_t scancode, uint8_t state) {
    // Safety boundary restriction check
    if (scancode >= KEYBOARD_MATRIX_SIZE) {
        return;
    }
    keyboard_state[scancode] = state;
}

/*
   Constant-time O(1) state evaluator.
   Queries our matrix map instantly to check if a specific key combination is held down.
*/
extern "C" uint8_t is_key_pressed(uint8_t scancode) {
    if (scancode >= KEYBOARD_MATRIX_SIZE) {
        return KEY_RELEASED;
    }
    return keyboard_state[scancode];
}
