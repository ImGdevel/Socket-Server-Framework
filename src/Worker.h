#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <thread>
#include "WorkerQueue.h"

class Worker {
public:
    Worker(int id, WorkerQueue& queue);
    void run();

private:
    int id;
    WorkerQueue& workerQueue;
    void task();
};

#endif
