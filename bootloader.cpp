#include "kernel/kernel.h"
#include <chrono>
#include <iostream>
#include <thread>
int main() {
  std::string bootloader_name = "Talon Bootloader\n";
  int bootloader_option;
  std::cout << bootloader_name;
  std::cout << "Boot Option : [1] Talon Linux\n";
  std::cout << "Enter Boot Option : ";
  std::cin >> bootloader_option;
  if (bootloader_option == 1) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    kernel();
  }
}
