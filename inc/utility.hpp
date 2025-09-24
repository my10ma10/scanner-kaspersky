#pragma once
#include "traversal.hpp"
#include "logger.hpp"
#include "csv_parser.hpp"
#include "md5.hpp"
#include "timer.hpp"
#include "threadsafe_queue.hpp"

class Utility {
    Traversal travelsal;
    CSVParser parser;
    MD5Calculator md5Calc;
    ThreadSafeQueue<fs::path> queue;

    Timer timer;
public:
    Utility(const std::string& path, const std::string& base, const std::string& logfile);

    void run();

    void printReport() const;

};