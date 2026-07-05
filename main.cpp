#include <string>
#include "kernel/kernel.h"
#include "system/free_h.h"
#include "driver_registry.h"
#include <iostream>
int main() {
    std::string shell;
    std::cout << "Welcome To Talon Linux";
    while (true){
    std::cout << "admin@talon ~ #";
    std::getline(std::cin,shell);
    }
     return 0;
     }
