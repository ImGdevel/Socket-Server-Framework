#ifndef WORKERQUEUE_H
#define WORKERQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class WorkerQueue {
public:
    void push(const T& task) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(task);
        condition.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [this]() { return !queue.empty(); });
        T task = queue.front();
        queue.pop();
        return task;
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable condition;
};

#endif
