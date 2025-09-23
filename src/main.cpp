#include "traversal.hpp"
#include "md5.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
    // Travelsal t(fs::path("/mnt/c/Dev/Personal/cpp/Projects/kaspersky/scanner/src"));
    // t();

    // try {
    //     MD5Calculator calc("/mnt/c/Dev/Personal/cpp/Projects/kaspersky/scanner/base.csv");
    //     std::string hash = calc.calculate();
    //     std::cout << "MD5: " << hash << "\n";
    // } 
    // catch (const std::exception& e) {
    //     std::cerr << e.what() << "\n";
    // }

    Logger::getInstance("../lg.log").createLog("First log message!");
    
    return 0;
}