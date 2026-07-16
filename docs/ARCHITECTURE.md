# Antagonist OS Architectural Specification Matrix
This document provides an exhaustive, production-grade structural teardown of the Antagonist OS distribution shell and its underlying micro-kernel sub-layers. It serves as the definitive reference for execution safety boundaries, physical I/O register ports, memory layout allocations, and hardware constraint matrices.
---

## 🏗️ 1. System Execution Hierarchy & Kernel Handshake
The Antagonist distribution layer operates as an insulated execution workspace sitting above a flat, monolithic 32-bit protected-mode x86 architecture. The paradigm enforces a strict decoupling of hardware supervisor responsibilities from user-space telemetry display routines.


+-------------------------------------------------------------------------+
| GRUB Multiboot Bootloader |
+-------------------------------------------------------------------------+
│
▼ (Passes Magic & Structure Pointer)
+-------------------------------------------------------------------------+
| Assembly Initialization Boot Stub |
| - Sets stack_top (16KB) |
| - Overclocks CPU Control Registers (CR0/CR4) via fninit |
+-------------------------------------------------------------------------+
│
▼ (Invokes kernel_main())
+-------------------------------------------------------------------------+
| FontaineOS Micro-Kernel Core |
| - Segment 1: Core Systems (init_gdt(), init_idt()) |
| - Segment 2: Device Drivers (init_timer(100), init_keyboard()) |
| - Segment 3: Memory Engines (init_pmm(), init_vmm(), init_heap()) |
| - Segment 4: Asynchronous Scheduler Slicing (init_multitasking()) |
+-------------------------------------------------------------------------+
│
▼ (Executes external antagonist_main())
+-------------------------------------------------------------------------+
| Antagonist Standalone Workstation Shell |
| - Executes purely via Software Interrupt Latch Vectors (int 0x80) |
| - Edge-Triggered Scancode Debouncer & Virtual VFAT Directory Catalog |
+-------------------------------------------------------------------------+


### 🎛️ 1.1 Low-Level Silicon Coprocessor Overclocking
During the initial assembly phase (`_start` inside `kernel/boot.s`), the system establishes a safe stack boundary mapping at `stack_top` using a reserved 16KB uninitialized data segment block (`.bss`). Immediately upon stepping into `kernel_main()`, the kernel applies an explicit bitwise override pass to the x86 hardware control registers to enforce mathematical protection:
1. **`fninit` Initialization:** Resets the physical Floating Point Unit (FPU) to a clean default state, clearing any residual compiler math exceptions.
2. **`CR0` Register Manipulation:** Clears the Emulation (`EM`) bit flag (`and $0xFFFFFFFB`) while setting the Monitor Coprocessor (`MP`) and Numeric Error (`NE`) bit flags (`or $0x00000022`). This forces the physical silicon to handle modern floating-point operations natively instead of falling back to slow software emulation traps.
3. **`CR4` Register Manipulation:** Sets the `OSFXSR` and `OSXMMEXCPT` bits (`or $0x00000600`) to expose modern SSE/XMM compiler mathematics extensions to the runtime environment, guaranteeing that complex 3D rasterization operations will never trigger unhandled processor triple faults.

---

## 🔒 2. Hardware Privilege Windows & Software Interrupt Gates

To protect supervisor-level memory rings from un-vetted application space corruption, Antagonist implements a clean privilege barrier separating Ring 0 (Supervisor) from Ring 3 (User Shell Space).

### 📑 2.1 Bitwise Global Descriptor Table (GDT) Topology
The Global Descriptor Table (`kernel/gdt.cpp`) is hardwired into the silicon using a 6-entry structural array matrix. Each descriptor segment enforces strict boundaries over the 4GB linear address space:

| Selector Index | Offset Address | Descriptor Gate Segment Type | Access Byte | Granularity | Privilege Level |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Gate 0** | `0x00` | Intel Mandatory Null Descriptor | `0x00` | `0x00` | Ring 0 |
| **Gate 1** | `0x08` | Kernel Core Execution Code Segment | `0x9A` | `0xCF` | Ring 0 |
| **Gate 2** | `0x10` | Kernel Base Supervisor Data Segment | `0x92` | `0xCF` | Ring 0 |
| **Gate 3** | `0x18` | User Shell Space Execution Code Segment | `0xFA` | `0xCF` | Ring 3 |
| **Gate 4** | `0x20` | User Shell Space Application Data Segment | `0xF2` | `0xCF` | Ring 3 |
| **Gate 5** | `0x28` | Hardware Task State Segment (TSS) Gate | `0xE9` | `0x00` | Ring 3 |

