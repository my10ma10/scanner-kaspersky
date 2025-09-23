#pragma once
#include <filesystem>
#include <iostream>

#include "md5.hpp"

namespace fs = std::filesystem;

class Travelsal {
    MD5Calculator md5Calc;

    fs::path path;
    unsigned int processedCounter;

public:
    Travelsal();
    Travelsal(const fs::path& p);

    void execute();

    unsigned int getProcessedCount() const;
    unsigned int getReadErrorsCount() const;
};

