#include "thread_pool.hpp"

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

ThreadPool::~ThreadPool() {
    if (is_running) {
        is_running = false;
        
        joinAll();
    }
}

void ThreadPool::joinAll() {
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

void ThreadPool::shutdown() {
    if (is_running) is_running = false;
    tasks.allAddedNotifiation(); // разбудит все потоки
}

unsigned int ThreadPool::getThreadsNumber() const
{
    return threads_number;
}
