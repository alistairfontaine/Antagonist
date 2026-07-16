#ifndef LOADER_H
#define VBOOT_LOADER_H

#include <stdint.h>

/*
   The Master Blueprint for an Insulated User Space Process Stack.
   Simulates CPU execution segment registers, application entry offsets,
   and return codes inside a bounded tracking frame context.
*/
struct ExecutableProcess {
    const char* program_name;
    uint32_t process_id;
    uint32_t virtual_entry_point;
    uint32_t instruction_pointer_offset;
    uint8_t execution_privilege_ring;
    uint32_t status_return_code;
};

extern "C" {
    void execute_bounded_binary(const char* binary_name, const char* instruction_payload);
    const char* get_active_process_log();
    uint32_t get_active_process_status();
}

#endif
