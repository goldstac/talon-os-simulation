#!/bin/bash
# runs make file
echo "Running Compile.sh"
chmod +x compile.sh
./compile.sh
# compiles main and kernel
echo "Compiling Main & Kernel"
g++ -o main main.cpp kernel/kernel.c system/free-h.cpp driver_registry.cpp
# run main
echo "Running Main"
./main
