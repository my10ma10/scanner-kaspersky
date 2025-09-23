#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Travelsal {
    fs::path path;

public:
    Travelsal();
    Travelsal(const fs::path& p);

    void operator()() const;
};

