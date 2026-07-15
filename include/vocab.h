#ifndef VOCAB_H
#define VOCAB_H

#include <stdint.h>
#include "tensor.h"

/*
   The Maximum Word Token String Boundary Limit.
   Restricts the length of an individual string token translation payload
   to 16 bytes to guarantee a perfectly predictable layout grid footprint inside RAM.
*/
#define MAX_TOKEN_STR_LEN    16

struct VocabEntry {
    uint32_t token_id;
    char token_string[MAX_TOKEN_STR_LEN];
};

/* Expose our raw vocabulary translation routines cleanly to C linkage maps */
extern "C" {
    void init_vocab_parser();
    void add_vocab_mapping(uint32_t token_id, const char* str);
    const char* lookup_token_string(uint32_t token_id);
}

#endif
