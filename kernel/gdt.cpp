#include "gdt.h"

// Allocate our flat memory array matrix expanded to exactly 6 descriptors (GATES 0-5)
struct gdt_entry gdt[6];
struct gdt_ptr   gp;

// Hardware Task State Segment Instance Container
struct tss_entry_struct tss_entry __attribute__((aligned(4)));


/*
   Internal utility to break apart 32-bit memory addresses and sizes
   and pack them into the specific bitfields the CPU hardware demands.
*/
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access      = access;
}

/*
   The main initialization function called by our core kernel boot loop.
*/
void init_gdt() {
    // 1. Setup our GDT descriptor tracking pointer structure block sized for 6 descriptors
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base  = (uint32_t)&gdt;

    // 2. Gate 0: The Mandatory Intel Null Descriptor (Must be completely clear)
    gdt_set_gate(0, 0, 0, 0, 0);

    // 3. Gate 1: Kernel Code Segment (Base 0, Limit 4GB, Ring 0)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // 4. Gate 2: Kernel Data Segment (Base 0, Limit 4GB, Ring 0)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // 5. Gate 3: User Space Code Segment (Base 0, Limit 4GB, Ring 3 Privileges)
    // Access 0xFA = Present, Ring 3 (User Shell), Executable, Readable
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // 6. Gate 4: User Space Data Segment (Base 0, Limit 4GB, Ring 3 Privileges)
    // Access 0xF2 = Present, Ring 3 (User Shell), Read/Write, Writable
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    /*
       7. Gate 5: Hardware Task State Segment Descriptor Mapping (Ring 3 Switch)
          Access 0xE9 = Present, Ring 3 Execution Access, Executable Hardware TSS descriptor type
    */
    uint32_t tss_base = (uint32_t)&tss_entry;
    uint32_t tss_limit = sizeof(struct tss_entry_struct) - 1;
    gdt_set_gate(5, tss_base, tss_limit, 0xE9, 0x00);

    // Zero out the hardware TSS state tracking allocation structures cleanly
    uint8_t* tss_ptr = (uint8_t*)&tss_entry;
    for (uint32_t i = 0; i < sizeof(struct tss_entry_struct); i++) {
        tss_ptr[i] = 0;
    }

    // Bind our kernel supervisor stack segment and a safe scratch memory stack frame pointer base
    tss_entry.ss0  = 0x10;             // Kernel Data Segment Selector (Gate 2 offset address)
    static uint8_t tss_kernel_stack[4096]; // Private 4KB aligned privilege switch stack frame
    tss_entry.esp0 = (uint32_t)&tss_kernel_stack[4096];

    /*
       8. Inform the CPU register hardware where our table sits in RAM.
       - 'lgdt' loads our new 6-entry descriptor block.
       - 'ltr' loads the Task Register with our Gate 5 TSS selector index (0x28 offset address).
    */
    asm volatile(
        "lgdt (%0)\n\t"
        "ljmp $0x08, $.reload_segments\n\t"
        ".reload_segments:\n\t"
        "movw $0x10, %%ax\n\t"
        "movw %%ax, %%ds\n\t"
        "movw %%ax, %%es\n\t"
        "movw %%ax, %%fs\n\t"
        "movw %%ax, %%gs\n\t"
        "movw %%ax, %%ss\n\t"
        "movw $0x2B, %%ax\n\t"  // 0x28 offset plus Ring 3 privilege selector request bits = 0x2B
        "ltr %%ax\n\t"          // Load x86 Hardware Task Register
        : : "r" (&gp) : "ax"
    );
}

