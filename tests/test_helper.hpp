#pragma once
#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>

namespace fs = std::filesystem;

inline std::string makeTempPath(const std::string& name) {
    auto p = fs::temp_directory_path();
    auto time = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
    return (p / ("test_" + time + "_" + name)).string();
}

inline void writeFile(const std::string& path, const std::string& content) {
    std::ofstream out(path, std::ios::binary);

    ASSERT_TRUE(out.is_open()) << "Невозможно создать файл: " << path;
    out.write(content.data(), static_cast<std::streamsize>(content.size()));

    out.close();
}

inline std::vector<std::string> readAllLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream in(path);
    std::string line;

    while (std::getline(in, line)) lines.push_back(line);
    return lines;
}