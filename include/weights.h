#ifndef WEIGHTS_H
#define WEIGHTS_H

#include <stdint.h>
#include "tensor.h"

/*
   Model Weights Disk Geometry Constraints.
   Defines the hard disk layout mapping for model parameter tracking blocks.
   We map our weights binary data to start exactly at virtual sector LBA 20000
   to completely insulate model parameters from operating system kernel sectors.
*/
#define MODEL_WEIGHTS_LBA_START    20000
#define SECTOR_SIZE_BYTES          512

// 256 Vocab * 64 Dimension * 4 Bytes per float = 65,536 Bytes Total
#define WEIGHTS_TOTAL_BYTES        (FONTANA_VOCAB_SIZE * FONTANA_EMBED_DIM * 4)
#define WEIGHTS_REQUIRED_SECTORS   (WEIGHTS_TOTAL_BYTES / SECTOR_SIZE_BYTES)

/* Expose our raw weights serialization mapping routines cleanly to C linkage maps */
extern "C" {
    void init_model_weights_buffer();
    uint8_t load_weights_from_disk();
    const float* get_loaded_weights_reference();
}

#endif
