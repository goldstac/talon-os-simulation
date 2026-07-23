# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added
- Initial project structure with UEFI boot, kernel, and OS simulation components
- DXE (Driver Execution Environment) framework with driver registry
- Bootloader with UEFI support (`arch-uefi/`)
- Hardware simulation layer (RAM, system, NVRAM)
- Filesystem simulation with `/proc` entries (cpuinfo, gpuinfo)
- Driver management system (`driver_registry.cpp`)
- CI/CD workflows (`opencode.yml`, `release.yml`)
- Build system (`compile.sh`, `quick.sh`, `run.sh`, `run.bat`)
- OpenCode agent for automated changelog updates

### Changed

### Fixed

### Removed

<!--

Comparison links use this format:
[Unreleased]: https://github.com/goldstac/talon-os-simulation/compare/v0.1.0...HEAD
[v0.1.0]: https://github.com/goldstac/talon-os-simulation/releases/tag/v0.1.0

-->
