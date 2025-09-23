#include "traversal.hpp"
#include "md5.hpp"
#include "logger.hpp"
#include "csv_parser.hpp"

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

    // Logger::getInstance("../lg.log").createLog("First log message!");

    std::string filepath = "../base.csv";

    CSVParser parser(filepath);
    auto res = parser.findMalicious("ac6204ffeb36d2320e52f1d551cfa370");

    if (!res) {
        std::cout << "Not find the hash in the " << filepath << std::endl;
    }
    else {
        std::cout << "Found line: " << *res << std::endl;
    }
    
    return 0;
}