#include "ThreadPool.h"
#include "../reactor/Worker.h"
#include <memory>

using namespace std;

ThreadPool::ThreadPool(int numThreads) : taskQueue(make_shared<WorkerQueue<std::function<void()>>>()) {
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back(make_unique<Worker>(taskQueue));
        workers.back()->start();
    }
}

ThreadPool::~ThreadPool(){
    for (auto& worker : workers) {
        worker->stop();
    }
}

// 작업 Queue에 작업 할당
void ThreadPool::enqueueTask(const function<void()>& task) {
    taskQueue->push(task);
}
