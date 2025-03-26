#ifndef WORKER_H
#define WORKER_H

#include <functional>
#include <thread>
#include <atomic>
#include "../threadpool/WorkerQueue.h"

class Worker {
public:
    
    Worker(int workerId, std::shared_ptr<WorkerQueue<std::function<void()>>> taskQueue);
    ~Worker();

    void start();
    void stop();

private:
    int workerId;
    std::shared_ptr<WorkerQueue<std::function<void()>>> taskQueue;
    std::thread thread;
    std::atomic<bool> running;
};

#endif
