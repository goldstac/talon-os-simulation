// lol im not good at batchfiles lol 
// for windows ppl :)
echo "Compiling"
g++ -o main.exe main.cpp kernel/kernel.c driver_registry.cpp filesystem/proc/cpuinfo.cpp
echo "Running"
./main.exe

