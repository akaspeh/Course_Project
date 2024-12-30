//
// Created by PC on 29.12.2024.
//

#ifndef COURSE_PROJECT_THREADPOOL_H
#define COURSE_PROJECT_THREADPOOL_H

#include <vector>
#include <functional>
#include "TaskQueue.h"
#include <condition_variable>
class ThreadPool {
public:
    inline ThreadPool() = default;
    inline ~ThreadPool() { terminate(); }
public:
    void initialize(const size_t worker_count);
    void terminate();
    void routine();
    bool working() const;
    bool working_unsafe() const;
public:
    template <typename task_t, typename... arguments>
    void add_task(task_t&& task, arguments&&... parameters);
public:
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(const ThreadPool& rhs) = delete;
    ThreadPool& operator=(ThreadPool&& rhs) = delete;
private:
    mutable read_write_lock m_rw_lock;
    mutable std::condition_variable_any m_task_waiter;
    std::vector<std::thread> m_workers;
    TaskQueue<std::function<void()>> m_tasks;
    std::vector<std::pair<std::function<void()>, int>> m_buff;
    bool m_initialized = false;
    bool m_terminated = false;
};

template <typename task_t, typename... arguments>
void ThreadPool::add_task(task_t&& task, arguments&&... parameters)
{
    ZoneScopedN( "add_task" );
    {
        read_lock _(m_rw_lock);
        if (!working_unsafe()) {
            return;
        }
    }
    auto bind = std::bind(std::forward<task_t>(task),
                          std::forward<arguments>(parameters)...);
    m_tasks.emplace(bind);
    m_task_waiter.notify_one();

}


#endif //COURSE_PROJECT_THREADPOOL_H
