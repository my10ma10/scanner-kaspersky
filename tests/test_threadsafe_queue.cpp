#include <algorithm>

#include "test_helper.hpp"
#include "threadsafe_queue.hpp"

using namespace std::chrono_literals;

TEST(ThreadSafeQueueTest, PushPopSingleThread) {
    ThreadSafeQueue<int> q;

    q.push(42);
    auto v = q.pop();

    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(ThreadSafeQueueTest, EmptyQueueState) {
    ThreadSafeQueue<int> q;
    
    const auto& raw_queue = q.getOriginQueue();

    EXPECT_TRUE(raw_queue.empty());
    EXPECT_EQ(raw_queue.size(), 0u);
}

TEST(ThreadSafeQueueTest, PopSleepsUntilPush) {
    ThreadSafeQueue<int> q;
    std::optional<int> result;

    std::thread t([&] {
        result = q.pop();
    });

    std::this_thread::sleep_for(100ms);

    q.push(99);

    t.join();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 99);
}

TEST(ThreadSafeQueueTest, NotificatingReturnsNullopt) {
    ThreadSafeQueue<int> q;

    q.allAddedNotifiation();
    auto v = q.pop();

    EXPECT_FALSE(v.has_value());
}

TEST(ThreadSafeQueueTest, MultipleProducerConsumer) {
    ThreadSafeQueue<int> q;

    const int N = 100;
    std::vector<int> consumed;
    std::mutex consMtx;

    std::thread producer([&] {
        for (int i = 0; i < N; ++i) {
            q.push(i);
        }
        q.allAddedNotifiation();
    });

    std::thread consumer([&] {
        while (true) {
            auto v = q.pop();
            if (!v.has_value()) {
                break;
            }
            std::scoped_lock lock(consMtx);

            consumed.push_back(*v);
        }
    });

    producer.join();
    consumer.join();

    EXPECT_EQ(consumed.size(), N);
    
    std::sort(consumed.begin(), consumed.end());
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(consumed[i], i);
    }
}

TEST(ThreadSafeQueueTest, GetOriginQueue) {
    ThreadSafeQueue<int> q;
    q.push(1);
    q.push(2);

    const auto& rawQueue = q.getOriginQueue();
    EXPECT_EQ(rawQueue.size(), 2u);
    EXPECT_EQ(rawQueue.front(), 1);
}

TEST(ThreadSafeQueueTest, PushAfterNotification) {
    ThreadSafeQueue<int> q;
    
    q.allAddedNotifiation();
    
    q.push(27);
    
    auto result = q.pop();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 27);
    
    auto result2 = q.pop();
    EXPECT_FALSE(result2.has_value());
}

TEST(ThreadSafeQueueTest, PerfectForwardingTest) {
    ThreadSafeQueue<std::unique_ptr<int>> q;
    
    auto ptr = std::make_unique<int>(33);
    int* raw_ptr = ptr.get();
    
    q.push(std::move(ptr));
    
    EXPECT_EQ(ptr.get(), nullptr);
    
    auto result = q.pop();
    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->get(), raw_ptr);
    EXPECT_EQ(**result, 33);
}

TEST(ThreadSafeQueueTest, StressTest) {
    ThreadSafeQueue<int> q;
    
    const int ITERS = 10000;
    std::atomic<int> sum_produced{0};
    std::atomic<int> sum_consumed{0};
    
    std::thread producer([&] {
        for (int i = 1; i <= ITERS; ++i) {
            q.push(i);
            sum_produced++;
        }
        q.allAddedNotifiation();
    });
    
    std::thread consumer([&] {
        while (true) {
            auto result = q.pop();
            if (!result.has_value()) {
                break;
            }
            sum_consumed++;
        }
    });
    
    producer.join();
    consumer.join();
    
    EXPECT_EQ(sum_produced.load(), sum_consumed.load());
    EXPECT_EQ(sum_consumed.load(), ITERS);
}

TEST(ThreadSafeQueueTest, EmptyAndSizeAfterPush) {
    ThreadSafeQueue<int> q;
    
    q.push(1);
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 1u);
    
    q.push(2);
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 2u);
}