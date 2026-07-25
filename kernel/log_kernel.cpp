#include "log_kernel.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
void write_file(const std::string &filepath, const std::string &content) {
  std::ofstream file(filepath, std::ios::app);
  file << content << '\n';
}
int main() {}
