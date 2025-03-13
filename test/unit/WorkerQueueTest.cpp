#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "../../src/server/threadpool/WorkerQueue.h"

TEST(WorkerQueueTest, PushAndPopSingleElement) {
    WorkerQueue<int> queue;

    queue.push(42);
    int value = queue.pop();

    EXPECT_EQ(value, 42);
}

TEST(WorkerQueueTest, PushAndPopMultipleElements) {
    WorkerQueue<int> queue;

    queue.push(1);
    queue.push(2);
    queue.push(3);

    EXPECT_EQ(queue.pop(), 1);
    EXPECT_EQ(queue.pop(), 2);
    EXPECT_EQ(queue.pop(), 3);
}

TEST(WorkerQueueTest, PopBlocksWhenQueueIsEmpty) {
    WorkerQueue<int> queue;
    bool pop_completed = false;

    std::thread t([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.push(99);
    });

    int value = queue.pop();
    pop_completed = true;

    t.join();

    EXPECT_EQ(value, 99);
    EXPECT_TRUE(pop_completed);
}

