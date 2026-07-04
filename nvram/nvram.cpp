#include <iostream>
#include <string>
#include <fstream>

bool IS_SIMULATION_SILENT = true;
std::ofstream logFile("logs.txt", std::ios::app);

void log(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }

    if (!IS_SIMULATION_SILENT) {
        std::cout << message << std::endl;
    }
}

int nvram() {
    log("[UEFI] Initializing NVRAM...");

    
    logFile << "[DATA] Key: BootOrder, Value: 0001" << std::endl;
    logFile << "[DATA] NVRAM size: 1024 bytes" << std::endl;
    
    return 0;
}