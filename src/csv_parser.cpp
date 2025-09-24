#include "csv_parser.hpp"

CSVParser::CSVParser()
    : counter(0)
{}

CSVParser::CSVParser(const std::string& basePath) 
    : counter(0)
{
    openBaseFile(basePath);
}

std::optional<std::string> CSVParser::findMalicious(
    const std::string& targetHash) 
{
    std::string line;
    unsigned int counter = 0;

    while (std::getline(file, line)) {
        ++counter;

        if (line.find(targetHash) != std::string::npos) {
            return line;
        }
    }
    if (file.is_open()) file.close();
    return std::nullopt;
}

bool CSVParser::openBaseFile(const std::string& basePath) {
    file.open(basePath);

    if (file.is_open()) {
        return true;
    }
    std::cout << "Ошибка при открытии файла " << basePath << std::endl; 
    return false;
}

unsigned int CSVParser::getCoincidencesCount() const {
    return counter;
}
