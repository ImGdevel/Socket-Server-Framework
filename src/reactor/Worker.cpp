#include "Worker.h"
#include <memory>

using namespace std;

Worker::Worker(shared_ptr<WorkerQueue<std::function<void()>>> taskQueue)
: taskQueue(taskQueue), running(true) {}

Worker::~Worker(){
    // todo : 종료 작업
}

void Worker::start() {
    thread = std::thread([this]() {
        while (running) {
            auto task = taskQueue->pop(); //작업 큐에서 작업 pop
            if (task) {
                task();
            }
        }
    });
}

void Worker::stop() {
    running = false;
    taskQueue->push(nullptr);
    if (thread.joinable()) {
        thread.join();
    }
}