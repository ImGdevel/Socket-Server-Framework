#include "ThreadPool.h"
#include "../reactor/Worker.h"
#include <memory>
#include <stdexcept>
#include "Logger.h"

using namespace std;

ThreadPool::ThreadPool(int numThreads) : taskQueue(make_shared<WorkerQueue<std::function<void()>>>()) {
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back(make_unique<Worker>(taskQueue));
        workers.back()->start();
    }
}

ThreadPool::~ThreadPool(){
    stop();
}

// 작업 Queue에 작업 할당
void ThreadPool::enqueueTask(const function<void()>& task) {
    if (!task) {
        return;
    }
    taskQueue->push(task);
}

// ThreadPool내 Worker 정리
void ThreadPool::stop() {
    for (auto& worker : workers) {
        worker->stop();
    }
    Logger::info("All workers stopped.");
}
