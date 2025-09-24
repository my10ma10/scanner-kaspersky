#pragma once
#include <filesystem>
#include <iostream>

#include "threadsafe_queue.hpp"


namespace fs = std::filesystem;

class Traversal {
    fs::path path;
    unsigned int processedCounter;

public:
    Traversal();
    Traversal(const fs::path& p);

    void run(ThreadSafeQueue<fs::path>& queue);

    unsigned int getProcessedCount() const;
};

