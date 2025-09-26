#include <set>

#include "test_helper.hpp"
#include "threadsafe_queue.hpp"
#include "traversal.hpp"

#ifdef _WIN32

class TraversalTest : public ::testing::Test {
protected:
    fs::path tempDir;

    void SetUp() override {
        tempDir = makeTempPath("dir");
        fs::create_directories(tempDir);
        ASSERT_TRUE(fs::exists(tempDir));
    }

    void TearDown() override {
        if (!tempDir.empty() && fs::exists(tempDir)) {
            fs::remove_all(tempDir);
        }
    }
};

TEST_F(TraversalTest, EmptyDir) {
    ThreadSafeQueue<fs::path> queue;
    Traversal t(tempDir);

    t.run(queue);
    queue.allAddedNotifiation();

    std::vector<fs::path> collected;
    while (auto p = queue.pop()) {
        collected.push_back(*p);
    }

    EXPECT_TRUE(collected.empty());
    EXPECT_EQ(t.getProcessedCount(), 0u);
}

TEST_F(TraversalTest, FilesInNestedDirs) {
    fs::path f1 = tempDir / "file1.txt";
    fs::path sub = tempDir / "subdir";
    fs::create_directories(sub);
    fs::path f2 = sub / "file2.bin";

    writeFile(f1.string(), "hello");
    writeFile(f2.string(), "world");

    ThreadSafeQueue<fs::path> queue;
    Traversal t(tempDir);

    t.run(queue);
    queue.allAddedNotifiation();

    std::set<std::string> found;
    while (auto p = queue.pop()) {
        found.insert(fs::weakly_canonical(*p).string());
    }

    EXPECT_EQ(found.size(), 2u);
    EXPECT_TRUE(found.count(fs::weakly_canonical(f1).string()) == 1);
    EXPECT_TRUE(found.count(fs::weakly_canonical(f2).string()) == 1);

    EXPECT_GE(t.getProcessedCount(), 2u);
}

TEST_F(TraversalTest, ManyFilesInDir) {
    const int N = 10;
    std::vector<fs::path> paths;

    for (int i = 0; i < N; ++i) {
        fs::path p = tempDir / ("file" + std::to_string(i) + ".txt");

        writeFile(p.string(), std::to_string(i));
        paths.push_back(fs::weakly_canonical(p));
    }

    ThreadSafeQueue<fs::path> queue;
    Traversal t(tempDir);

    t.run(queue);
    queue.allAddedNotifiation();

    std::set<std::string> found;
    while (auto p = queue.pop()) {
        found.insert(fs::weakly_canonical(*p).string());
    }

    EXPECT_EQ(found.size(), static_cast<size_t>(N));

    for (auto& p : paths) {
        EXPECT_TRUE(found.count(p.string()) == 1);
    }

    EXPECT_GE(t.getProcessedCount(), static_cast<unsigned int>(N));
}

#endif // _WIN32