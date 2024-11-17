#ifndef WORKER_QUEUE_H
#define WORKER_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class WorkerQueue {
public:
    WorkerQueue();
    ~WorkerQueue();

    void pushTask(int taskSocket);
    bool popTask(int& taskSocket); 
    void shutdown();

private:
    std::queue<int> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> isRunning;
};

#endif // WORKER_QUEUE_H
