#include "tokenizer.h"

/*
   The Master Distribution Tokenizer Context Engine Tracker.
   Allocates a flat global state container inside our static data segment area.
   Maintains the active rolling memory history window for the cognitive matrix operations.
*/
static TokenizerState global_tokenizer __attribute__((aligned(4)));

/*
   Raw bare-metal character string comparison helper.
*/
static uint8_t token_strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0') ? 1 : 0;
}

/*
   Initializes the cognitive tokenizer state buffers at boot time.
*/
extern "C" void init_tokenizer_routing() {
    for (int i = 0; i < FONTANA_CONTEXT_LEN; i++) {
        global_tokenizer.context_history[i] = 0; // Populate context slots with baseline tokens
    }
    global_tokenizer.current_index = 0;
    global_tokenizer.total_tokens_processed = 0;
}

/*
   Sifts through raw text slices and maps them to a numeric vocabulary identity code.
   Utilizes your active Phase Q lookup tables to evaluate entries in constant time.
*/
extern "C" uint32_t tokenize_string_slice(const char* word_str) {
    if (!word_str || word_str[0] == '\0') {
        return 0; // Default padding token index
    }

    // Step through the active vocabulary records to find a textual match
    for (uint32_t i = 0; i < FONTANA_VOCAB_SIZE; i++) {
        const char* vocab_check = lookup_token_string(i);
        if (token_strcmp(word_str, vocab_check) == 1) {
            return i; // Struck a valid registered token mapping match!
        }
    }

    return 255; // Out-of-vocabulary fallback character code assignment marker
}

/*
   Pushes an integer token ID directly into the sliding history context window.
   Shifts ancient context historical blocks backward if memory tracking fills up.
*/
extern "C" void push_token_to_context(uint32_t token_id) {
    if (global_tokenizer.current_index < FONTANA_CONTEXT_LEN) {
        // Window has empty slots: insert data directly into the sequential array trace line
        global_tokenizer.context_history[global_tokenizer.current_index] = token_id;
        global_tokenizer.current_index++;
    } else {
        /*
           Context window boundary is completely full:
           Execute a fast array shift operation to slide history tokens left by exactly 1 slot,
           dropping the oldest token parameter index right off the edge!
        */
        for (int i = 0; i < FONTANA_CONTEXT_LEN - 1; i++) {
            global_tokenizer.context_history[i] = global_tokenizer.context_history[i + 1];
        }

        // Append our brand-new token index tracking integer straight to the final array slot
        global_tokenizer.context_history[FONTANA_CONTEXT_LEN - 1] = token_id;
    }

    global_tokenizer.total_tokens_processed++;
}

/*
   Exposes a secure pointer tracking the active context array matrix frame to the cognitive loop.
*/
extern "C" const uint32_t* get_active_context_reference() {
    return global_tokenizer.context_history;
}
