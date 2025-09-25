#include <iostream>
#include <tuple>

#include "utility.hpp"

void argsChecking(int argc, char** argv);

std::tuple<std::string, std::string, std::string> 
flagsProcessing(int argc, char** argv);


int main(int argc, char** argv) {
    try {
        argsChecking(argc, argv);
        auto [path, base, log] = flagsProcessing(argc, argv);
        
        Utility utility(path, base, log);
        utility.run();
        utility.printReport();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}

void argsChecking(int argc, char** argv) {
    if (argc < 7) {
        throw std::invalid_argument("Недостаточно аргументов! Ожидалось:\n\
            scanner.exe --base [hash-base] --log [log-file] --path [scanning-path]");
    }
}

std::tuple<std::string, std::string, std::string> 
    flagsProcessing(int argc, char** argv) 
{
    std::tuple<std::string, std::string, std::string> res{"", "", ""};

    for (size_t i = 1; i < argc; i += 2) {
        std::string arg = argv[i];

        if (arg == "--path") {
            std::get<0>(res) = argv[i+1];
        }
        else if (arg == "--base") {
            std::get<1>(res) = argv[i+1];
        }
        else if (arg == "--log") {
            std::get<2>(res) = argv[i+1];
        }
    }
    return res;
}