//
// Created by PC on 17.01.2025.
//

#include "ThreadPoolTest.h"

void test_thread_pool_initialize() {
    ThreadPool pool;
    pool.initialize(3);
    ASSERT_TRUE(pool.working());  // Pool should be working after initialization
}

void test_thread_pool_terminate() {
    ThreadPool pool;
    pool.initialize(3);
    pool.terminate();
    ASSERT_FALSE(pool.working());  // Pool should not be working after termination
}

void test_thread_pool_add_task() {
    ThreadPool pool;
    pool.initialize(3);
    std::atomic<int> counter(0);

    pool.add_task([&counter] { counter.fetch_add(1); });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Allow time for task to execute

    ASSERT_EQ(counter.load(), 1);  // The task should have been executed
}

void test_thread_pool_add_multiple_tasks() {
    ThreadPool pool;
    pool.initialize(3);
    std::atomic<int> counter(0);

    pool.add_task([&counter] { counter.fetch_add(1); });
    pool.add_task([&counter] { counter.fetch_add(1); });
    pool.add_task([&counter] { counter.fetch_add(1); });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Allow time for tasks to execute

    ASSERT_EQ(counter.load(), 3);  // All tasks should have been executed
}