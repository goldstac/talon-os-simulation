# Talon OS Simulation

![penguinfetch](penguinfetch.png)

> **⚠️ Under Development** — This project is still actively receiving updates. Some components (e.g. DXE) are stubs/skeletons and not yet fully implemented.

A user-space simulation of the Talon Linux operating system environment, written primarily in C++ with C components. It features a shell interface, a simulated kernel boot sequence, UEFI firmware emulation, a driver execution environment (DXE), driver registry, NVRAM simulation, and virtual filesystem subsystems.

## Features

- **Shell** (`main.cpp`) — A `[root@talon]` command-line REPL supporting commands like `reboot uefi`, `penguinfetch`, `cat proc/cpuinfo`, `pacman -Syu`, `free -h`, `df -h`, `iwctl`, and more
- **Kernel** (`kernel/kernel.c`) — Simulated Linux kernel boot sequence with driver-loading messages and cross-platform sleep
- **UEFI** (`uefi.cpp`) — Mock UEFI setup screen showing boot order, secure boot, XMP, and TPM status
- **DXE** (`dxe/dxe.cpp`) — Driver Execution Environment with 16+ driver initialization stubs (CPU, RAM, PCIe, NVMe, SATA, GPU, Ethernet, WiFi, USB, audio, and more)
- **Driver Registry** (`driver_registry.cpp`) — Simple device driver registration system
- **NVRAM** (`nvram.cpp`) — UEFI NVRAM variable stubs (secure boot, boot entry, TPM 2.0)
- **SEC** (`sec.cpp`) — UEFI Security (SEC) phase stub
- **Virtual Filesystem** (`filesystem/`) — Simulated `/proc/cpuinfo` (Intel Core Ultra 9 285K) and `/proc/gpuinfo` (Nvidia GeForce RTX 5090)
- **Hardware Detection** (`system/hardware/system.cpp`) — Hardware profile stubs
- **Logo** (`logo.cpp`) — ASCII art "Talon Linux" branding displayed by `penguinfetch`
- **Arch UEFI** (`arch-uefi/`) — Standalone GRUB-like bootloader (C) and incomplete Arch shell (C++)

## Build & Run

### Linux / macOS
```sh
# Quick build and run
g++ -o main main.cpp driver_registry.cpp kernel/kernel.c filesystem/proc/cpuinfo.cpp filesystem/proc/gpuinfo.cpp uefi.cpp logo.cpp && ./main
```

### Windows
```bat
g++ -o main.exe main.cpp kernel/kernel.c driver_registry.cpp filesystem/proc/cpuinfo.cpp && main.exe
```

### Scripts
- `quick.sh` — one-liner build + run
- `run.sh` — full build with additional utilities
- `run.bat` — Windows build + run
- `compile.sh` — compile individual utility objects

## Project Structure

```
├── main.cpp              # Entry point and shell command dispatch
├── uefi.cpp / .h         # UEFI firmware simulation
├── driver_registry.cpp   # Driver registration stubs
├── nvram.cpp / .h        # UEFI NVRAM variable simulation
├── sec.cpp               # Security (SEC) phase stubs
├── logo.cpp / .h         # ASCII art Talon Linux logo
├── kernel/
│   ├── kernel.c          # Simulated kernel boot sequence
│   ├── kernel.h          # Kernel function declaration
│   └── compat.h          # Cross-platform sleep macro
├── dxe/
│   ├── dxe.cpp           # DXE driver initialization stubs
│   └── dxe.h             # DXE declarations
├── filesystem/
│   ├── bin/penguin       # Placeholder /bin/penguin
│   └── proc/
│       ├── cpuinfo.cpp   # Simulated /proc/cpuinfo
│       └── gpuinfo.cpp   # Simulated /proc/gpuinfo
├── system/hardware/
│   └── system.cpp        # Hardware detection stubs
├── arch-uefi/
│   ├── main.c            # Standalone GRUB-like bootloader
│   ├── arch.cpp          # Incomplete Arch shell
│   └── arch.h            # Empty header
├── extras/               # Miscellaneous text files
├── bin/                  # Build artifacts
├── compile.sh            # Compile utility objects
├── quick.sh              # Quick build + run (Linux)
├── run.sh                # Full build + run (Linux)
├── run.bat               # Build + run (Windows)
└── .github/workflows/    # GitHub Actions CI
```

## License

AGPL-3.0
