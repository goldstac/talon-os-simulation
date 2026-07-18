#!/bin/bash
set -e

echo "Compiling system objects..."
g++ -c system/free-h.cpp -o bin/free-h.o
g++ -c system/df-h.cpp -o bin/df-h.o

QUICK_SRC=$(sed -n '3p' quick.sh | sed 's/^g++ -o [^ ]* //')

echo "Compiling Main & Kernel..."
g++ -o main $QUICK_SRC bin/free-h.o bin/df-h.o

echo "Running Main..."
./main
