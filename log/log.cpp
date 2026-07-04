#include "log.h"

Logger::Logger(const std::string& filename, bool isSilent) 
    : silent(isSilent) {
    logFile.open(filename, std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::write(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }

    if (!silent) {
        std::cout << message << std::endl;
    }
}

Logger globalLogger("../logs.txt", false);

void log(const std::string& message) {
    globalLogger.write(message);
}