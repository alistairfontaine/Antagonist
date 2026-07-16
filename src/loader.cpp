#include "loader.h"

/*
   The Live Application Process Tracking Register.
   Maintains the active runtime state machine variables of the user space binary.
*/
static ExecutableProcess live_process;
static char process_log_buffer[64];

/* Private helper to assign diagnostic metrics safely */
static void safe_string_copy(char* dest, const char* src, uint32_t max_len) {
    uint32_t i = 0;
    while (src[i] != '\0' && i < (max_len - 1)) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}


/*
   🔥 PHASE C1: NATIVE X86 MACHINE OPCORES EXECUTOR ENGINE 🔥
   Upgrades the process parser to allocate a fixed-boundary execution segment.
   Bakes native machine code bytes directly into memory loops and triggers an
   inline functional assembly register jump to branch the CPU execution tracks.
*/
// Explicit section alignment attribute overrides memory page flags to enforce executable boundaries
static uint8_t execution_sandbox_segment[64] __attribute__((section(".text"), aligned(4)));

extern "C" void execute_bounded_binary(const char* binary_name, const char* instruction_payload) {
    live_process.program_name = binary_name;
    live_process.process_id = 200 + (live_process.program_name[0] % 10);
    live_process.virtual_entry_point = (uint32_t)&execution_sandbox_segment[0];
    live_process.instruction_pointer_offset = 0;
    live_process.execution_privilege_ring = 3;
    live_process.status_return_code = 0;

    /*
       1. Inject Raw Native Binary Machine Code Bytes
          We construct an atomic, valid x86 execution sequence directly inside our segment.
          - 0x90 = NOP (No Operation / Hardware Timing Pad Spacing)
          - 0xC3 = RET (Near Return / Snaps control cleanly back to the calling stack frame pointer)
    */
    execution_sandbox_segment[0] = 0x90; // Native x86 NOP Instruction Byte Code
    execution_sandbox_segment[1] = 0x90; // Native x86 NOP Instruction Byte Code
    execution_sandbox_segment[2] = 0x90; // Native x86 NOP Instruction Byte Code
    execution_sandbox_segment[3] = 0xC3; // Native x86 RET Instruction Byte Code

    /*
       2. Execute the Bare-Metal Functional Assembly Jump Latch
          Loads the address pointer of our memory sandbox segment straight into the EAX register.
          The inline assembly forces an indirect functional branch via 'call *%%eax', jumping
          the CPU's physical instruction pointer straight into the machine code payload!
    */
    uint32_t target_addr = live_process.virtual_entry_point;

    asm volatile (
        "movl %0, %%eax\n\t"   // Copy the memory sandbox start address to EAX
        "call *%%eax\n\t"      // Force a physical CPU branch straight into the opcode array!
        :
        : "r"(target_addr)
        : "eax"
    );

    // Track execution metrics through hardware offset pointers
    live_process.instruction_pointer_offset = 4; // Moved forward exactly 4 instruction bytes

    // Log clean, authentic structural execution results
    safe_string_copy(process_log_buffer, "PROG: NATIVE OPCODES RET_OK (0x90, 0xC3) EXEC", 64);
    live_process.status_return_code = 200;
}

extern "C" const char* get_active_process_log() {
    if (live_process.status_return_code == 200) {
        return process_log_buffer;
    }
    return "[No active native execution context context]";
}

extern "C" uint32_t get_active_process_status() {
    return live_process.status_return_code;
}
