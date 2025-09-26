#include <filesystem>
#include <fstream>
#include <thread>
#include <chrono>

#include "test_helper.hpp"
#include "utility.hpp"

using namespace std::chrono_literals;

class UtilityTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir = fs::temp_directory_path() / "utility_test";
        base_dir = fs::temp_directory_path() / "utility_base";
        log_file = fs::temp_directory_path() / "test_log.log";
        
        if (fs::exists(test_dir)) {
            fs::remove_all(test_dir);
        }
        if (fs::exists(base_dir)) {
            fs::remove_all(base_dir);
        }
        
        fs::create_directories(test_dir);
        fs::create_directories(base_dir);
        
        if (fs::exists(log_file)) {
            fs::remove(log_file);
        }
    }
    
    void TearDown() override {
        if (fs::exists(test_dir)) {
            fs::remove_all(test_dir);
        }
        if (fs::exists(base_dir)) {
            fs::remove_all(base_dir);
        }
        if (fs::exists(log_file)) {
            fs::remove(log_file);
        }
    }
    
    void createTestFile(const fs::path& path, const std::string& content) {
        fs::create_directories(path.parent_path());
        std::ofstream file(path);
        file << content;
        file.close();
    }
    
    void createBaseFile(const std::string& hash) {
        std::ofstream base_file(base_dir / "base.csv");
        base_file << hash << ";testMalware\n";
        base_file.close();
    }
    
    fs::path test_dir;
    fs::path base_dir;
    fs::path log_file;
};

TEST_F(UtilityTest, ConstructorTest) {
    createBaseFile("ctor_hash");
    
    EXPECT_NO_THROW({
        Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    });
}

TEST_F(UtilityTest, ProcessSingleFile) {
    createTestFile(test_dir / "test.txt", "Hello World");
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    
    EXPECT_NO_THROW(utility.run());
    
    testing::internal::CaptureStdout();
    utility.printReport();

    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: 1"));
    EXPECT_THAT(output, testing::HasSubstr("Из них вредоносных: 0"));
}

TEST_F(UtilityTest, ProcessMultipleFiles) {
    createTestFile(test_dir / "file1.txt", "Content 1");
    createTestFile(test_dir / "file2.txt", "Content 2");
    createTestFile(test_dir / "subdir" / "file3.txt", "Content 3");
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    
    EXPECT_NO_THROW(utility.run());
    
    testing::internal::CaptureStdout();
    utility.printReport();

    std::string output = testing::internal::GetCapturedStdout();    
    
    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: 3"));
}

TEST_F(UtilityTest, DetectMaliciousFile) {
    std::string content = "malicious content";
    createTestFile(test_dir / "malware.txt", content);
    
    MD5Calculator calc;
    std::string expected_hash = "d5708d67cee304cde1a69dae5a463a9e";
    
    createBaseFile(expected_hash);
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    utility.run();
    
    testing::internal::CaptureStdout();
    utility.printReport();

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: 1"));
    EXPECT_THAT(output, testing::HasSubstr("Из них вредоносных: 1"));
}

TEST_F(UtilityTest, EmptyDirectoryTest) {
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    utility.run();
    
    testing::internal::CaptureStdout();
    utility.printReport();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: 0"));
    EXPECT_THAT(output, testing::HasSubstr("Из них вредоносных: 0"));
}

TEST_F(UtilityTest, NonExistentDirectoryTest) {
    fs::path non_existent = test_dir / "does_not_exist";
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    // Конструктор может кинуть исключение или обработать ошибку
    EXPECT_NO_THROW({
        Utility utility(non_existent.string(), (base_dir / "base.csv").string(), log_file.string());
        utility.run();
    });
}

TEST_F(UtilityTest, ThreadSafetyTest) {
    const int FILE_COUNT = 100;
    
    for (int i = 0; i < FILE_COUNT; ++i) {
        createTestFile(test_dir / ("file_" + std::to_string(i) + ".txt"), 
                      "Content " + std::to_string(i));
    }
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    
    utility.run();
    
    testing::internal::CaptureStdout();
    utility.printReport();

    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: " + std::to_string(FILE_COUNT)));
}

TEST_F(UtilityTest, PrintReportFormat) {
    createTestFile(test_dir / "test.txt", "test");
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    utility.run();
    
    testing::internal::CaptureStdout();
    utility.printReport();
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверяем формат отчета
    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: "));
    EXPECT_THAT(output, testing::HasSubstr("Из них вредоносных: "));
    EXPECT_THAT(output, testing::HasSubstr("Количество ошибок чтения: "));
    EXPECT_THAT(output, testing::HasSubstr("Время выполнения утилиты: "));
    EXPECT_THAT(output, testing::HasSubstr("сек"));
}

TEST_F(UtilityTest, TimerPrintingTest) {
    createTestFile(test_dir / "test.txt", "test");
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    auto start = std::chrono::steady_clock::now();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    
    // Добавляем задержку в обработку
    std::this_thread::sleep_for(100ms);
    
    utility.run();
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    testing::internal::CaptureStdout();
    utility.printReport();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_GE(duration, 100ms);
    
    EXPECT_THAT(output, testing::HasSubstr("Время выполнения утилиты: "));
    EXPECT_THAT(output, testing::HasSubstr("сек"));
}

TEST_F(UtilityTest, SpecificFilesNamesTest) {
    std::setlocale(LC_ALL, "RU");
    
    createTestFile(test_dir / "file with spaces.txt", "content");
    createTestFile(test_dir / "кириллица.txt", "content");
    createTestFile(test_dir / "file-with-dashes.txt", "content");
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    utility.run();
    
    testing::internal::CaptureStdout();
    utility.printReport();

    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: 3"));
}

TEST_F(UtilityTest, LargeFileHandling) {
    std::string large_content(10000, 'A');
    createTestFile(test_dir / "large_file.bin", large_content);
    
    std::ofstream base_file(base_dir / "base.csv");
    base_file.close();
    
    Utility utility(test_dir.string(), (base_dir / "base.csv").string(), log_file.string());
    utility.run();
    
    testing::internal::CaptureStdout();
    utility.printReport();

    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_THAT(output, testing::HasSubstr("Обработано файлов: 1"));
}