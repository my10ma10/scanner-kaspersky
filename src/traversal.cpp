#include "traversal.hpp"

Travelsal::Travelsal() : path(fs::current_path()) {}
Travelsal::Travelsal(const fs::path& p) : path(p) {}

void Travelsal::operator()() const {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            std::cout << entry.path() << std::endl;
        }
    }
    catch (const fs::filesystem_error& err) {
        std::cout << err.what() << std::endl;
    }
}