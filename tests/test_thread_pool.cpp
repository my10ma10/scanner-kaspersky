#include <algorithm>
#include <atomic>
#include <chrono>
#include <vector>
#include <mutex>
#include <stdexcept>

#include "test_helper.hpp"
#include "thread_pool.hpp"

namespace ch = std::chrono;
using namespace std::chrono_literals;

TEST(ThreadPoolTest, SingleTask) {
    std::atomic<int> counter{0};
    {
        ThreadPool pool(1);
        pool.enqueueTask([&counter]() {
            counter++;
        });
        pool.shutdown();
        pool.joinAll();
    }
    
    EXPECT_EQ(counter.load(), 1);
}


TEST(ThreadPoolTest, MultipleTasks) {
    const int TASK_COUNT = 100;
    std::atomic<int> counter{0};
    {
        ThreadPool pool(4);
        
        for (int i = 0; i < TASK_COUNT; ++i) {
            pool.enqueueTask([&counter]() {
                counter++;
            });
        }
        
        pool.shutdown();
        pool.joinAll();
    }
    
    EXPECT_EQ(counter.load(), TASK_COUNT);
}

TEST(ThreadPoolTest, ZeroThreadsPool) {
    std::atomic<int> counter{0};
    
    {
        ThreadPool pool(0);
        
        pool.enqueueTask([&counter] {
            counter++;
        });
        
        pool.shutdown();
        pool.joinAll();
    }
    
    EXPECT_EQ(counter.load(), 0);
}

TEST(ThreadPoolTest, ShutdownInfluence) {
    std::atomic<int> counter{0};
    
    ThreadPool pool(2);
    
    pool.enqueueTask([&counter]() {
        counter++;
    });
    
    pool.shutdown();
    pool.joinAll();
    
    pool.enqueueTask([&counter]() {
        counter++;
    });
    
    EXPECT_EQ(counter.load(), 1);
}

TEST(ThreadPoolTest, ParallelExecution) {
    const int TASK_COUNT = 8;
    std::vector<int> task_ids;
    std::mutex mutex;
    {
        ThreadPool pool(4);
        
        for (int i = 0; i < TASK_COUNT; ++i) {
            pool.enqueueTask([i, &task_ids, &mutex]() {
                std::this_thread::sleep_for(50ms);
                
                std::scoped_lock lock(mutex);
                task_ids.push_back(i);
            });
        }        
        pool.shutdown();
        pool.joinAll();
    }
    
    EXPECT_EQ(task_ids.size(), TASK_COUNT);
    
    std::sort(task_ids.begin(), task_ids.end());
    for (int i = 0; i < TASK_COUNT; ++i) {
        EXPECT_EQ(task_ids[i], i);
    }
}

TEST(ThreadPoolTest, GetThreadsNumber) {
    ThreadPool pool1(5);
    pool1.shutdown();
    pool1.joinAll();
    EXPECT_EQ(pool1.getThreadsNumber(), 5u);
    
    ThreadPool pool2(10);
    pool2.shutdown();
    pool2.joinAll();
    EXPECT_EQ(pool2.getThreadsNumber(), 10u);
    
    ThreadPool pool3;
    EXPECT_EQ(pool3.getThreadsNumber(), 0u);
}

TEST(ThreadPoolTest, ShutdownWithoutTasks) {
    {
        ThreadPool pool(3);
        // не добавляем никаких задач
    }
    SUCCEED();
}

TEST(ThreadPoolTest, StressTest) {
    const int TASK_COUNT = 10000;
    std::atomic<int> counter{0};
    
    auto start = ch::steady_clock::now();    
    {
        ThreadPool pool(10);
        
        for (int i = 0; i < TASK_COUNT; ++i) {
            pool.enqueueTask([&counter] {
                counter++;
            });
        }        
        pool.shutdown();
        pool.joinAll();
    }
    
    auto end = ch::steady_clock::now();
    auto duration = ch::duration_cast<ch::milliseconds>(end - start);
    
    EXPECT_EQ(counter.load(), TASK_COUNT);
    
    EXPECT_LT(duration.count(), 5000);
}

TEST(ThreadPoolTest, MultipleShutdownJoinAllCalls) {
    std::atomic<int> counter{0};
    
    ThreadPool pool(2);
    
    pool.enqueueTask([&counter] {
        counter++;
    });
    
    pool.shutdown();
    pool.shutdown();
    pool.shutdown();
    
    pool.joinAll();
    
    EXPECT_EQ(counter.load(), 1);
    
    pool.joinAll();
    pool.joinAll();
}
