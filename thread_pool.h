#ifndef CO_ROUTINES_THREADPOOL_H
#define CO_ROUTINES_THREADPOOL_H

#include <atomic>
#include <thread>
#include <vector>
#include <condition_variable>
#include <functional>
#include <mutex>

#include "thread_safe_queue.h"

class thread_pool
{
public:
    explicit thread_pool();

    ~thread_pool();
    void enqueue(std::function<void()> &&);
    size_t get_size() const;

private:
    std::vector<std::thread> threads_;
    thread_safe_queue tasks_;
    std::condition_variable cv_;
    std::mutex mutex_;
    std::atomic_bool stop_;
};

#endif