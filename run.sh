#!/bin/bash
# runs make file
echo "Running MAKE"
MAKE
# compiles main and kernel
echo "Compiling Main & Kernel"
g++ -o main main.cpp kernel/kernel.c
# run main
echo "Running Main"
./main
