#include <gtest/gtest.h>
#include "../src/server/threadpool/ThreadPool.h"
#include <atomic>
#include <thread>

// ThreadPool 단일 작업 할당 테스트
TEST(ThreadPoolTest, EnqueueTask) {
    ThreadPool pool(2);
    std::atomic<int> counter = 0;
    
    pool.enqueueTask([&counter]() { counter++; });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_EQ(counter, 1);
}

// ThreadPool 다중 작업 할당 테스트
TEST(ThreadPoolTest, MultipleTasks) {
    ThreadPool pool(4);
    std::atomic<int> counter = 0;
    
    for (int i = 0; i < 10; ++i) {
        pool.enqueueTask([&counter]() { counter++; });
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_EQ(counter, 10);
}
