#ifndef TENSOR_H
#define TENSOR_H

#include <stdint.h>

/*
   Fontana Micro-Brain Structural Constants.
   Defines the flat vocabulary array width boundary and token context window limits
   for bare-metal execution passes.
*/
#define FONTANA_VOCAB_SIZE       256
#define FONTANA_CONTEXT_LEN      32
#define FONTANA_EMBED_DIM        64

/* Freestanding Multi-Dimensional Weights Matrix Matrix Structure */
struct TensorMatrix {
    uint32_t rows;
    uint32_t cols;
    float* weights;  // Reference pointer to a flat array memory address space
};

/* Expose our raw neural tensor backend routines cleanly to C linkage maps */
extern "C" {
    void init_tensor_engine();
    void evaluate_tensor_forward(const uint32_t* input_tokens, float* output_probabilities);
    float compute_vector_dot(const float* vec_a, const float* vec_b, uint32_t length);
}

#endif
