#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>

class Logger {
    std::ofstream file;
    std::unique_ptr<Logger> instance; 
    std::mutex mtx;

public:
    Logger() = default;
    void setLogFile(const std::string& filepath);

    bool createLog(
        const std::string& filePath,
        const std::string& hash,
        const std::string& verdict
    );
    
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
};

