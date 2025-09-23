#include <iostream>
#include <fstream>


class Logger {
    std::ofstream file;

    Logger(const std::string& filepath);
public:
    static Logger& getInstance(const std::string& filepath);

    bool createLog(const std::string& message);
};