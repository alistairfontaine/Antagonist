# Antagonist OS Distribution

Antagonist is an advanced, bare-metal, neural-interactive operating system distribution built entirely from the silicon up. Hosted on a self-contained, locally integrated `FontaineOS` micro-kernel core, it integrates a hardware-independent 3D voxel engine (`MontaigneEngine`) and a native background neural tensor network language engine (`Fontana`) into a unified, completely standalone computing platform with zero underlying host OS dependencies.

---

## 🔧 Distribution Core Principles
* **Independent Workspace Architecture:** Embeds all core kernel files, boot structures, and graphics engines into a single, localized repo tree to guarantee completely portable forks.
* **Direct Protected-Mode Video Streaming:** Bypasses unsafe 16-bit VESA BIOS interrupts, writing text and performance matrix frames directly to the physical VGA framebuffer pointer address (`0xB8000`) natively from 32-bit Protected Mode.
* **Cooperative Supervisor Slicing:** Embeds the distribution's entry point hooks directly into the core kernel boot track, maintaining full control over execution loops with zero timing collisions.

---

## 🗺 Monolithic Order of Operations (OOO) Roadmap

### 🛠 Part I: The Silicon & Display Realignment (Phases A–G)
* **[x] Phase A: The Distro Workspace Architecture** -> Initialize the master multi-directory tree layout linking your kernel binaries to the compiled engine objects.
* **[x] Phase B: The High-Resolution VESA VBE Video Driver** -> Program the graphics card's physical I/O ports (`0x03C0`–`0x03DF`) and map the VESA BIOS Extensions (VBE) to lock the machine into high-resolution Linear Framebuffer (LFB) space.
* **[x] Phase C: Raw Double-Buffered Graphics Pipelines** -> Allocate a continuous 64KB secondary back-buffer array in RAM to completely eliminate on-screen flickering.
* **[x] Phase D: Custom VGA Color Palette Mapping** -> Write raw RGB hex color matrices directly to the DAC registers via port `0x03C8` and `0x03C9` to match your engine's dark slate, gold, and cyan design scheme.
* **[x] Phase E: Multi-Drive Master Boot Record (MBR) Realignment** -> Reconfigure your automated loop tools to generate an ISO image backing up dual-drive virtualization maps inside QEMU.
* **[x] Phase F: The Host Interrupt Clock Overclock Pass** -> Accelerate your kernel's Programmable Interval Timer (PIT) from 100Hz to 1000Hz (1ms tick intervals) to accurately drive the graphics engine's frame timers.
* **[x] Phase G: Hardware Keyboard Scancode Matrix Expansion** -> Update your keyboard maps to monitor multi-key combinations (like `W` + `Space`) to handle smooth 3D camera navigation and jumps simultaneously.

### 🎨 Part II: The Graphics & Matrix Porting Layer (Phases H–N)
* **[x] Phase H: Freestanding Math Array Abstractions** -> Implement freestanding, zero-allocation versions of 3D vectors (`Vec3`) and 4x4 Row-Major transformation matrices (`Mat4`) without using standard library headers.
* **[x] Phase I: Software 3D Ray-to-VGA Rasterization Engine** -> Port your engine's custom projection pipelines to draw 3D voxel blocks onto a 2D viewport by translating row-major MVP transformations natively on the CPU.
* **[x] Phase J: Bounding Box (AABB) Collision Integration** -> Inject your engine's hard-clipping bounding box physics into the kernel's keyboard loop to stop your camera coordinate offsets from sliding through blocks.
* **[x] Phase K: Broad-Phase Spatial Partitioning Cell Mapping** -> Implement the constant-time O(N) virtual 3D room coordinate sectors directly inside kernel memory space.
* **[x] Phase L: Interactive Raycast Selection Core** -> Map your engine's raycast picker loop into the hardware mouse/keyboard handler to step sampling lines out along your look vector on click.
* **[x] Phase M: 3D Visual Particle Debugger Trails** -> Log vector tracers inside a static global ring array to draw textured particle tracking lines across the camera sightline.
* **[x] Phase N: Voxel Block Placement & Destruction Engine** -> Deploy 3D axis delta evaluations within the interrupt loop to snap new blocks flush against target cube faces on click commands.

