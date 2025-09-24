#include "utility.hpp"
#include <iostream>

int main(int argc, char** argv) {
    // if (argc < 8) {
    //     std::cout << "Not enough arguments! Expected:\n\
    //         scanner.exe --base [hash-base] --log [log-file] --path [scanning-path]";
    // }
    try {
        Utility utility("../", "../base.csv", "../logfile.log");
        utility.run();
        utility.printReport();
    }
    catch(std::exception ex) {
        std::cout << ex.what() << std::endl;
    }
    
    // Traversal t(fs::path("/mnt/c/Dev/Personal/cpp/Projects/kaspersky/scanner/src"));
    // t();

    // try {
    //     MD5Calculator calc;
    //     std::string hash = calc.calculate("../src/utility.cpp");
    //     std::cout << "MD5: " << hash << "\n";
    // } 
    // catch (const std::exception& e) {
    //     std::cerr << e.what() << "\n";
    // }

    // Logger::getInstance("../lg.log").createLog("First log message!");

    // std::string filepath = "../base.csv";

    // CSVParser parser(filepath);
    // auto res = parser.findMalicious("ac6204ffeb36d2320e52f1d551cfa370");

    // if (!res) {
    //     std::cout << "Not find the hash in the " << filepath << std::endl;
    // }
    // else {
    //     std::cout << "Found line: " << *res << std::endl;
    // }
    
    return 0;
}