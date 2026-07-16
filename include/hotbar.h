#ifndef HOTBAR_H
#define HOTBAR_H

#include <stdint.h>

/*
   Hotbar Inventory Slots Constraints.
   Maps 4 distinct selectable material profiles into an index array matrix.
*/
#define HOTBAR_SLOT_COUNT     4

/* Material Type Identifiers */
#define MATERIAL_GOLD         0
#define MATERIAL_SLATE        1
#define MATERIAL_CYAN_MINT    2
#define MATERIAL_BEDROCK      3

/* Structure tracking the active selection state of our material inventory */
struct HotbarState {
    uint32_t active_slot_index;
    const char* slot_materials[HOTBAR_SLOT_COUNT];
    uint8_t slot_attributes[HOTBAR_SLOT_COUNT];
};

/* Expose our hotbar interface management loops cleanly to C linkage maps */
extern "C" {
    void init_inventory_hotbar();
    void update_hotbar_selection(uint8_t scancode);
    uint32_t get_active_material_type();
    const char* get_active_material_name();
    uint8_t get_active_material_attribute();
}

#endif
