#include <gtest/gtest.h>
#include "../../src/server/threadpool/WorkerQueue.h"
#include <thread>

// 단일 요소 push/pop 테스트
TEST(WorkerQueueTest, PushAndPopSingleElement) {
    WorkerQueue<int> queue;
    queue.push(42);
    EXPECT_EQ(queue.pop(), 42);
}

// 여러 요소 push 후 FIFO 순서 확인
TEST(WorkerQueueTest, PushMultipleElements) {
    WorkerQueue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    EXPECT_EQ(queue.pop(), 1);
    EXPECT_EQ(queue.pop(), 2);
    EXPECT_EQ(queue.pop(), 3);
}

// pop이 블로킹 상태인지 확인
TEST(WorkerQueueTest, PopBlocksUntilElementIsPushed) {
    WorkerQueue<int> queue;
    std::thread producer([&queue]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.push(99);
    });
    EXPECT_EQ(queue.pop(), 99);
    producer.join();
}

// 멀티스레드 환경에서 push/pop 동작 확인
TEST(WorkerQueueTest, ThreadSafetyTest) {
    WorkerQueue<int> queue;
    std::vector<std::thread> producers, consumers;
    const int numOperations = 1000;

    for (int i = 0; i < 5; ++i) {
        producers.emplace_back([&queue, numOperations]() {
            for (int j = 0; j < numOperations; ++j) {
                queue.push(j);
            }
        });
    }

    std::atomic<int> count{0};
    for (int i = 0; i < 5; ++i) {
        consumers.emplace_back([&queue, &count, numOperations]() {
            for (int j = 0; j < numOperations; ++j) {
                queue.pop();
                count.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto& p : producers) p.join();
    for (auto& c : consumers) c.join();

    EXPECT_EQ(count.load(), numOperations * 5);
}
