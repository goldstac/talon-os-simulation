#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <fstream>

class Logger {
private:
    std::ofstream logFile;
    bool silent;

public:
    Logger(const std::string& filename, bool isSilent);
    ~Logger();
    void write(const std::string& message);
};

extern Logger globalLogger;

void log(const std::string& message);

#endif