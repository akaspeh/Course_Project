//
// Created by PC on 17.01.2025.
//

#include "TaskQueueTest.h"

void test_task_queue_empty() {
    TaskQueue<int> queue;
    ASSERT_TRUE(queue.empty());  // The queue should be empty initially.
}

void test_task_queue_size() {
    TaskQueue<int> queue;
    queue.emplace(1);
    queue.emplace(2);
    ASSERT_EQ(queue.size(), 2);  // The size should be 2 after adding two elements.
}

void test_task_queue_pop() {
    TaskQueue<int> queue;
    queue.emplace(1);
    queue.emplace(2);
    int task;
    ASSERT_TRUE(queue.pop(task));  // Pop should succeed and return the first task.
    ASSERT_EQ(task, 1);  // The task popped should be 1.
    ASSERT_EQ(queue.size(), 1);  // The size should be 1 after popping one task.
}

void test_task_queue_clear() {
    TaskQueue<int> queue;
    queue.emplace(1);
    queue.emplace(2);
    queue.clear();
    ASSERT_TRUE(queue.empty());  // The queue should be empty after clearing it.
}

void test_task_queue_emplace() {
    TaskQueue<int> queue;
    queue.emplace(1);
    queue.emplace(2);
    int task;
    ASSERT_TRUE(queue.pop(task));  // Pop should return the first element.
    ASSERT_EQ(task, 1);  // The first task should be 1.
    ASSERT_TRUE(queue.pop(task));  // Pop should return the second element.
    ASSERT_EQ(task, 2);  // The second task should be 2.
}