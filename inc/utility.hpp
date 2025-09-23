#pragma once
#include "traversal.hpp"
#include "logger.hpp"
#include "csv_parser.hpp"


class Utility {
    Travelsal travelsal;
    CSVParser parser;
public:
    Utility(const std::string& path, const std::string& base, const std::string& logfile);

    void execute();

    void printReport() const;

};