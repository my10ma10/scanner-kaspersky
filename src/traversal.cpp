#include "traversal.hpp"

void Travelsal::operator()() const {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(_path)) {
            std::cout << entry.path() << std::endl;
        }
    }
    catch (const fs::filesystem_error& err) {
        std::cout << err.what() << std::endl;
    }
}