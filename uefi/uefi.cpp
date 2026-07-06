#include <iostream>
#include <string>
#include "uefi.h"
using namespace std;
void uefi(){
std::string uefi_user_input;
    bool boot_priority_talon = false;
    cout << "Booting To UEFI\n";
    cout << "Welcome TO Unified Extensible Firmware Interface";
cout << "...";
cout << " Unified Extensible Firmware Interface";
cout << "-----------------------------------------";
cout << "|Boot Order    |  Other                 |";
cout<< "----------------|------------------------|";
cout << "| 1| Talon     | Secure Boot | Disabled |";
cout << "| 2| Arch      |   XMP       | Enabled  |";
cout << "| 3| USB       |   TPM       | Enabled  |"; 
cout << "|---------------------------------------|";
cout << "| NVRAM IS LOCATED AT ~/nvram/nvram.cpp |";
cout << "|---------------------------------------|";


}