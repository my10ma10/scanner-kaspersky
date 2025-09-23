#include "traversal.hpp"
#include "md5.hpp"

int main(int argc, char** argv) {
    // Travelsal t(fs::path("/mnt/c/Dev/Personal/cpp/Projects/kaspersky/scanner/src"));
    // t();

    try {
        MD5Calculator calc("/mnt/c/Dev/Personal/cpp/Projects/kaspersky/scanner/base.csv");
        std::string hash = calc.calculate();
        std::cout << "MD5: " << hash << "\n";
    } 
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
    
    return 0;
}