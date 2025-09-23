#include "traversal.hpp"

Travelsal::Travelsal() 
    : Travelsal(fs::current_path())  
    {}

Travelsal::Travelsal(const fs::path& p) 
    : path(p), processedCounter(0)
    {}

void Travelsal::execute() {
    try {
        for (const fs::path& entry : fs::recursive_directory_iterator(path)) {
            ++processedCounter;
            auto fileHash = md5Calc.calculate(entry.string());
        }
    }
    catch (const fs::filesystem_error& err) {
        std::cout << err.what() << std::endl;
    }
}

unsigned int Travelsal::getProcessedCount() const {
    return processedCounter;
}

unsigned int Travelsal::getReadErrorsCount() const {
    return md5Calc.getReadErrorsCount();
}
