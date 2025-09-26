#pragma once
#include <atomic>
#include <functional>
#include <thread>
#include <vector>

#include "threadsafe_queue.hpp"

class ThreadPool {
    ThreadSafeQueue<std::function<void()> > tasks;
    std::vector<std::thread> threads;

    unsigned int threads_number;
    std::atomic<bool> is_running;
public:
    ThreadPool();
    ThreadPool(unsigned int th_n);

    void shutdown() {
        if (is_running) {
            is_running = false;
        }
        tasks.allAddedNotifiation();
    }
    
    ~ThreadPool() {
        shutdown();
        joinAll();
    }

    template <typename Func>
    void enqueueTask(Func&& func);
    
    void joinAll() {
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }

    unsigned int getThreadsNumber() const;
};

template <typename Func>
void ThreadPool::enqueueTask(Func&& func) {
    tasks.push(std::function<void()>(std::forward<Func>(func)));
}

