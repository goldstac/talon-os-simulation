#include "bootloader.h"
#include "driver_registry.h"
#include "filesystem/proc/cpuinfo.h"
#include "filesystem/proc/gpuinfo.h"
#include "kernel/kernel.h"
#include "logo.h"
#include "uefi.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
std::map<std::string, std::string> kernel_memory_locate = {
    {"wlan", "0Xffffffff81812"},
};
std::map<std::string, std::string> locate_bin = {
    {"penguinfetch", "/bin/penguinfetch"},
    {"bash", "/bin/bash"},
    {"zsh", "/bin/zsh"},
};
int main() {
  bootloader();
  int first_number;
  int second_number;
  std::string shell;
  std::string iwd;
  std::cout << "Welcome To Talon Linux\n";
  register_device("keyboard_driver");
  while (true) {
    std::cout << "[root@talon] >> [#] ";
    std::getline(std::cin, shell);
    if (shell == "exit" || shell == "shutdown") {
      std::cout << "exiting\n";
      break;

    } else if (shell.substr(0, 11) == "root locate") {
      std::string target = shell.substr(12);
      if (kernel_memory_locate.count(target)) {
        std::cout << "[KERNEL] " << target << " "
                  << kernel_memory_locate[target] << "\n";
      }
      continue;
    } else if (shell.substr(0, 5) == "which") {
      std::string target_locate_bin = shell.substr(6);
      if (locate_bin.count(target_locate_bin)) {
        std::cout << "[KERNEL] " << target_locate_bin << " "
                  << locate_bin[target_locate_bin] << "\n";
      }
    } else if (shell == "") {
      continue;
    } else if (shell == "whoami") {
      std::cout << "root";
    }
    //
    else if (shell == "pacman -Syu") {
      std::cout << "Updating Core  ###";
      std::cout << "Updating Extra ###";
      std::cout << "";
      std::cout << "";
    } else if (shell == "free -h") {
      std::cout << "Memory| Free | Using\n";
      std::cout << "------|------|-------";
      std::cout << "96 GB | 93GB | 3 GB  \n";
      continue;
    } else if (shell == "df -h") {
      std::cout << "Filesystem |     Path     | Size | Used | Avail \n";
      std::cout << "-----------|--------------|------|------|-------\n";
      std::cout << "ext4       |/dev/nvme0n1p6| 8TB  | 1TB  |  6TB  \n";
      std::cout << "ext4       |/dev/nvme0n1p7| 8TB  | 200G | 7.8TB \n";
      std::cout << "esp        |/dev/nvmeon1p5|  1G  | 221M |  779M \n";
      continue;
    } else if (shell == "iwctl") {
      while (true) {
        std::cout << "[iwd #]  ";
        std::getline(std::cin, iwd);
        if (iwd == "device list") {
          std::cout << "wlan0\n";
          continue;
        } else if (iwd == "exit") {
          break;
        }
      }

    } else if (shell == "cat proc/cpuinfo") {
      cpuinfo();
    } else if (shell == "$SHELL") {
      std::cout << "Penguin\n";
    } else if (shell == "clear") {
#ifdef _WIN32
      std::system("cls");
#else
      std::system("clear");
#endif
    } else if (shell == "which penguin") {
      std::cout << "/bin/penguin\n";
    } else if (shell == "chsh -s $(which zsh)" ||
               shell == "chsh -s $(which bash)") {
      std::cout << "[Talon] No\n";
      std::cout << "[Kernel] No\n";
    } else if (shell == "cat proc/gpuinfo") {
      gpuinfo();
    } else if (shell == "reboot uefi") {
      uefi();
    } else if (shell == "penguinfetch") {
      printLogo();
    } else if (shell == "calculator") {
      std::cout << "-----------------------\n";
      std::cout << "   Talon Calculator\n";
      std::cout << "-----------------------\n";
      std::cout << "[Calculator] Enter First Number >> ";
      std::cin >> first_number;
      std::cout << "[Calculator] Enter Second Number >> ";
    } else {
      std::cout << "Command Not Found\n";
      continue;
    }
  }
}
