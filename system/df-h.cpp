#include <iostream>
void disk_free(){
    std::cout << "Filesystem |     Path     | Size | Used | Avail \n";
    std::cout << "-----------|--------------|------|------|-------\n";
    std::cout << "ext4       |/dev/nvme0n1p6| 8TB  | 1TB  |  6TB  \n";
    std::cout << "ext4       |/dev/nvme0n1p7| 8TB  | 200G | 7.8TB \n";
    std::cout << "esp        |/dev/nvmeon1p5|  1G  | 221M |  779M \n";
}