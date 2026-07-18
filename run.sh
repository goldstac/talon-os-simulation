#!/bin/bash
set -e

echo "Compiling system objects..."
g++ -c system/free-h.cpp -o bin/free-h.o
g++ -c system/df-h.cpp -o bin/df-h.o

echo "Compiling Main & Kernel..."
g++ -o main main.cpp kernel/kernel.c system/free-h.cpp driver_registry.cpp

echo "Running Main..."
./main
