#include <iostream>
#include <string>
#include "gpuinfo.h"
int main(){

    std::string gpu_name = "Nvidia GeForce RTX 5090";
    std::string vram = "32 GB";
    std::string memory_type = "GDDR7";
    int cuda_cores = 21760;
    std::string architecture = "Blackwell";
    std::string interface = "PCIE 5.0 X16 Slot";
    std::string memory_bus = "512-Bit";
    std::string bandwith = "1.79 TB/S";
    std::cout << "----------------------------------------";
    std::cout << "          GPU INFO";
    std::cout << "----------------------------------------";
    std::cout << "Name              |" << gpu_name << "\n";
    std::cout << "VRAM              |" << vram << "\n";
    std::cout << "CUDA Cores        |" << cuda_cores << "\n";
    std::cout << "RAM Type          |" << memory_type << "\n";
    std::cout << "Architecture      |" << architecture << "\n";
    std::cout << "Interface         |" << interface << "\n";
    std::cout << "Memory Bus        |" << memory_bus << "\n";
    std::cout << "Memory Bandwith   |" << bandwith << "\n";
}