#!/bin/bash
g++ -o main.exe main.cpp driver_registry.cpp kernel/kernel.c filesystem/proc/cpuinfo.cpp
./main.exe
