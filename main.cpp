#include <iostream>
#include <format>
#include <thread>
#include <string>
#include "thread_pool.h"

static std::mutex output_mutex;
static std::atomic_int counter = 1;

void simulate_task(int i)
{
    {
        std::lock_guard lock(output_mutex);
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        std::cout << std::format("Order {}: Task {} is running on thread {}", counter.fetch_add(1), i, oss.str())<< std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main()
{
    auto threadPool = thread_pool();
    const auto threads = threadPool.get_size();

    std::cout << "threads available: " << threads << std::endl;
    for (auto i = 0; i < 100; ++i)
    {
        threadPool.enqueue([i]
                           { simulate_task(i); });
    }
    return 0;
}
