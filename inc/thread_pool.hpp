#pragma once
#include <atomic>
#include <functional>
#include <thread>
#include <vector>

#include "md5.hpp"
#include "threadsafe_queue.hpp"

class ThreadPool {
    ThreadSafeQueue<std::function<void()> > tasks;
    std::vector<std::thread> threads;
    
    std::atomic<bool> is_running{true};
public:
    ThreadPool(unsigned int threads_number);
    ~ThreadPool();

    template <typename Func>
    void enqueueTask(Func&& func);
};
