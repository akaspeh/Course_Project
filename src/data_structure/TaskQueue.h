//
// Created by PC on 29.12.2024.
//

#ifndef COURSE_PROJECT_TASKQUEUE_H
#define COURSE_PROJECT_TASKQUEUE_H

#include <queue>
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <mutex>

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;


template <typename task_type_t>
class TaskQueue {
    using task_queue_implementation = std::queue<task_type_t>;
public:
    TaskQueue() = default;
    ~TaskQueue() { clear(); }
    bool empty() const;
    size_t size() const;
public:
    void clear();
    bool pop(task_type_t& task);
    template <typename... arguments>
    void emplace(arguments&&... parameters);
public:
    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue(TaskQueue&& other) = delete;
    TaskQueue& operator=(const TaskQueue& rhs) = delete;
    TaskQueue& operator=(TaskQueue&& rhs) = delete;
private:
    mutable read_write_lock m_rw_lock;
    task_queue_implementation m_tasks;
};

#include "TaskQueue.tpp"


#endif //COURSE_PROJECT_TASKQUEUE_H
