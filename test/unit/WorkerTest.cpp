#include <gtest/gtest.h>
#include "../src/server/reactor/Worker.h"
#include "../src/server/threadpool/WorkerQueue.h"
#include <atomic>
#include <memory>
#include <thread>

// Worker 단위 테스트
TEST(WorkerTest, ExecuteTask) {
    auto queue = std::make_shared<WorkerQueue<std::function<void()>>>();
    Worker worker(queue);
    std::atomic<int> counter = 0;
    
    worker.start();
    queue->push([&counter]() { counter++; });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_EQ(counter, 1);
    
    worker.stop();
}

// Worker 정상 종료 테스트
TEST(WorkerTest, StopsGracefully) {
    auto queue = std::make_shared<WorkerQueue<std::function<void()>>>();
    Worker worker(queue);
    
    worker.start();
    worker.stop();
    
    ASSERT_NO_FATAL_FAILURE(worker.stop());
}
