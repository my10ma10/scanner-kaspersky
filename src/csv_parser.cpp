#include "csv_parser.hpp"
#include <iostream>

CSVParser::CSVParser()
    : counter(0)
    {}

std::optional<std::string> CSVParser::findMalicious(
    const std::string& filename, 
    const std::string& targetHash
) {
    std::string line;
    unsigned int counter = 0;

    openFile();

    while (std::getline(file, line)) {
        ++counter;

        if (line.find(targetHash) != std::string::npos) {
            return line;
        }
    }
    if (file.is_open()) file.close();
    return std::nullopt;
}

void CSVParser::openFile() {
    
}

unsigned int CSVParser::getCoincidencesCount() const {
    return counter;
}
