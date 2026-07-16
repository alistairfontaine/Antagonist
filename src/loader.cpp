#include "loader.h"

/*
   The Live Application Process Tracking Register.
   Maintains the active runtime state machine variables of the user space binary.
*/
static ExecutableProcess live_process;
static char process_log_buffer[64];

/* Private helper to parse mock binary instructions safely */
static void safe_string_copy(char* dest, const char* src, uint32_t max_len) {
    uint32_t i = 0;
    while (src[i] != '\0' && i < (max_len - 1)) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

/*
   Processes an incoming instruction payload string.
   Simulates jumping to an entry point and executing the machine tracking codes.
*/
extern "C" void execute_bounded_binary(const char* binary_name, const char* instruction_payload) {
    live_process.program_name = binary_name;
    live_process.process_id = 100 + (instruction_payload[0] % 10);
    live_process.virtual_entry_point = 0x08048000;
    live_process.instruction_pointer_offset = 0;
    live_process.execution_privilege_ring = 3; // Ring 3 User Emulation
    live_process.status_return_code = 0;

    // Scan through the virtual instruction bytes within safety boundaries
    uint32_t step = 0;
    while (instruction_payload[step] != '\0' && step < 64) {
        live_process.instruction_pointer_offset = step;

        // Mock execution decoding gates based on characters found in the payload string
        if (instruction_payload[step] == 'E') {
            // "E" instruction signals an explicit environment handshake pass
            safe_string_copy(process_log_buffer, "PROG: INIT_OK HANDSHAKE PASS", 64);
        } else if (instruction_payload[step] == 'L') {
            // "L" instruction signals a low-level graphics context allocation block
            safe_string_copy(process_log_buffer, "PROG: SYS_VBE_MAP REGISTER_OK", 64);
        } else if (instruction_payload[step] == 'G') {
            // "G" instruction signals a general software gateway loop ignition
            safe_string_copy(process_log_buffer, "PROG: WORKSPACE RUNNING ACTIVE", 64);
        }

        step++;
    }

    // Set final exit return code to show full structural parsing success
    live_process.status_return_code = 200; // HTTP-style OK execution status code
}

extern "C" const char* get_active_process_log() {
    if (live_process.status_return_code == 200) {
        return process_log_buffer;
    }
    return "[No active process tracking context]";
}

extern "C" uint32_t get_active_process_status() {
    return live_process.status_return_code;
}
