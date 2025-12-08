#include "thread_pool.h"

#include <iostream>

thread_pool::thread_pool() {
    const size_t size = std::thread::hardware_concurrency();
    stop_.store(false);
    for (size_t i = 0; i < size; i++) {
        threads_.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock lock(mutex_);
                    cv_.wait(lock, [this] {
                        return !tasks_.empty() || stop_.load();
                    });
                    if (stop_.load() && tasks_.empty()) {
                        return;
                    }
                    auto opt = std::move(tasks_.pop_front());
                    if (!opt) continue;
                    task = std::move(opt.value());
                }
                task();
            }
        });
    }
}

thread_pool::~thread_pool() {
    {
        std::lock_guard lock(mutex_);
        stop_.store(true);
    }
    cv_.notify_all();
    for (auto& thread : threads_) {
        thread.join();
    }
}

void thread_pool::enqueue(std::function<void()> &&task) {
    {
        std::lock_guard lock(mutex_);
        tasks_.push_back(std::move(task));
    }
    cv_.notify_one();

}


size_t thread_pool::get_size() const{
    return threads_.size();
}

