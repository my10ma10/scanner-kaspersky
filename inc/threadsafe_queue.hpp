#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <optional>


namespace fs = std::filesystem;

template <typename T>
class ThreadSafeQueue {
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::queue<T> queue;
    bool all_files_added = false;

public:
    explicit ThreadSafeQueue() = default;

    template <typename U>
    void push(U&& value);
    
    std::optional<T> pop();

    bool empty() const;
    size_t size() const;
    
    void allAddedNotifiation();
   
    const std::queue<T>& getOriginQueue() const;
};


template <typename T> template <typename U>
void ThreadSafeQueue<T>::push(U&& value) {
    {
        std::scoped_lock lock(mtx);
        queue.push(std::forward<U>(value));
    }
    cv.notify_one();
}

template <typename T>
std::optional<T> ThreadSafeQueue<T>::pop() {
    std::unique_lock lock(mtx);

    cv.wait(lock, [&] () {
        return !queue.empty() || all_files_added;
    });

    if (queue.empty() && all_files_added) {
        return std::nullopt;
    }

    T res = std::move(queue.front());
    queue.pop();

    return res;
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const {
    std::scoped_lock lock(mtx);
    return queue.empty();
}

template <typename T>
size_t ThreadSafeQueue<T>::size() const {
    std::scoped_lock lock(mtx);
    return queue.size();
}

template <typename T>
void ThreadSafeQueue<T>::allAddedNotifiation() {
    {
        std::scoped_lock lock(mtx);
        all_files_added = true;
    }
    cv.notify_all();
}

template <typename T>
const std::queue<T>& ThreadSafeQueue<T>::getOriginQueue() const {
    std::scoped_lock lock(mtx);
    return queue;
}