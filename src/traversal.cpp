#include "traversal.hpp"

Traversal::Traversal() 
    : Traversal(fs::current_path())  
    {}

Traversal::Traversal(const fs::path& p) 
    : path(p), processedCounter(0)
    {}

void Traversal::run(ThreadSafeQueue<fs::path>& queue) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            ++processedCounter;
            if (entry.is_regular_file()) {
                queue.push(entry.path());
            }
        }
    }
    catch (const fs::filesystem_error& err) {
        std::cout << err.what() << std::endl;
    }
}

unsigned int Traversal::getProcessedCount() const {
    return processedCounter;
}
