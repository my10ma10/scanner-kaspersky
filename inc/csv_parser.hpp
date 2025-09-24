#pragma once
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

class CSVParser {
    std::ifstream file;
    unsigned int counter;


public:
    CSVParser();
    CSVParser(const std::string& basePath);
    
    std::optional<std::string> findMalicious(
        const std::string& targetHash
    );

    bool openBaseFile(const std::string& basePath);

    unsigned int getCoincidencesCount() const; 
};