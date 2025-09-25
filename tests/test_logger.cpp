#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>

#include "logger.hpp"

namespace fs = std::filesystem;

class LoggerTest : public ::testing::Test {
protected:
    Logger logger;
    fs::path logFilePath;

    void SetUp() override {
        logFilePath = fs::temp_directory_path() / "test_log.log";
    }
    
    void TearDown() override {
        if (fs::exists(logFilePath)) {
            fs::remove(logFilePath);
        }
    }

    std::vector<std::string> readAllLines() {
        std::ifstream in(logFilePath);
        std::vector<std::string> lines;
        std::string line;

        while (std::getline(in, line)) {
            lines.push_back(line);
        }
        return lines;
    }
};


TEST_F(LoggerTest, LogFileOpening) {
    Logger logger;
    logger.setLogFile(logFilePath.string());

    EXPECT_TRUE(fs::exists(logFilePath));
    EXPECT_NO_THROW(logger.setLogFile(logFilePath.string()));
}

TEST_F(LoggerTest, CreatingLogFile) {
    Logger logger;
    logger.setLogFile(logFilePath.string());

    EXPECT_TRUE(logger.createLog("file.txt", "somehash", "Verdict"));
    EXPECT_TRUE(fs::exists(logFilePath));

    std::ifstream in(logFilePath);
    std::string line;
    std::getline(in, line);
    EXPECT_FALSE(line.empty());
}

TEST_F(LoggerTest, CreateLogWritesLine) {
    Logger logger;
    logger.setLogFile(logFilePath.string());

    logger.createLog("file.txt", "somehash", "Verdict");

    auto lines = readAllLines();
    ASSERT_EQ(lines.size(), 3);
    EXPECT_NE(lines[0].find("file.txt"), std::string::npos);
    EXPECT_NE(lines[1].find("somehash"), std::string::npos);
    EXPECT_NE(lines[2].find("Verdict"), std::string::npos);
}

TEST_F(LoggerTest, WriteLogFailIfNotOpened) {
    Logger logger;
    EXPECT_FALSE(logger.createLog("file.txt", "hash", "Verdict"));
}

TEST_F(LoggerTest, MultipleWritesToFile) {
    Logger logger;
    logger.setLogFile(logFilePath.string());

    EXPECT_TRUE(logger.createLog("file1.txt", "somehash1", "OK"));
    EXPECT_TRUE(logger.createLog("file2.txt", "somehash2", "BAD"));

    auto lines = readAllLines();
    ASSERT_EQ(lines.size(), 6);
    EXPECT_NE(lines[0].find("file1.txt"), std::string::npos);
    EXPECT_NE(lines[3].find("file2.txt"), std::string::npos);
}

TEST_F(LoggerTest, FileClosedOnDestruction) {
    {
        Logger logger;
        logger.setLogFile(logFilePath.string());
        logger.createLog("file.txt", "hash", "OK");
    }
    
    std::ofstream out(logFilePath, std::ios::app);
    EXPECT_TRUE(out.is_open());
}

TEST_F(LoggerTest, CreateLogWithEmptyStrings) {
    Logger logger;
    logger.setLogFile(logFilePath.string());

    EXPECT_NO_THROW(logger.createLog("", "", ""));
    auto lines = readAllLines();
    ASSERT_EQ(lines.size(), 3);
    EXPECT_FALSE(lines[0].empty());
    EXPECT_FALSE(lines[1].empty());
    EXPECT_FALSE(lines[2].empty());
}

TEST_F(LoggerTest, MultiThreadedLogging) {
    Logger logger;
    logger.setLogFile(logFilePath.string());

    auto writer = [&logger](int id) {
        for (int i = 0; i < 10; ++i) {
            logger.createLog("f" + std::to_string(id),
                             "h" + std::to_string(i*42),
                             "V");
        }
    };

    std::thread t1(writer, 1);
    std::thread t2(writer, 2);

    t1.join();
    t2.join();

    auto lines = readAllLines();
    // Каждая запись — 3 строки, 2 потока по 10 записей = 60 строк
    EXPECT_EQ(lines.size(), 60);
}
