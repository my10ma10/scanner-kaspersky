#pragma once
#include <fstream>
#include <sstream>
#include <optional>

class CSVParser {
    std::ifstream file;
    unsigned int counter;

    void openFile();

public:
    CSVParser();
    
    std::optional<std::string> findMalicious(
        const std::string& filename, 
        const std::string& targetHash
    );

    unsigned int getCoincidencesCount() const; 
};