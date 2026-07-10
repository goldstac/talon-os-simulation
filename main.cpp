#include <string>
#include "kernel/kernel.h"
#include "system/free_h.h"
#include "driver_registry.h"
#include "system/df-h.h"
#include <string>
#include <iostream>
int main() {
    kernel();
    std::string shell;
    std::cout << "Welcome To Talon Linux\n";
    register_device("keyboard_driver");
    while (true){
    std::cout << "admin@talon ~ # ";
    std::getline(std::cin,shell);
    if (shell == "exit" || shell == "shutdown"){
     std::cout << "exiting\n";
        break;
     

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
    else if (shell == ""){
        std::cout << ""
    }
    else if (shell == "free -h") {
     std::cout << "Memory| Free | Using\n";
     std::cout << "------|------|-------";
     std::cout << "96 GB | 93GB | 3 GB  \n";
     continue;
    }
    else if (shell == "df -h"){
    std::cout << "Filesystem |     Path     | Size | Used | Avail \n";
    std::cout << "-----------|--------------|------|------|-------\n";
    std::cout << "ext4       |/dev/nvme0n1p6| 8TB  | 1TB  |  6TB  \n";
    std::cout << "ext4       |/dev/nvme0n1p7| 8TB  | 200G | 7.8TB \n";
    std::cout << "esp        |/dev/nvmeon1p5|  1G  | 221M |  779M \n";
    continue;
    }
    else{
     std::cout << "Command Not Found\n";
     continue;
    }
     return 0;
     }
}
