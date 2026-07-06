#include <iostream>
void disk_free(){
    std::cout << "Filesystem |     Path     | Size | Used | Avail \n";
    std::cout << "-----------|--------------|------|------|-------\n";
    std::cout << "ext4       |/dev/nvme0n1p6| 8TB  | 1TB  |  64TB \n";
    std::cout << "ext4       |/dev/nvme0n1p6| 8TB  | 3TB  |  64TB \n";
    std::cout << "esp        |/dev/nvmeon1p5|  1G  | 221M |  779M \n";
}