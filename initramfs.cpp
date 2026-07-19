#include <chrono>
#include <iostream>
#include <thread>
int main() {
  std::cout << "[initramfs] starting..";
  std::this_thread::sleep_for(std::chrono::seconds(2));
}
