#include "weights.h"

/*
   The Master Cognitive Model Parameter Landing Pad.
   Allocates a flat 16,384 floating-point array (exactly 65,536 bytes) inside RAM.
   Enforces a 4-byte structural alignment to guarantee direct register streaming via the IDE bus.
*/
static float model_weights_matrix[FONTANA_VOCAB_SIZE * FONTANA_EMBED_DIM] __attribute__((aligned(4)));

// External ATA hardware driver hooks living within your adjacent FontaineOS kernel
extern "C" {
    void ata_read_sector(uint32_t lba, uint8_t* buffer);
}

/*
   Clears out our local weights allocation grid at system startup.
*/
extern "C" void init_model_weights_buffer() {
    for (uint32_t i = 0; i < (FONTANA_VOCAB_SIZE * FONTANA_EMBED_DIM); i++) {
        model_weights_matrix[i] = 0.0f;
    }
}

/*
   The High-Integrity Multi-Sector Storage Streamer.
   Loops through the hard disk configuration matrix and streams your neural parameters
   straight off LBA sector blocks into our flat system memory pad.
*/
extern "C" uint8_t load_weights_from_disk() {
    // Cast our float landing pad into a raw byte pointer for our ATA driver block streaming loops
    uint8_t* byte_dest = (uint8_t*)model_weights_matrix;
    uint32_t current_lba = MODEL_WEIGHTS_LBA_START;

    for (uint32_t s = 0; s < WEIGHTS_REQUIRED_SECTORS; s++) {
        /*
           Call your kernel's underlying hardware driver directly!
           Streams 512 bytes off the motherboard bus straight into the active memory segment slot.
        */
        ata_read_sector(current_lba, byte_dest);

        // Advance our byte offset tracker and sector targets
        byte_dest += SECTOR_SIZE_BYTES;
        current_lba++;
    }

    return 1; // Return true to signify successful hardware parameters synchronization
}

/*
   Exposes a constant reference pointer of our model weights matrix grid to the tensor engine.
*/
extern "C" const float* get_loaded_weights_reference() {
    return model_weights_matrix;
}
