#!/bin/bash
MAKE
g++ -o main main.cpp kernel/kernel.c
./main