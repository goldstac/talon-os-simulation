#!/bin/bash
// this is for developer testing not for actual use
g++ -o main.exe main.cpp driver_registry.cpp kernel/kernel.c filesystem/proc/cpuinfo.cpp filesystem/proc/gpuinfo.cpp uefi.cpp logo.cpp bootloader.cpp
./main.exe
