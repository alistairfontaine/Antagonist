#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdint.h>
#include "tensor.h"
#include "vocab.h"

/* Structure tracking the active sliding token history window */
struct TokenizerState {
    uint32_t context_history[FONTANA_CONTEXT_LEN];
    uint32_t current_index;
    uint32_t total_tokens_processed;
};

/* Expose our raw neural tokenizer routing loops cleanly to C linkage maps */
extern "C" {
    void init_tokenizer_routing();
    void push_token_to_context(uint32_t token_id);
    uint32_t tokenize_string_slice(const char* word_str);
    const uint32_t* get_active_context_reference();
}

#endif
