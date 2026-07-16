#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include <stdint.h>

/*
   Procedural Generation Constraints.
   Defines a standardized grid tracking envelope for calculating pseudo-random noise steps
   completely within localized stack matrices without standard floating-point headers.
*/
#define NOISE_PRIME_A    15731
#define NOISE_PRIME_B    789221
#define NOISE_PRIME_C    1376312589

/* Expose our raw procedural terrain tracking loops cleanly to C linkage maps */
extern "C" {
    uint32_t calculate_integer_noise(int32_t x, int32_t y);
    void generate_procedural_topography(uint32_t ticks, char* output_matrix);
}

#endif
