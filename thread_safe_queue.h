#ifndef CO_ROUTINES_THREAD_SAFE_QUEUE_H
#define CO_ROUTINES_THREAD_SAFE_QUEUE_H
#include <deque>
#include <functional>
#include <mutex>
#include <optional>

class thread_safe_queue
{
public:
    void push_back(std::function<void()> &&task)
    {
        std::lock_guard lock(mutex_);
        queue_.push_back(task);
    }

    void push_front(std::function<void()> &&task)
    {
        std::lock_guard lock(mutex_);
        queue_.push_front(task);
    }

    [[nodiscard]] bool empty() const
    {
        std::lock_guard lock(mutex_);
        return queue_.empty();
    }

    [[nodiscard]] std::optional<std::function<void()>> pop_front()
    {
        std::lock_guard lock(mutex_);
        if (queue_.empty())
            return std::nullopt;

        auto front = std::move(queue_.front());
        queue_.pop_front();
        return front;
    }

    [[nodiscard]] std::optional<std::function<void()>> pop_back()
    {
        std::lock_guard lock(mutex_);
        if (queue_.empty())
            return std::nullopt;

        auto back = std::move(queue_.back());
        queue_.pop_back();
        return back;
    }

private:
    std::deque<std::function<void()>> queue_;
    mutable std::mutex mutex_;
};

#endif // CO_ROUTINES_THREAD_SAFE_QUEUE_H