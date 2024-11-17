#include "Worker.h"
#include <iostream>
using namespace std;

Worker::Worker(int id, WorkerQueue& queue) : id(id), workerQueue(queue) {}

void Worker::run() {
    while (true) {
        task();
    }
}

void Worker::task() {
    int clientSocket;

    if (!workerQueue.popTask(clientSocket)) {
        return;
    }

    try {
        // todo : 쓰레드 작업
        cout << "Task! " << id << " " <<  clientSocket << "\n";
    } catch (const exception& ex) {
        cerr << "[Error]" << "[Worker " << id << "] : " << ex.what() << endl;
    }
}
