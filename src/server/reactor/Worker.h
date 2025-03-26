#ifndef WORKER_H
#define WORKER_H

#include <functional>
#include <thread>
#include "../threadpool/WorkerQueue.h"

class Worker {
public:
    
    Worker(std::shared_ptr<WorkerQueue<std::function<void()>>> taskQueue);
    ~Worker();

    void start();
    void stop();

private:
    std::shared_ptr<WorkerQueue<std::function<void()>>> taskQueue;
    std::thread thread;
    bool running;
};

#endif
