#include "thread_pool.hpp"
#include <iostream>

ThreadPool::ThreadPool() : ThreadPool(0) {}

ThreadPool::ThreadPool(unsigned int th_n)
    : threads_number(th_n), is_running(true)
{
    for (size_t i = 0; i < threads_number; ++i) {
        threads.emplace_back([this] () {
            while (is_running) {
                auto task = tasks.pop();
                if (!task) {
                    break;
                }
                (*task)();
            }
        });
    }
}

void ThreadPool::shutdown() {
    if (is_running) is_running = false;
    tasks.allAddedNotifiation();
}

unsigned int ThreadPool::getThreadsNumber() const {
    return threads_number;
}
