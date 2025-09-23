#include "logger.hpp"

Logger::Logger(const std::string& filepath) 
    : file(filepath, std::ios::app)
    {}

Logger& Logger::getInstance(const std::string& filepath) {
    static Logger instance(filepath);
    return instance;
}

bool Logger::createLog(const std::string& message) {
    if (file.is_open()) {
        file << message << std::endl;
        return true;
    }
    return false;
}