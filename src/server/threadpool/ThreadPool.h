#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <functional>
#include "WorkerQueue.h"

class Worker;

class ThreadPool {
public:

    ThreadPool(int numThreads);
    ~ThreadPool();
    
    void enqueueTask(const std::function<void()>& task);
    void stop();

private:
    std::shared_ptr<WorkerQueue<std::function<void()>>> taskQueue;
    std::vector<std::unique_ptr<Worker>> workers;
};

#endif
