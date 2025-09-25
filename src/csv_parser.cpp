#include "csv_parser.hpp"

CSVParser::CSVParser()
    : counter(0)
{}

CSVParser::CSVParser(const std::string& basePath) 
    : counter(0)
{
    init(basePath);
}

void CSVParser::init(const std::string &basePath) {
    openBaseFile(basePath);
    fillBase();
}

std::optional<std::string> CSVParser::findMalicious(
    const std::string& targetHash,
    const std::string& filePath
    ) 
{    
    // first - хэш, second - вердикт 
    auto it = hashBase.find(targetHash); 
    
    if (it != hashBase.end()) {
        ++counter;
        Logger::getInstance().createLog(
            filePath, 
            it->first,
            it->second
        );
        return it->first;
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

void CSVParser::fillBase() {
    std::string hash;
    std::string msg;
    while (std::getline(file, hash, ';') && std::getline(file, msg)) {
        hashBase.insert(std::make_pair(hash, msg));
    }
}

unsigned int CSVParser::getCoincidencesCount() const {
    return counter;
}
