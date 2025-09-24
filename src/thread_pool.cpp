#include "thread_pool.hpp"

ThreadPool::ThreadPool(unsigned int threads_number) {
    for (size_t i = 0; i < threads_number; ++i) {
        threads.emplace_back([this] () {
            while (is_running) {
                auto task = tasks.pop();
                if (!task) {
                    if (is_running) {
                        break;
                    }
                    continue;
                }
                (*task)();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    if (is_running) {
        is_running = false;
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }
}

template <typename Func>
void ThreadPool::enqueueTask(Func&& func) {
    tasks.push(std::function<void()>(std::forward<Func>(func)))
}
