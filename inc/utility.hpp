#pragma once
#include <mutex>

#include "traversal.hpp"
#include "logger.hpp"
#include "csv_parser.hpp"
#include "md5.hpp"
#include "timer.hpp"
#include "threadsafe_queue.hpp"
#include "thread_pool.hpp"

#ifdef _WIN32
    #include "windows.h"
    #ifdef BUILD_MYLIB
        #define MYLIB_API __declspec(dllexport)
    #else
        #define MYLIB_API __declspec(dllimport)
    #endif
#else
  #define MYLIB_API
#endif

class MYLIB_API Utility {
    Traversal traversal;
    CSVParser parser;
    MD5Calculator md5Calc;
    ThreadSafeQueue<fs::path> queue;
    ThreadPool thPool;
    Timer timer;

    std::mutex mtx;

    void createWorkers(unsigned int n);
public:
    Utility(const std::string& path, const std::string& base, const std::string& logfile);

    void run();

    void printReport() const;

};