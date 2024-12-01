#include "ThreadPool.h"
#include "../reactor/Worker.h"
#include <memory>
#include <stdexcept>

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
    if (!task) {
        return;
        // todo: 아직 예외를 받을 부분이 없다. 
        // (예외 던지면 바로 종료 되어버림, 예외처리는 어디서 받고 처리할지 고민할 것!)
        //throw std::invalid_argument("Task cannot be null");
    }
    taskQueue->push(task);
}
