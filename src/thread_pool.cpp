#include "thread_pool.hpp"

ThreadPool::ThreadPool() : ThreadPool(0) {}

ThreadPool::ThreadPool(unsigned int th_n)
    : threads_number(th_n), is_running(true)
{
    for (size_t i = 0; i < threads_number; ++i) {
        threads.emplace_back([this] () {
            while (true) {
                auto task = tasks.pop();
                if (!task) {
                    break;
                }
                (*task)();
            }
        });
    }
}

unsigned int ThreadPool::getThreadsNumber() const {
    return threads_number;
}
