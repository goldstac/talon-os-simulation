#include "bootloader.h"
#include "driver_registry.h"
#include "filesystem/proc/cpuinfo.h"
#include "filesystem/proc/gpuinfo.h"
#include "kernel/kernel.h"
#include "logo.h"
#include "uefi.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
std::string home_dir = "filesystem/home/admin/";
std::string desktop_dir = "filesystem/home/admin/Desktop/";
std::string cache_dir = "filesystem/home/admin/.cache/";
#ifdef _WIN32
const std::string PLATFORM = "windows.exe";
#elif __APPLE__
const std::string PLATFORM = "macos";
#else
const std::string PLATFORM = "linux";
#endif
const std::string VERSION = "1.0.7";

std::string read_file(const std::string &filepath) {
  std::ifstream file(filepath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
void write_file(const std::string &filepath, const std::string &content) {
  std::ofstream file(filepath, std::ios::app);
  file << content << '\n';
}
void create_file(const std::string& filepath) {
    std::ofstream file(filepath);
}
std::map<std::string, std::string> kernel_memory_locate = {
    {"wlan", "0Xffffffff81812"},
};
std::map<std::string, std::string> locate_bin = {
    {"penguinfetch", "/bin/penguinfetch"},
    {"bash", "/bin/bash"},
    {"zsh", "/bin/zsh"},
    {"calculator","/bin/calculator"}
};
int main(int argc, char* argv[]) {
  bootloader(argc, argv);
  std::cin.ignore();
  int first_number = 0;
  int second_number = 0;
  std::string shell;
  std::string iwd;
  std::cout << "Welcome To Talon Linux\n";
  register_device("keyboard_driver");
  while (true) {
    std::cout << "root@talon [Desktop] >> [%]\n> ";
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
      std::cout << "root\n";
    }
    //
    else if (shell == "pacman -Syu") {
      std::cout << "Updating Core  ###\n";
      std::cout << "Updating Extra ###\n";
      std::cout << "Updating Multilib ###\n";
    } else if (shell == "free -h") {
      std::cout << "Memory| Free | Using\n";
      std::cout << "------|------|-------\n";
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
    } else if (shell == "echo $SHELL") {
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
      continue;
    } else if (shell == "calculator") {
      std::cout << "-----------------------\n";
      std::cout << "   Talon Calculator\n";
      std::cout << "-----------------------\n";
      std::cout << "[Calculator] Enter First Number >> ";
      std::cin >> first_number;
      std::cout << "[Calculator] Enter Second Number >> ";
      std::cin >> second_number;
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid input\n";
      } else {
        std::cout << "Answer == " << first_number + second_number << "\n";
      }
      std::cin.ignore();
      continue;
    } else if (shell == "update") {
      std::cout << "Checking for updates...\n";
      std::string api_cmd =
          "curl -s "
          "https://api.github.com/repos/goldstac/talon-os-simulation/releases/"
          "latest | grep 'tag_name' | cut -d'\"' -f4";
      FILE *fp = popen(api_cmd.c_str(), "r");
      char buf[64];
      if (fp && fgets(buf, sizeof(buf), fp)) {
        pclose(fp);
        std::string latest(buf);
        if (!latest.empty() && latest.back() == '\n')
          latest.pop_back();
        if (latest == "v" + VERSION) {
          std::cout << "Already up to date (v" << VERSION << ")\n";
        } else {
          std::cout << "Updating from v" << VERSION << " to " << latest
                    << "...\n";
          std::string dl = "curl -L "
                           "https://github.com/goldstac/talon-os-simulation/"
                           "releases/latest/download/talon-os-";
          dl += PLATFORM + " -o talon-os-" + PLATFORM + ".new";
          std::system(dl.c_str());
#ifndef _WIN32
          std::system(("chmod +x talon-os-" + PLATFORM +
                       ".new && mv talon-os-" + PLATFORM + ".new talon-os-" +
                       PLATFORM)
                          .c_str());
          std::cout << "Update applied. Type 'exit' and run ./talon-os-"
                    << PLATFORM << " again.\n";
#else
          std::cout << "Update downloaded to talon-os-" << PLATFORM
                    << ".new. Restart to use the new version.\n";
#endif
        }
      } else {
        if (fp)
          pclose(fp);
        std::cout
            << "Failed to check for updates. Check your internet connection.\n";
      }
      continue;
    } else if (shell.substr(0, 5) == "touch") {
      if (shell.length() > 6) {
        std::string touch_strip = shell.substr(6);
        std::string touch_command = "touch " + desktop_dir + touch_strip;
        std::system(touch_command.c_str());
      } else {
        std::cout << "[touch] >> add the file name dude\n";
      }
    } else if (shell.substr(0, 3) == "cat") {
      if (shell.length() > 4) {
        std::string cat_strip = shell.substr(4);
        std::string cat_target = desktop_dir + cat_strip;
        std::string cat_output = read_file(cat_target);
        std::cout << cat_output << "\n";
      } else {
        std::cout << "add a space lol";
      }
    } else if (shell.substr(0, 2) == "ls") {
      std::system("ls filesystem/home/admin/Desktop> filesystem/home/admin/.cache/ls.txt");
      std::string ls_output = read_file("filesystem/home/admin/.cache/ls.txt");
      std::cout << ls_output << "\n";
    } 
    else if (shell.substr(0,4) == "echo"){
    if (shell.length() > 5){
      std::cout << shell.substr(5) << "\n";
    }
    }
    else if (shell == "--version") {
      std::cout << "Talon Linux v" << VERSION << "\n";
    } else {
      std::cout << "Command Not Found\n";
      continue;
    }
  }
}