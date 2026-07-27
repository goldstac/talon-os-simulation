#!/bin/bash
// this is for developer testing not for actual use
g++ -o main.exe -std=c++17 main.cpp driver_registry.cpp kernel/kernel.c filesystem/proc/cpuinfo.cpp filesystem/proc/gpuinfo.cpp uefi.cpp logo.cpp bootloader.cpp make_system_dir.cpp kernel/initramfs.cpp
./main.exe
