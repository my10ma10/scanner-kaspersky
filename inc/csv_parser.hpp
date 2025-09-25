#pragma once
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <unordered_map>

#include "logger.hpp"

class CSVParser {
    std::ifstream file;
    unsigned int counter;
    std::unordered_multimap<std::string, std::string> hashBase;

public:
    CSVParser();
    CSVParser(const std::string& basePath);

    void init(const std::string& basePath);
    
    std::optional<std::string> findMalicious(
        const std::string& targetHash,
        const std::string& filePath
    );

    bool openBaseFile(const std::string& basePath);
    void fillBase();

    unsigned int getCoincidencesCount() const; 
};