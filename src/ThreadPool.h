#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include "Worker.h"
#include "WorkerQueue.h"

class ThreadPool {
public:
    ThreadPool(int count);
    ~ThreadPool();

    void initialize();
    void enqueueTask(int taskSocket);

private:
    std::vector<std::thread> threads;
    std::vector<Worker> workers;
    WorkerQueue workerQueue;
    int workerCount;
};

#endif
