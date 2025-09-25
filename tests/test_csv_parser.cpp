#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "csv_parser.hpp"

namespace fs = std::filesystem;


class CSVParserTest : public ::testing::Test {
protected:
    std::string basePath;
    std::string logPath;

    void SetUp() override {}

    void TearDown() override {
        std::error_code ec;
        if (!basePath.empty() && fs::exists(basePath)) {
            fs::remove(basePath, ec);
        }
        if (!logPath.empty() && fs::exists(logPath)) {
            fs::remove(logPath, ec);
        }
    }

    std::string makeTempPath(const std::string& name) {
        auto p = fs::temp_directory_path();
        auto time = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
        return (p / ("csv_test_" + time + "_" + name)).string();
    }

    void writeFile(const std::string& path, const std::string& content) {
        std::ofstream out(path, std::ios::binary);

        ASSERT_TRUE(out.is_open()) << "Невозможно создать файл: " << path;
        out.write(content.data(), static_cast<std::streamsize>(content.size()));

        out.close();
    }

    std::vector<std::string> readAllLines(const std::string& path) {
        std::vector<std::string> lines;
        std::ifstream in(path);
        std::string line;

        while (std::getline(in, line)) lines.push_back(line);
        return lines;
    }
};

TEST_F(CSVParserTest, OpenBaseNonExistentFile) {
    basePath = makeTempPath("non-existent.csv");
    
    CSVParser parser;

    EXPECT_FALSE(parser.openBaseFile(basePath));
    EXPECT_EQ(parser.getCoincidencesCount(), 0u);
}

TEST_F(CSVParserTest, FillBaseAndFindMalicious) {
    basePath = makeTempPath("base.csv");
    logPath  = makeTempPath("log.log");

    const std::string baseContent = 
        "hash1;BAD\n"
        "hash2;OK\n";
    writeFile(basePath, baseContent);

    CSVParser parser;
    parser.init(basePath, logPath);

    auto res = parser.findMalicious("hash1", "some\\dir\\path\\file.txt");
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "hash1");
    EXPECT_EQ(parser.getCoincidencesCount(), 1u);

    // Проверка лога
    auto lines = readAllLines(logPath);
    ASSERT_EQ(lines.size(), 3u);

    EXPECT_NE(lines[0].find("some\\dir\\path\\file.txt"), std::string::npos);
    EXPECT_NE(lines[1].find("hash1"), std::string::npos);
    EXPECT_NE(lines[2].find("BAD"), std::string::npos);
}

TEST_F(CSVParserTest, FindingNonExistentHash) {
    basePath = makeTempPath("base2.csv");
    logPath  = makeTempPath("log2.txt");

    writeFile(basePath, "hsh1;VER\n");

    CSVParser parser;
    parser.init(basePath, logPath);

    auto res = parser.findMalicious("non-existent_hash", "file.bin");
    EXPECT_FALSE(res.has_value());
    EXPECT_EQ(parser.getCoincidencesCount(), 0u);

    auto lines = readAllLines(logPath);
    EXPECT_TRUE(lines.empty());
}

TEST_F(CSVParserTest, MultipleSameHash) {
    basePath = makeTempPath("base3.csv");
    logPath  = makeTempPath("log3.txt");

    // Две записи с одинаковым хэшем, разными вердиктами
    writeFile(basePath, 
        "hash;V1\n"
        "hash;V2\n"
    );

    CSVParser parser;
    parser.init(basePath, logPath);

    auto r1 = parser.findMalicious("hash", "f1");
    EXPECT_TRUE(r1.has_value());
    EXPECT_EQ(parser.getCoincidencesCount(), 1u);

    auto r2 = parser.findMalicious("hash", "f2");
    EXPECT_TRUE(r2.has_value());
    EXPECT_EQ(parser.getCoincidencesCount(), 2u);

    auto lines = readAllLines(logPath);
    ASSERT_EQ(lines.size(), 6u);
    EXPECT_NE(lines[0].find("f1"), std::string::npos);
    EXPECT_NE(lines[3].find("f2"), std::string::npos);
}

TEST_F(CSVParserTest, ConstructorInit) {
    basePath = makeTempPath("base4.csv");
    logPath  = makeTempPath("log4.txt");

    writeFile(basePath, "hash1;BAD\nhash2;OK\n");

    CSVParser parser(basePath, logPath);

    auto r = parser.findMalicious("hash2", "somefile");
    EXPECT_TRUE(r.has_value());
    EXPECT_EQ(parser.getCoincidencesCount(), 1u);

    auto lines = readAllLines(logPath);
    ASSERT_EQ(lines.size(), 3u);
    EXPECT_NE(lines[0].find("somefile"), std::string::npos);
}