#include "uefi.h"
#include <iostream>
#include <string>
using namespace std;
void uefi() {
  std::string uefi_user_input;
  bool boot_priority_talon = false;
  cout << "Booting To UEFI\n";
  cout << "Welcome TO Unified Extensible Firmware Interface\n";
  cout << "...\n";
  cout << " Unified Extensible Firmware Interface\n";
  cout << "-----------------------------------------\n";
  cout << "|Boot Order    |  Other                 |\n";
  cout << "|--------------|------------------------|\n";
  cout << "| 1| Talon     | Secure Boot | Disabled |\n";
  cout << "| 2| Arch      |   XMP       | Enabled  |\n";
  cout << "| 3| USB       |   TPM       | Enabled  |\n";
  cout << "|---------------------------------------|\n";
  cout << "| NVRAM IS LOCATED AT ~/nvram/nvram.cpp |\n";
  cout << "|---------------------------------------|\n";
}
