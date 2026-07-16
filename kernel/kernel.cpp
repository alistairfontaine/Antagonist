#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/timer.h"
#include "kernel/pmm.h"
#include "kernel/vmm.h"
#include "kernel/heap.h"
#include "kernel/task.h"
#include "kernel/keyboard.h"

// Forward declare the external distribution entry function at global scope
extern "C" void antagonist_main();
extern "C" void sys_handler_stub(); // Reference our new assembly vector gateway loop point
void kernel_idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags); // Unique local C++ prototype

// Global Scope Hardware Storage Driver Prototypes
extern "C" void ata_write_sector(uint32_t lba, const uint8_t* buffer);
extern "C" void ata_read_sector(uint32_t lba, uint8_t* buffer);

uint32_t count_alpha = 0;
uint32_t count_beta = 0;


/*
   A lightweight, bare-metal string comparison utility.
   Enforces strict AND logic bounds so matching stops exactly at the null terminator!
*/
bool mystrcmp(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }
    return (str1[i] == '\0' && str2[i] == '\0');
}

/*
   Thread Task Alpha.
   Runs concurrently on its own private stack and prints system cycle counts on row 3.
*/
void task_alpha_routine() {
    volatile char* video_memory = (volatile char*)0xB8000;
    while (true) {
        count_alpha = count_alpha + 1;
        char state_char = '0' + (count_alpha % 10);

        video_memory[430] = state_char;
        video_memory[431] = 0x0A; // Light Green style ticker

        for (uint32_t delay = 0; delay < 1000000; delay++) { asm volatile(""); }

        // Cooperatively hand the CPU over to the next task in the ring
        switch_task();
    }
}

/*
   Thread Task Beta.
   Runs concurrently alongside Task Alpha and updates its own separate status ticker on row 4!
*/
void task_beta_routine() {
    volatile char* video_memory = (volatile char*)0xB8000;
    while (true) {
        count_beta = count_beta + 1;
        char state_char = '0' + (count_beta % 10);

        video_memory[590] = state_char;
        video_memory[591] = 0x0D; // Light Purple style ticker

        for (uint32_t delay = 0; delay < 1000000; delay++) { asm volatile(""); }

        // Cooperatively hand the CPU over to the next task in the ring
        switch_task();
    }
}

/*
   Wipe every character cell so leftover firmware text (SeaBIOS banners,
   'Booting from ROM...' residue) cannot linger behind our own console output.
*/
static void clear_screen() {
    volatile char* video_memory = (volatile char*)0xB8000;
    for (int i = 0; i < 4000; i = i + 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x07; // Standard gray-on-black attribute
    }
}

extern "C" void kernel_main() {
    /* 🔥 COPROCESSOR REALIGNMENT PASS 🔥
       Enable the physical x86 Floating Point Unit (FPU) natively on the silicon
       to completely shield our 3D software rendering loops from triple faults!
    */
    asm volatile (
        "fninit\n\t"
        "mov %%cr0, %%eax\n\t"
        "and $0xFFFFFFFB, %%eax\n\t"  // Clear EM (Emulation) flag
        "or $0x00000022, %%eax\n\t"   // Set MP (Monitor Coprocessor) and NE (Numeric Error)
        "mov %%eax, %%cr0\n\t"
        "mov %%cr4, %%eax\n\t"
        "or $0x00000600, %%eax\n\t"   // Set OSFXSR and OSXMMEXCPT for modern compiler math
        "mov %%eax, %%cr4\n\t"
        : : : "eax"
    );

    /* Step 1: Initialize the Core System Engine Segments */
    init_gdt();
    init_idt();

    init_timer(100);
    init_pmm(64 * 1024 * 1024);
    init_vmm();
    init_heap(0x300000, 256);

    /* Initialize our hardware keyboard subsystem */
    init_keyboard();

    /* Step 2: Initialize the Multitasking Scheduler Layer */
    init_multitasking();

    /*
       🔥 HARDWARE GATE REGISTER INTEGRATION 🔥
       Inject our assembly software handler into IDT descriptor gate index 0x80.
       Flags: 0x8E maps a 32-bit Interrupt Gate, and shifting bits tells the CPU
       that this gate can be executed by user-space applications (Privilege Ring 3)!
    */
    kernel_idt_set_gate(0x80, (uint32_t)sys_handler_stub, 0x08, 0xEE);

    /* Step 3: Spawn our parallel runtime threads */
    create_thread(task_alpha_routine);
    create_thread(task_beta_routine);


    /* Start from a pristine screen so firmware boot text cannot bleed into our console */
    clear_screen();

    /* Render Baseline Screen Texts ONCE before loops execute to eliminate memory drift */
    volatile char* video_memory = (volatile char*)0xB8000;

    const char* msg_master = "FontaineOS Architecture Complete! Task Scheduler Loops Active.";
    const char* msg_alpha  = "[Task Alpha Running Concurrently] Cycle State Ticking: ";
    const char* msg_beta   = "[Task Beta Running Concurrently] Cycle State Ticking:  ";
    const char* msg_shell  = "FontaineOS Console Interface Live. Type 'help' or 'clear':";

    int i = 0;
    while (msg_master[i] != '\0') {
        video_memory[0 + (i * 2)] = msg_master[i];
        video_memory[0 + (i * 2) + 1] = 0x0E; // Gold text style on Row 1
        i++;
    }

    i = 0;
    while (msg_alpha[i] != '\0') {
        video_memory[320 + (i * 2)] = msg_alpha[i];
        video_memory[320 + (i * 2) + 1] = 0x0A; // Light Green style on Row 3
        i++;
    }

    i = 0;
    while (msg_beta[i] != '\0') {
        video_memory[480 + (i * 2)] = msg_beta[i];
        video_memory[480 + (i * 2) + 1] = 0x0D; // Light Purple style on Row 4
        i++;
    }

    i = 0;
    while (msg_shell[i] != '\0') {
        video_memory[1440 + (i * 2)] = msg_shell[i];
        video_memory[1440 + (i * 2) + 1] = 0x0B; // Light Cyan style on Row 9
        i++;
    }

    /* Start from a pristine screen so firmware boot text cannot bleed into our console */
    clear_screen();

    /*
       🔥 MASTER DISTRO HANDOFF GATEWAY 🔥
       Launches the direct protected-mode distribution engine safely on the
       wide master boot thread before asynchronous task collisions can happen.
    */
    antagonist_main();

    /* Fallback infinite trap loop safety guard */
    while (true) {
        asm volatile("hlt");
    }
}

