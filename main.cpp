#include <string>
#include "kernel/kernel.h"
#include "system/free_h.h"
#include "driver_registry.h"
#include <iostream>
int main() {
    std::string shell;
    std::cout << "Welcome To Talon Linux\n";
    while (true){
    std::cout << "admin@talon ~ # ";
    std::getline(std::cin,shell);
    if (shell == "exit" || shell == "shutdown"){
     break;
     std::cout << "exiting\n";

    }
    else if (shell == ""){
     continue;
    }
    else if (shell == "whoami") {
     std::cout << "admin";
    }
    else if (shell == "drivers-list") {
     std::cout << "";
    }
    else if (shell == "free -h") {
     show_memory();
     continue;
    }
    else{
     std::cout << "Command Not Found\n";
     continue;
    }
     return 0;
     }
}
