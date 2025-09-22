#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Travelsal {
    fs::path _path;

public:
    Travelsal() : _path(fs::current_path()) {}
    Travelsal(const fs::path& path) : _path(path) {}

    void operator()() const;
};

