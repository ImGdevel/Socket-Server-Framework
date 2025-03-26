#include "Worker.h"
#include "Logger.h"
#include <memory>
#include <iostream>

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
            if (!task) {
                continue;
            }
            try {
                task();
            } catch (const runtime_error& e) {
                Logger::error("Runtime Exception in task execution: " + string(e.what()));
            } catch (const exception& e) {
                Logger::error("Exception in task execution: " + string(e.what()));
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