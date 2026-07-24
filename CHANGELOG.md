# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

## [0.1.0] - 2026-07-24

### Added

- Interactive shell REPL with commands: `reboot uefi`, `penguinfetch`, `cat`, `pacman -Syu`, `free -h`, `df -h`, `iwctl`, `root locate`, `which`, `whoami`, `$SHELL`, `clear`, `calculator`, `exit`, `shutdown`
- Simulated bootloader with boot option selection screen
- Linux kernel boot sequence with driver loading messages
- UEFI firmware setup screen with boot order, Secure Boot, XMP, and TPM
- Driver Execution Environment (DXE) with 17 device driver stubs
- Driver registry system
- NVRAM simulation for secure boot state, boot entry, and TPM
- SEC (Security) phase stub
- Initramfs simulation
- Virtual proc filesystem (`/proc/cpuinfo`, `/proc/gpuinfo`) with realistic hardware specs
- Hardware detection stubs (CPU, RAM, partition scheme)
- RAM communication simulation
- ASCII art Talon Linux logo
- Arch Linux UEFI boot simulation (experimental)
- Flash boot configuration
- Build scripts for Linux, Windows, and macOS
- CI/CD pipeline with GitHub Actions
- Pre-built binaries for all platforms

[Unreleased]: https://github.com/goldstac/talon-os-simulation/compare/v0.1.0...HEAD
[v0.1.0]: https://github.com/goldstac/talon-os-simulation/releases/tag/v0.1.0
