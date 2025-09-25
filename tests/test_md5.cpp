#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "md5.hpp"

namespace fs = std::filesystem;

static std::string makeTempPath(const std::string& suffix) {
    auto p = fs::temp_directory_path();
    auto uniq = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
    return (p / ("md5_test_" + uniq + "_" + suffix)).string();
}

constexpr const char* MD5_EMPTY  = "d41d8cd98f00b204e9800998ecf8427e";
constexpr const char* MD5_ABC    = "900150983cd24fb0d6963f7d28e17f72";
constexpr const char* MD5_HELLO  = "5d41402abc4b2a76b9719d911017c592";

class MD5Test : public ::testing::Test {
protected:
    std::string tmp_path;

    void SetUp() override {}

    void TearDown() override {
        if (!tmp_path.empty()) {
            std::error_code ec;
            fs::remove(tmp_path, ec);
        }
    }

    void writeFile(const std::string& path, const std::string& content) {
        std::ofstream out(path, std::ios::binary);

        ASSERT_TRUE(out.is_open()) << "Невозможно создать файл: " << path;
        out.write(content.data(), static_cast<std::streamsize>(content.size()));

        out.close();
    }
};

TEST_F(MD5Test, EmptyFileHash) {
    tmp_path = makeTempPath("empty.bin");
    writeFile(tmp_path, ""); 

    MD5Calculator calc;
    std::string hash = calc.calculate(tmp_path);

    EXPECT_EQ(hash, MD5_EMPTY);
    EXPECT_EQ(calc.getReadErrorsCount(), 0u);
    EXPECT_EQ(calc.getHash(), hash);
}

TEST_F(MD5Test, SmallContentHash) {
    tmp_path = makeTempPath("abc.bin");
    writeFile(tmp_path, "abc");

    MD5Calculator calc;
    std::string hash = calc.calculate(tmp_path);

    EXPECT_EQ(hash, MD5_ABC);
    EXPECT_EQ(calc.getReadErrorsCount(), 0u);
}

TEST_F(MD5Test, NonExistentFile) {
    tmp_path = makeTempPath("non-existent.bin");

    MD5Calculator calc;
    std::string hash = calc.calculate(tmp_path);

    EXPECT_TRUE(hash.empty());
    EXPECT_EQ(calc.getReadErrorsCount(), 1u);
}

TEST_F(MD5Test, MultipleCalls) {
    tmp_path = makeTempPath("hello.bin");
    writeFile(tmp_path, "hello");

    MD5Calculator calc;
    std::string h1 = calc.calculate(tmp_path);
    
    EXPECT_EQ(h1, MD5_HELLO);
    EXPECT_EQ(calc.getReadErrorsCount(), 0u);

    
    writeFile(tmp_path, "abc");
    std::string h2 = calc.calculate(tmp_path);
    
    EXPECT_EQ(h2, MD5_ABC);
    EXPECT_EQ(calc.getReadErrorsCount(), 0u);
    EXPECT_NE(h1, h2);
}

TEST_F(MD5Test, MultipleNonExistentFiles) {
    MD5Calculator calc;
    std::string p1 = makeTempPath("non-existent1.bin");
    std::string p2 = makeTempPath("non-existent2.bin");

    EXPECT_EQ(calc.getReadErrorsCount(), 0u);

    calc.calculate(p1);
    EXPECT_EQ(calc.getReadErrorsCount(), 1u);

    calc.calculate(p2);
    EXPECT_EQ(calc.getReadErrorsCount(), 2u);
}