### 🧠 Part III: The Neural Tensor Cognitive Core (Phases O–U)
* **[x] Phase O: Freestanding Tensor Float Matrix Engine** -> Cross-compile your `tensor_engine.cpp` backend using your freestanding kernel flags (`-fno-exceptions -fno-rtti`) to isolate its loops from Linux system dependencies.
* **[x] Phase P: High-Integrity Weight Serialization Mapping** -> Map your kernel's ATA hard drive block driver to read your binary weight sequences (`fontana_weights.bin`) off specific sectors of your disk image directly into a safe, flat global matrix array.
* **[x] Phase Q: Vocabulary Metadata Meta Mappings Parser** -> Write a low-level character parsing array to parse your `vocab_meta.json` mappings without a dynamic JSON parsing library.
* **[x] Phase R: Neural Tokenizer Core Routing** -> Connect your probabilistic `tokenizer.py` logic straight into your C++ tensor engine matrix data routines inside the kernel.
* **[x] Phase S: Multi-Thread Context Stack Isolation** -> Expand your multitasking kernel scheduler to handle three completely isolated execution threads running on custom 4KB stacks.
* **[x] Phase T: Cognitive Thread Background Slicing** -> Delineate execution loops: Thread 1 runs graphics rasterization at high frame rates, while Thread 2 evaluates your tensor neural predictions in the background without blocking screen rendering.
* **[x] Phase U: Interactive Neural-Workstation Command Terminal** -> Route the background neural output text strings to print directly onto row lines of your high-resolution graphical display.

### 📦 Part IV: The Public Release Runway (Phases V–Z)
* **[x] Phase V: Persistent Disk Sandbox Saving/Loading** -> Wire the hard disk block driver to dump your entire active voxel matrix configuration block state onto Sector 2 of your storage drive via a hotkey save command.
* **[x] Phase W: Master Distribution Makefile Framework** -> Write a monolithic `Makefile` to compile your bootloader, kernel, graphics engine, and neural network backend into one unified binary file.
* **[x] Phase X: Real-Time Performance Profiler Interface** -> Draw a live on-screen overlay measuring active ticks, free memory frames from your PMM bitmap, and scheduler thread states.
* **[x] Phase Y: The Automated Antagonist Release Deployment Script** -> Write a bash deployment pipeline script (`build_distro.sh`) to automate compilation, back up your progress, and push the entire distribution tree to GitHub.
* **[x] Phase Z: System Halt Goodbye Gateway** -> Implement a safe power-down state command that cleanly clears display frames, outputs a farewell signature log line, and locks the silicon via `cli; hlt`.

### 🚀 Part V: The Horizon Evolution Stack (Phases A1–A4)
* **[x] Phase A1: Dynamic Voxel Ambient Occlusion Vertex Shading ...
* **[x] Phase A2: Procedural Perlin Noise Terrain Generation Loops ...
* **[x] Phase A3: Creative Voxel Hotbar Selection Arrays ...
* **[x] Phase A4: Dynamic Day/Night Vector Sun Shader Rotations ...

---

## 🛠 Required Distribution Build Stack
* **Compiler:** `g++` (Freestanding i386 Cross-Compilation Flags Matrix)
* **Linker:** `ld` (Targeted hardware layout memory mapping configuration)
* **Assembler:** `nasm` (Multiboot entrypoint runtime abstractions)
* **Virtualizer:** `qemu-system-i386` (With explicitly configured raw format secondary master hard drive bindings)
* **Distro Tools:** `xorriso`, `mtools`, `binutils-multiarch`


---

## 🗺 Part VI: The Subsystem Architecture Extension Runway (Phases B1–B7)

Following the stabilization of the system call gateway vectors and interactive file system workstation tables, the roadmap transitions into establishing standalone kernel execution isolation boundaries:

* **[x] Phase B1: Software Interrupt Scancode Buffer Debouncer** -> Integrate an explicit timestamp or delta tick state array mask over the direct I/O port `0x60` queries to eliminate scancode repeat bouncing and stabilize structural character state streaming.
* **[x] Phase B2: Virtual File Allocation Node Serializer (VFAT)** -> Map a static indexing block structural array linked directly to the IDE storage drivers, allowing directories to physically load, catalog, and modify raw data buffers persistent across hardware reboots.
* **[x] Phase B3: Bounded Executable Object Loader (/bin Program Parser)** -> Write an insulated system subroutine capable of fetching a binary offset block straight off raw disk storage sectors, parsing its entry points, and executing it within a restricted stack boundary space.
* **[x] Phase B4: Isolated Ring 3 Privilege Window Traps** -> Configure the Task State Segment (TSS) and update the Global Descriptor Table (GDT) segment register descriptors to physically launch user-space shell logic inside unprivileged Ring 3, fully insulated from Ring 0 supervisor execution lines.
* **[x] Phase B5: Hardware Interrupt Cascading PIC EOI Alignment** -> Audit the keyboard and system timer interrupt stub routines inside the assembly boot matrix to enforce absolute, instant End of Interrupt (EOI) acknowledgments, preventing asynchronous device choking.
* **[x] Phase B6: Centralized System Core Panic Interceptor** -> Write a definitive supervisor validation loop that intercepts unhandled memory safety exceptions, wipes the active text-video canvas, and prints a comprehensive register state diagnostic dump before locking the silicon via `cli; hlt`.
* **[x] Phase B7: Standalone Workstation Public Release Matrix Image** -> Update pipeline build parameters to package un-mangled kernel binary stubs, GRUB configuration boot trees, and serialized storage images into a unified, bootable ISO delivery file.

