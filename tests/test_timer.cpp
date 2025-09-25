#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "timer.hpp"

using namespace std::chrono_literals;

static constexpr double EPS = 0.025;

TEST(TimerTest, AfterStart) {
    Timer t;
    t.start();

    double sec = t.getTime();

    EXPECT_GE(sec, 0.0);
    EXPECT_LT(sec, 0.5);
}

TEST(TimerTest, NearAfterSleep) {
    Timer t;
    t.start();
    
    std::this_thread::sleep_for(200ms);

    double sec = t.getTime();
    EXPECT_NEAR(sec, 0.200, EPS);
}

TEST(TimerTest, MonotonicIncreasing) {
    Timer t;
    t.start();

    std::this_thread::sleep_for(100ms);
    double t1 = t.getTime();

    std::this_thread::sleep_for(100ms);
    double t2 = t.getTime();

    EXPECT_GT(t2, t1);
    EXPECT_NEAR(t2 - t1, 0.100, EPS);
}

TEST(TimerTest, StartResetsTimer) {
    Timer t;
    t.start();

    std::this_thread::sleep_for(150ms);

    double before = t.getTime();
    EXPECT_NEAR(before, 0.150, EPS);

    t.start();

    std::this_thread::sleep_for(100ms);

    double after = t.getTime();
    EXPECT_NEAR(after, 0.100, EPS);
}