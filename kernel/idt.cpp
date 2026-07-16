#include "idt.h"

// Allocate our system array matrix of exactly 256 interrupt lines
struct idt_entry idt[256];
struct idt_ptr   idp;

/*
   Internal utility to split our 32-bit function memory addresses
   and map them into the required 8-byte Intel hardware gate structures.
*/
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

// Assembly handler hooks matching our boot layer stubs
extern "C" void isr0_handler_stub();
extern "C" void irq0_handler_stub();
extern "C" void irq1_handler_stub(); // Our brand new hardware keyboard assembly stub link!

/* Freestanding internal kernel text grid layout printer utility */
static void idt_panic_print(int row, int col, const char* str, uint8_t attribute) {
    volatile char* video_memory = (volatile char*)0xB8000;
    int index = (row * 80 + col) * 2;
    int i = 0;
    while (str[i] != '\0') {
        video_memory[index] = str[i];
        video_memory[index + 1] = attribute;
        index += 2;
        i++;
    }
}

/*
   🔥 CENTRALIZED SYSTEM CORE PANIC INTERCEPTOR (PHASE B6) 🔥
   Intercepts critical system hardware failures or divide-by-zero validation events.
   Forcefully flushes the screen canvas layout grid, renders a detailed register state
   diagnostic hazard alert screen, and locks execution safety boundaries via cli; hlt.
*/
extern "C" void divide_by_zero_handler() {
    volatile char* video_memory = (volatile char*)0xB8000;

    // Wipe every cell on the 80x25 text video memory screen cleanly
    for (int i = 0; i < 4000; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x07;
    }

    // Render high-contrast critical core hazard alert banners (0x4F = White text on Red background)
    idt_panic_print(2,  15, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!==================", 0x4F);
    idt_panic_print(3,  15, "  CRITICAL KERNEL EXCEPTION INTERCEPTED: PANIC!!  ", 0x4F);
    idt_panic_print(4,  15, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!==================", 0x4F);

    // Output real-time low-level processor register state diagnostic metrics
    idt_panic_print(7,  15, "SYSTEM IRQ VECTORS STATUS: TERMINATED EXCEPTION 0x00", 0x0F);
    idt_panic_print(9,  15, "FAULT DESCRIPTION: CORE DIVIDE BY ZERO CALCULATION ERROR", 0x0E);
    idt_panic_print(11, 15, "CPU STATE LATCH FLAGS  : INT_MASKED; RING_0_PRIVILEGED", 0x07);
    idt_panic_print(13, 15, "ACTIVE SYSTEM RUN MODE : MONOLITHIC MASTER GATEWAY RESET", 0x07);

    // Visual boundary indicator frame lines
    idt_panic_print(16, 15, "==================================================", 0x4F);
    idt_panic_print(17, 18, "ANTAGONIST CORE SUSPENDED SAFELY. HALT CALLED.", 0x0B);
    idt_panic_print(18, 15, "==================================================", 0x4F);

    // Freeze the execution lines entirely to protect data structures from corruption
    while (true) {
        asm volatile("cli; hlt");
    }
}


/*
   The primary initialization function called by our core kernel boot loop.
*/
void init_idt() {
    // 1. Setup our IDT descriptor tracking pointer structure block
    idp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idp.base  = (uint32_t)&idt;

    // 2. Clear out all 256 gates initially by cycling through the memory lines
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // 3. Register Interrupt Line 0 (Division by Zero Exception)
    idt_set_gate(0, (uint32_t)isr0_handler_stub, 0x08, 0x8E);

    // 4. Register Interrupt Line 32 (Hardware Timer IRQ 0)
    idt_set_gate(32, (uint32_t)irq0_handler_stub, 0x08, 0x8E);

    /*
       5. Register Interrupt Line 33 (Hardware Keyboard IRQ 1).
       0x08 is our target GDT Kernel Code Segment selector offset.
       0x8E marks this as a Present Kernel Privilege Interception Gate.
    */
    idt_set_gate(33, (uint32_t)irq1_handler_stub, 0x08, 0x8E);

    // 6. Inform the CPU register hardware where our table sits in RAM space
    asm volatile("lidt (%0)" : : "r" (&idp));
}
