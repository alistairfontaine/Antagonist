#include "hotbar.h"

/*
   The Master Distribution Inventory Hotbar Tracker.
   Allocates a flat global state container inside our static data segment area.
   Maintains the active item index focus and material metadata attributes.
*/
static HotbarState inventory_hotbar __attribute__((aligned(4)));

/*
   Initializes the hotbar data layouts and text labels at boot time.
*/
extern "C" void init_inventory_hotbar() {
    inventory_hotbar.active_slot_index = 0; // Default focus starts on Slot 1

    // Bind asset string descriptions directly to the material array matrices
    inventory_hotbar.slot_materials[MATERIAL_GOLD]      = "GOLD BLOCK ";
    inventory_hotbar.slot_materials[MATERIAL_SLATE]     = "SLATE VOXEL";
    inventory_hotbar.slot_materials[MATERIAL_CYAN_MINT]  = "CYAN MATRIX";
    inventory_hotbar.slot_materials[MATERIAL_BEDROCK]    = "BEDROCK ADM";

    // Bind matching VGA text attribute color codes to each material entry
    inventory_hotbar.slot_attributes[MATERIAL_GOLD]     = 0x0E; // Gold color
    inventory_hotbar.slot_attributes[MATERIAL_SLATE]    = 0x08; // Dark Slate gray
    inventory_hotbar.slot_attributes[MATERIAL_CYAN_MINT] = 0x0B; // Electric Cyan
    inventory_hotbar.slot_attributes[MATERIAL_BEDROCK]   = 0x0F; // Crisp White
}

/*
   Interceptors hardware scancodes and shifts the active index parameters.
   Scancode 0x02 = Main Row Key '1'
   Scancode 0x03 = Main Row Key '2'
   Scancode 0x04 = Main Row Key '3'
   Scancode 0x05 = Main Row Key '4'
*/
extern "C" void update_hotbar_selection(uint8_t scancode) {
    if (scancode == 0x02) {
        inventory_hotbar.active_slot_index = MATERIAL_GOLD;
    } else if (scancode == 0x03) {
        inventory_hotbar.active_slot_index = MATERIAL_SLATE;
    } else if (scancode == 0x04) {
        inventory_hotbar.active_slot_index = MATERIAL_CYAN_MINT;
    } else if (scancode == 0x05) {
        inventory_hotbar.active_slot_index = MATERIAL_BEDROCK;
    }
}

/*
   Returns the raw integer material identifier code.
*/
extern "C" uint32_t get_active_material_type() {
    return inventory_hotbar.active_slot_index;
}

/*
   Returns the text string descriptor pointer of the currently focused slot.
*/
extern "C" const char* get_active_material_name() {
    return inventory_hotbar.slot_materials[inventory_hotbar.active_slot_index];
}

/*
   Returns the text style attribute color byte of the active entry.
*/
extern "C" uint8_t get_active_material_attribute() {
    return inventory_hotbar.slot_attributes[inventory_hotbar.active_slot_index];
}
