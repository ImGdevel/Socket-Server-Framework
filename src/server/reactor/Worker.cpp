// Worker.cpp
#include "Worker.h"
#include "Logger.h"
#include <memory>
#include <iostream>

using namespace std;

Worker::Worker(int workerId, shared_ptr<WorkerQueue<std::function<void()>>> taskQueue)
:workerId(workerId), taskQueue(taskQueue),  running(true) {}

Worker::~Worker(){
    if (thread.joinable() && running.load()) {
        stop();
    }
}

void Worker::start() {
    thread = std::thread([this]() {
        while (running.load()) {
            auto task = taskQueue->pop();
            if (!task) {
                break;
            }
            try {
                task();
            } catch (const runtime_error& e) {
                Logger::error("[" + to_string(workerId) + "Runtime Exception in task execution: " + string(e.what()));
            } catch (const exception& e) {
                Logger::error("[" + to_string(workerId) + "]Exception in task execution: " + string(e.what()));
            }
        }
    });
}

void Worker::stop() {
    running.store(false);
    taskQueue->push(nullptr);
    if (thread.joinable()) {
        thread.join();
    }
    Logger::debug(to_string(workerId) + " Worker Shutdown");
}
