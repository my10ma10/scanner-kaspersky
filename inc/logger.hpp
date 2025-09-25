#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>

class Logger {
    static std::ofstream file;
    static std::unique_ptr<Logger> instance; 
    static std::mutex mtx;

    Logger() = default;
public:
    static Logger& getInstance();

    static void setLogFile(const std::string& filepath);

    bool createLog(
        const std::string& filePath,
        const std::string& hash,
        const std::string& verdict
    );
    
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
};

