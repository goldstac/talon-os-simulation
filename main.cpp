#include <string>
#include "kernel/kernel.h"
#include "system/free_h.h"
#include "driver_registry.h"
#include <iostream>
int main() {
    std::string shell;
    std::cout << "Welcome To Talon Linux\n";
    while (true){
    std::cout << "admin@talon ~ #\n";
    std::getline(std::cin,shell);
    if (shell == "exit" or "shutdown"){
     break;
     std::cout << "exiting\n";

    }
     return 0;
     }