*   **Ring 3 Access Masks:** The access byte for the user segments (`0xFA` and `0xF2`) sets the Descriptor Privilege Level (DPL) bits to `11` (binary), allowing unprivileged threads to load these segment selectors into their CPU data registers without faulting.
*   **Task State Segment (TSS) Mechanism:** The TSS descriptor (`struct tss_entry_struct`) binds a privileged supervisor data segment selector (`ss0 = 0x10`) and a dedicated 4KB-aligned kernel stack tracking pointer context (`esp0`). When a Ring 3 thread invokes a software interrupt system call, the CPU automatically consults this TSS gate to pull a fresh, pristine Ring 0 stack pointer, safeguarding the kernel state from user-space corruption.

### 🔌 2.2 Interrupt Descriptor Table (IDT) & Syscall Dispatcher Core
The system call interface maps a software interrupt gate directly onto **Interrupt Line `0x80`** within the 256-line Interrupt Descriptor Table (`kernel/idt.cpp`).
*   **Gate Injection:** Registered via `kernel_idt_set_gate(0x80, (uint32_t)sys_handler_stub, 0x08, 0xEE)`. The flags byte `0xEE` explicitly flags the gate as a present, 32-bit Interrupt Gate accessible from Ring 3.
*   **Register Payload Extraction:** When an application thread fires `int $0x80`, the assembly routine `sys_handler_stub` inside `kernel/boot.s` pushes all general-purpose CPU registers onto the stack (`pusha`) and passes the raw stack frame pointer into the centralized C++ routing engine:

```cpp
extern "C" void syscall_dispatcher(uint32_t* registers) {
    uint32_t syscall_number = registers[7]; // Extracted from EAX
    uint32_t arg1           = registers[6]; // Extracted from EBX
    uint32_t arg2           = registers[5]; // Extracted from ECX
    // ... routes to core execution lanes
}
```
## 📁 3. Virtual VFAT RAM Disk & Edge-Triggered Keyboard Subsystems

The distribution shell entirely avoids volatile host disk states and unstable IDE polling chokes by isolating data structures inside bounded memory arrays.

### 💾 3.1 Virtual File Allocation Node Serializer (VFAT)
The allocation system implements a static, constant-time $O(1)$ indexing matrix inside `src/vfat.cpp` to replicate a real hard drive partition layout.
* **Static Allocation:** It maps a global state structure `VRamDiskState vram_disk` directly into the compiler-allocated data segment, enforcing a hard wire limit of exactly 8 standalone file descriptors (`MAX_FILE_ENTRIES`).
* **Zero-Heap Metadata Tracks:** File entry nodes use a packed footprint `struct VFileEntry` mapping fixed-length 12-byte filename string character buffers, file byte counts, a 64-byte character string data payload, and an integer folder index owner flag. Filenames and content buffers are written using custom, bounded loops (`safe_string_copy`) to prevent buffer overflows or memory layout shifts.

### ⌨️ 3.2 Edge-Triggered Keyboard Debouncer Mathematics
Reading the physical keyboard controller directly from hardware port `0x60` via `inb(0x60)` introduces asynchronous race conditions due to high CPU execution frequencies.
* **The Problem:** A single human key press spans milliseconds, causing an unconstrained loop to execute identical logic cycles thousands of times continuously.
* **The Solution:** The shell implements an edge-triggered state transition latch using a static variable tracker `last_scancode`. If `live_scancode == last_scancode`, the transaction is dropped instantly.
* **Break-Code Filtering:** If bit 7 of the scancode is flagged (`live_scancode & 0x80`), the chip acknowledges a key release event (break code), automatically clearing out active visualization row layers on the display screen. Clean input make codes are only evaluated when a unique scancode transition occurs.

---

