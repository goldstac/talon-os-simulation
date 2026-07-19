#include "kernel/kernel.h"
#include <iostream>
int main() {
  std::string bootloader_name = "Talon Bootloader\n";
  int bootloader_option;
  std::cout << bootloader_name;
  std::cout << "Boot Option : [1] Talon Linux\n";
  std::cout << "Enter Boot Option : ";
  std::cin >> bootloader_option;
  if (bootloader_option == 1) {
    kernel();
  }
}
