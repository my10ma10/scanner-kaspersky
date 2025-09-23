#include "csv_parser.hpp"
#include <iostream>

CSVParser::CSVParser(const std::string& filename) 
    : file(filename, std::ios::in)
    {}

std::optional<std::string> CSVParser::findMalicious(const std::string& targetHash) {
    std::string line;
    unsigned int counter = 0;
    while (std::getline(file, line)) {
        ++counter;

        if (line.find(targetHash) != std::string::npos) {
            return line;
        }
    }
    return std::nullopt;
}
