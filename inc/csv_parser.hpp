#include <fstream>
#include <sstream>
#include <optional>

class CSVParser {
    std::ifstream file;
public:
    CSVParser(const std::string& filename);
    
    std::optional<std::string> findMalicious(const std::string& targetHash);
};