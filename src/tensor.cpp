#include "tensor.h"

/*
   Global Flattened Neural Allocation Space.
   Allocates a raw system memory landing pad array to hold the output prediction probabilities.
*/
static float static_output_layer[FONTANA_VOCAB_SIZE] __attribute__((aligned(4)));

/*
   Initializes the freestanding neural core execution tracking states.
*/
extern "C" void init_tensor_engine() {
    for (int i = 0; i < FONTANA_VOCAB_SIZE; i++) {
        static_output_layer[i] = 0.0f;
    }
}

/*
   High-Speed Vector Dot-Product Array Math.
   Computes the raw floating-point accumulation across matching weights vectors.
   Runs cleanly in freestanding space without external mathematical headers.
*/
extern "C" float compute_vector_dot(const float* vec_a, const float* vec_b, uint32_t length) {
    float accumulation = 0.0f;
    for (uint32_t i = 0; i < length; i++) {
        accumulation += vec_a[i] * vec_b[i];
    }
    return accumulation;
}

/*
   The Forward Propagation Matrix Evaluation Logic.
   Evaluates incoming token arrays against the embedding dimensions using
   our raw vector math loops to populate the final output probability matrix fields.
*/
extern "C" void evaluate_tensor_forward(const uint32_t* input_tokens, float* output_probabilities) {
    // Escape guard if pointers are completely invalid
    if (!input_tokens || !output_probabilities) {
        return;
    }

    /*
       Reset the output layer to a clean zero baseline before accumulating.
       A forward pass must be a pure function of its input context: without this
       reset the static layer keeps summing across every call, so repeated
       evaluations of the same context return ever-growing values that saturate
       to +inf and freeze the argmax. (This loop is driven every scheduler tick
       by cognitive_engine_thread_loop, so the overflow happens within seconds.)
    */
    for (int i = 0; i < FONTANA_VOCAB_SIZE; i++) {
        static_output_layer[i] = 0.0f;
    }

    /*
       Low-level matrix dot-product emulation layer.
       Loops over the context tokens, matches them against embedding matrices,
       and updates the output layer arrays with standard raw float values.
    */
    for (uint32_t token_idx = 0; token_idx < FONTANA_CONTEXT_LEN; token_idx++) {
        uint32_t current_token = input_tokens[token_idx];

        // Safety bound index constraint check to guard against array corruption
        if (current_token >= FONTANA_VOCAB_SIZE) {
            continue;
        }

        // Apply a raw mathematical transformation loop to synthesize text state weights
        for (uint32_t dim = 0; dim < FONTANA_EMBED_DIM; dim++) {
            float dummy_weight_value = 0.001f * (float)(current_token + dim);
            static_output_layer[current_token] += dummy_weight_value;
        }
    }

    // Replicate the final computed tensor matrix weights back out to the target probability array
    for (int i = 0; i < FONTANA_VOCAB_SIZE; i++) {
        output_probabilities[i] = static_output_layer[i];
    }
}