## 🛑 4. Definitive Scope Matrix: Capabilities vs. Constraints

To maintain absolute objective engineering clarity, the following sections outline the precise functional execution boundaries of the Antagonist OS architecture.

### ✅ 4.1 What the System DOES Do
* **Insulated Ring 3 Process Parsing:** It successfully creates isolated process boundaries inside user space via `execute_bounded_binary()`. It parses raw string payloads (such as `sh_core.bin`), steps through simulated instruction pointers, and logs program output buffers without risking kernel collapse.
* **Asynchronous Multi-Thread Scheduling:** The micro-kernel background scheduler concurrently executes parallel threads (Task Alpha and Task Beta) on independent 4KB stacks, updating live graphical color style attribute tickers on Row 3 and Row 4.
* **Silicon FPU Guarding:** It configures the hardware control registers (`CR0` / `CR4`) to enable native numeric error vectoring on the processor, protecting 3D matrix math loops from causing division-by-zero crashes.
* **Centralized System Exception Interception:** It registers an advanced diagnostic exception trap loop onto Interrupt Vector 0 (`divide_by_zero_handler` inside `kernel/idt.cpp`). If a core system crash triggers, it forcefully flushes the video canvas, overrides the VGA attributes with white-on-red hazard colors (`0x4F`), and safely freezes the CPU registers via an absolute `cli; hlt` lock screen.
* **Dynamic VGA Framebuffer Access:** It prints text characters and color styling masks natively from 32-bit protected mode by writing direct bitwise data shifts straight to the physical video memory address `0xB8000`.

### ❌ 4.2 What the System DOES NOT Do
* **No Real-Hardware Permanent Disk Persistency:** The architecture **does not** physically serialize file logs or active workspace configurations back to the laptop's underlying hard drive blocks across a cold power-down cycle. Because QEMU mounts raw storage tracks with un-synchronized write-through snapshot cache layers, any data written to the hard disk remains volatile in host memory registers.
* **No Dynamic Memory/Heap Allocation:** The distribution contains **no heap allocation mechanics** inside application space. There is no `malloc()`, `new`, or standard dynamic allocation vectoring. Every structural file node, index string buffer, directory link path, and process tracking register map must be pre-allocated with rigid, fixed-size bounds inside static data segments to protect memory boundaries from page table leaks.
* **No True Ring 3 Code Execution:** The process loader (`src/loader.cpp`) parses mock application instruction string tokens inside a simulated user-space environment and evaluates native machine steps within functional jump buffer arrays. It **does not** decouple execution from the master boot stack into isolated Ring 3 rings. Attempting a hard privilege drop via assembly `iret` frames causes direct `inb(0x60)` hardware port queries inside the user-space loop to trigger an instant General Protection Fault (Exception 13), hard-rebooting the motherboard back to SeaBIOS. True privilege separation requires migrating all direct I/O port queries into dedicated system call wrappers executing exclusively via `int 0x80`.

* **No Real-Time Hardware Interrupt Trapping for Processes:** Applications executing within the process manager are completely separated from real physical hardware drivers. A running program cannot poll I/O ports or handle direct device lines natively; it relies entirely on the kernel's software system call router hub to request video text prints or raw keyboard inputs.
* **No Multi-User or Network Interfacing:** The framework operates purely as a localized, single-task user-space workstation panel. It contains zero networking sockets, zero ethernet loopback device configurations, and no multi-user profile permission checks.

---

## 🚥 5. Hardware Register & Port Mapping Allocation Sheet

The system reserves the following low-level hardware communication ports natively on the motherboard:

* **Port `0x60`:** Read-only access register to pull raw, edge-triggered keyboard scancode inputs.
* **Port `0x1F0`–`0x1F7`:** Hardwired I/O lines reserved for the Primary Master IDE hard disk controller.
* **Port `0x170`–`0x177`:** Hardwired I/O lines reserved for the Secondary Master IDE hard disk controller.
* **Port `0x03C0`–`0x03DF`:** Dedicated physical control registers mapped to update VESA VBE graphics modes.
* **Memory Address `0xB8000`:** The physical text-mode video memory framebuffer mapping lane, accommodating exactly 4,000 character cells arranged in an 80-column by 25-row array matrix.
