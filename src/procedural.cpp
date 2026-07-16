#include "procedural.h"

/*
   Deterministic 2D Bit-Scrambler Noise Core.
   Utilizes highly optimized integer primes to generate pseudo-random
   numbers completely within standard CPU registers without floating-point math.
*/
extern "C" uint32_t calculate_integer_noise(int32_t x, int32_t y) {
    int32_t n = x + y * NOISE_PRIME_A;
    n = (n << 13) ^ n;

    // Scramble values deterministically via high-order primes
    int32_t nn = (n * (n * n * NOISE_PRIME_A + NOISE_PRIME_B) + NOISE_PRIME_C) & 0x7FFFFFFF;
    return (uint32_t)nn;
}

/*
   Monolithic Row-Major Landscape Matrix Evaluator.
   Generates an 80x5 text terrain view grid by shifting coordinate windows based
   on running system clock ticks to simulate fluid procedural travel map frames.
*/
extern "C" void generate_procedural_topography(uint32_t ticks, char* output_matrix) {
    // We target a small 5-row section (rows 18 to 22) for our visual topology grid
    for (int32_t y = 0; y < 5; y++) {
        for (int32_t x = 0; x < 80; x++) {
            int32_t linear_index = (y * 80) + x;

            // Shift our horizontal noise sampling coordinate based on running telemetry clocks
            int32_t sample_x = x + (int32_t)(ticks / 10);
            int32_t sample_y = y;

            // Generate raw pseudo-random height values
            uint32_t raw_noise = calculate_integer_noise(sample_x, sample_y);
            uint32_t height_tier = raw_noise % 4; // Map down to 4 distinct terrain zones

            // Assign structural ASCII topology characters to represent our material layers
            if (height_tier == 0) {
                output_matrix[linear_index] = '#'; // Solid Voxel Peak
            } else if (height_tier == 1) {
                output_matrix[linear_index] = '*'; // Elevated Ridge Track
            } else if (height_tier == 2) {
                output_matrix[linear_index] = '.'; // Flat Sand Valley
            } else {
                output_matrix[linear_index] = ' '; // Open Clear Workspace
            }
        }
    }
}
