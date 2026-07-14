#include <iostream>
#include <string> 

int main() {
    std::string cpu_model = "Intel Core Ultra 9 285K";
    int cpu_cores = 24;
    int cpu_threads = 24;
    std::string l2_cache = "40MB";
    std::string l3_cache = "36MB";
    std::string max_speed = "5.7 GHz";

    std::cout << "---------------------------------\n";
    std::cout << "            CPU INFO             \n";
    std::cout << "---------------------------------\n";
    std::cout << "Name      | " << cpu_model << "\n";
    std::cout << "Cores     | " << cpu_cores << "\n";
    std::cout << "Threads   | " << cpu_threads << "\n";
    std::cout << "L2 Cache  | " << l2_cache << "\n";
    std::cout << "L3 Cache  | " << l3_cache << "\n";
    std::cout << "Max Speed | " << max_speed << "\n"; 
}