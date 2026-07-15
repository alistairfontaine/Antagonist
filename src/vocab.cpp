#include "vocab.h"

/*
   The Master Token Translation Database Table.
   Allocates exactly 256 structural vocabulary data slots inside RAM.
   Enforces a strict 4-byte tracking alignment to guarantee rapid string indexing lookup.
*/
static VocabEntry vocab_table[FONTANA_VOCAB_SIZE] __attribute__((aligned(4)));

/*
   Freestanding Byte-Copy Utility.
   Replicates raw character arrays safely into destination memory blocks
   without standard <cstring> dependency links.
*/
static void vocab_strcpy(char* dest, const char* src, uint32_t max_len) {
    uint32_t i = 0;
    while (src[i] != '\0' && i < (max_len - 1)) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; // Enforce strict null-termination boundaries
}

/*
   Initializes the base character translation structures at boot time.
*/
extern "C" void init_vocab_parser() {
    for (int i = 0; i < FONTANA_VOCAB_SIZE; i++) {
        vocab_table[i].token_id = (uint32_t)i;
        vocab_table[i].token_string[0] = '\0'; // Clear array references to empty null markers
    }
}

/*
   Registers a custom character string map directly onto a target token numeric index.
*/
extern "C" void add_vocab_mapping(uint32_t token_id, const char* str) {
    // Safety boundary constraints checking
    if (token_id >= FONTANA_VOCAB_SIZE || !str) {
        return;
    }
    vocab_strcpy(vocab_table[token_id].token_string, str, MAX_TOKEN_STR_LEN);
}

/*
   Constant-time O(1) Token String Evaluator.
   Queries our vocabulary database table instantly to retrieve raw word values.
*/
extern "C" const char* lookup_token_string(uint32_t token_id) {
    if (token_id >= FONTANA_VOCAB_SIZE) {
        return "??"; // Fallback identifier marker for out-of-bounds tokens
    }
    return vocab_table[token_id].token_string;
}
