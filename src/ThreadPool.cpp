#include "ThreadPool.h"
#include <iostream>
using namespace std;

ThreadPool::ThreadPool(int workerCount) : workerCount(workerCount) {
    
}

ThreadPool::~ThreadPool() {
    workerQueue.shutdown();

    for (thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    cout << "ThreadPool 종료" << endl;
}

void ThreadPool::initialize() {
    cout << "ThreadPool 초기화 ";
    for (int i = 0; i < workerCount; ++i) {
        workers.emplace_back(i, workerQueue);
    }

    for (Worker& worker : workers) {
        threads.emplace_back(&Worker::run, &worker);
    }

    cout << workerCount << " workers." << endl;
}

void ThreadPool::enqueueTask(int taskSocket) {
    workerQueue.pushTask(taskSocket);
}
