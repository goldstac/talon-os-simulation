@echo off
setlocal enabledelayedexpansion

echo Compiling system objects...
g++ -c system/free-h.cpp -o bin/free-h.o
g++ -c system/df-h.cpp -o bin/df-h.o

echo Compiling Main ^& Kernel...
for /f "usebackq skip=2 tokens=3,*" %%A in ("quick.sh") do set QUICK_SRC=%%B

g++ -o main %QUICK_SRC% bin/free-h.o bin/df-h.o

echo Running Main...
main
