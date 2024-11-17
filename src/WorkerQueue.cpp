#include "WorkerQueue.h"
using namespace std;

WorkerQueue::WorkerQueue() : isRunning(true) {}

WorkerQueue::~WorkerQueue() {
    shutdown();
}

void WorkerQueue::pushTask(int taskSocket) {
    // 작업 추가
    {
        unique_lock<mutex> lock(queueMutex);
        taskQueue.push(taskSocket);
    }
    condition.notify_one();
}

bool WorkerQueue::popTask(int& taskSocket) {
    // 작업 소비
    unique_lock<mutex> lock(queueMutex);

    condition.wait(lock, [this]() {
        return !isRunning || !taskQueue.empty();
    });

    if (!isRunning && taskQueue.empty()) return false;

    taskSocket = taskQueue.front();
    taskQueue.pop();
    return true;
}

void WorkerQueue::shutdown() {
    {
        unique_lock<mutex> lock(queueMutex);
        isRunning = false;
    }
    condition.notify_all();
}
