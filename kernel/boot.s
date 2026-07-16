; ========================================================================
; Antagonist Independent Monolithic Bootloader Module (Phase S/W)
; Captures VESA VBE metrics and registers our master Software Interrupt Gate!
; Preserves all hardware interrupt stubs, context switch loops, and privilege tracks.
; ========================================================================

; Define constants for the Multiboot header alignment matrix
MBALIGN    equ 1 << 0           ; Align modules on page boundaries
MEMINFO    equ 1 << 1           ; Provide memory map info flags
VIDEO_MODE equ 1 << 2          ; Activate GRUB Multiboot video mode parameters flag
MAGIC      equ 0x1BADB002       ; Identification signature token
FLAGS      equ MBALIGN | MEMINFO | VIDEO_MODE ; Combined Multiboot tracking mask
CHECKSUM   equ -(MAGIC + FLAGS) ; Verification parity calculation offset

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

    ; Explicit Multiboot Video Mode Field Overrides
    dd 0    ; Mode type flag: 0 select linear pixel graphics mode
    dd 800  ; Targeted horizontal resolution width footprint
    dd 600  ; Targeted vertical resolution height footprint
    dd 32   ; True Color Bit Depth (32-bit depth = RGBA layout channel packing)

section .text
global _start
global isr0_handler_stub
global irq0_handler_stub
global irq1_handler_stub
global sys_handler_stub  ; 🔥 EXPOSE OUR NEW INT 0x80 SOFTWARE SYSCALL GATE STUB LABEL!
global context_switch
global jump_to_ring3     ; 🔥 EXPOSE OUR NEW RING PRIVILEGE TRANSITION ROUTINE!


extern kernel_main
extern divide_by_zero_handler
extern timer_handler
extern keyboard_handler
extern syscall_dispatcher ; Reference our centralized C++ distribution hub handler

_start:
    ; Hand over our newly allocated stack boundary pointer to the CPU stack register
    mov esp, stack_top

    ; Push the incoming GRUB multiboot structure magic tokens onto the stack frames
    push ebx
    push eax

    ; Jump directly into our main freestanding C++ environment kernel loop
    call kernel_main

    ; Safety fallback loop
    cli
halt_loop:
    hlt
    jmp halt_loop

; This is our raw low-level hardware entry stub for Interrupt 0 (Exception Trap)
isr0_handler_stub:
    pusha
    call divide_by_zero_handler
    popa
    iret

; This is our raw low-level hardware entry stub for Interrupt 32 (System Timer IRQ 0)
irq0_handler_stub:
    pusha
    call timer_handler

    ; Direct PIC Master EOI Signal Acknowledgment Command
    mov al, 0x20
    out 0x20, al

    popa
    iret

; This is our raw low-level hardware entry stub for Interrupt 33 (System Keyboard IRQ 1)
irq1_handler_stub:
    pusha
    call keyboard_handler

    ; Direct PIC Master EOI Signal Acknowledgment Command
    mov al, 0x20
    out 0x20, al

    popa
    iret


; 🔥 THE REVOLUTIONARY INT 0x80 SYSTEM CALL INTERRUPT VECTOR STUB 🔥
; Intercepts software interrupts, saves user-space registers, and routes
; system call number commands into our centralized C++ kernel router.
sys_handler_stub:
    pusha                    ; Securely cache all user-space CPU registers
    push esp                 ; Push the current stack pointer frame context as an argument
    call syscall_dispatcher  ; Jump straight into our C++ distribution router hub function
    add esp, 4               ; Clean up our pushed argument parameter line
    popa                     ; Restore user-space registers cleanly to their pristine states
    iret                     ; Special interrupt return pops context blocks and privilege levels!

; ------------------------------------------------------------------
; The Context Switch Engine
; ------------------------------------------------------------------
context_switch:
    mov eax, [esp + 4]
    mov edx, [esp + 8]
    pusha
    mov [eax], esp
    mov esp, edx
    popa
    ret

; ------------------------------------------------------------------
; 🔥 PHASE C3: ASSEMBLY-INSULATED RING 0 TO RING 3 HANDOFF STUB 🔥
; ------------------------------------------------------------------
jump_to_ring3:
    ; Disable interrupts temporarily while we manipulate the privilege frames
    cli

    ; Allocate a local User-Space application Stack Frame register target
    mov ax, 0x23             ; User Data Segment Selector (Gate 4 offset + Ring 3 privilege flags)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Craft the fake Intel hardware Interrupt Return frame block on the stack
    push 0x23                ; SS: Push User Data Segment Selector
    push 0x000E0000          ; ESP: Assign a safe, isolated high-memory user stack pointer address

    ; Push EFLAGS: Re-enable interrupts natively in user-space (0x200 = IF bit high)
    push 0x202

    push 0x1B                ; CS: Push User Code Segment Selector (Gate 3 offset + Ring 3 privilege flags)

    ; Push EIP: Target the memory address pointer of antagonist_main passed from C++
    mov eax, [esp + 24]      ; Extract the function target argument from the calling stack frame
    push eax

    ; Execute the absolute bare-metal hardware privilege degradation sequence!
    iret                     ; Pops parameters, drops privilege to Ring 3, and jumps to antagonist_main!


section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
