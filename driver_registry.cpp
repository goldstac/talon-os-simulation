#include <iostream>
#include <string>
#include "driver_registry.h"
void register_device(std::string device_name){
    std::cout << "[DRIVER] " << device_name << " Connected\n";
}