/* Freestanding port input utility inline assembly wrapper */
inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/*
   Centralized Monolithic Syscall Router Hub Framework.
   Intercepts user-space command numbers passed inside CPU registers.
   registers[7] = eax value (Syscall Identifier Number)
   registers[6] = ebx value (Argument 1 - text buffers / pointer data blocks)
   registers[5] = ecx value (Argument 2 - target row lanes / column indexes)
*/
extern "C" void syscall_dispatcher(uint32_t* registers) {
    uint32_t syscall_number = registers[7];
    uint32_t arg1           = registers[6];
    uint32_t arg2           = registers[5];

    // Syscall 1: System Print String Raw Gateway
    if (syscall_number == 1) {
        int row = (int)arg2;
        const char* text_buffer = (const char*)arg1;

        volatile char* video_mem = (volatile char*)0xB8000;
        int index = (row * 80 + 10) * 2; // Fixed column alignment center track

        int i = 0;
        while (text_buffer[i] != '\0') {
            video_mem[index] = text_buffer[i];
            video_mem[index + 1] = 0x0E; // Gold text layout style attribute
            index += 2;
            i++;
        }
    }

    // Syscall 2: Direct Hardware Port Input Query Gateway
    else if (syscall_number == 2) {
        uint8_t scancode = inb(0x60);

        // Pass the live scancode byte directly back into user-space via the eax register slot!
        registers[7] = scancode;
    }

    // 🔥 SYSCALL 3: PERSISTENT STORAGE HARD DRIVE SECTOR WRITE GATEWAY (PHASE V) 🔥
    else if (syscall_number == 3) {
        uint32_t target_lba_sector = arg2;             // Passed in ecx (registers)
        const uint8_t* source_buffer = (const uint8_t*)arg1; // Passed in ebx (registers)

        ata_write_sector(target_lba_sector, source_buffer);
    }

    // 🔥 SYSCALL 4: PERSISTENT STORAGE HARD DRIVE SECTOR READ GATEWAY (PHASE P) 🔥
    else if (syscall_number == 4) {
        uint32_t target_lba_sector = arg2;       // Passed in ecx (registers)
        uint8_t* destination_buffer = (uint8_t*)arg1; // Passed in ebx (registers)

        ata_read_sector(target_lba_sector, destination_buffer);
    }
}

/*
   Linker Namespace Interface Bridge.
   Maps our localized proxy calls directly into your kernel's true
   global compiled C++ IDT register manipulation arrays.
*/
void kernel_idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    // Forward-declare your kernel's global un-namespaced name
    extern void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

    // Pass the parameters straight to the physical descriptor arrays!
    idt_set_gate(num, base, sel, flags);
}
