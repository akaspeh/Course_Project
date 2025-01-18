//
// Created by PC on 29.12.2024.
//

#include "ThreadPool.h"

bool ThreadPool::working() const{
    read_lock _(m_rw_lock);
    return working_unsafe();
}
bool ThreadPool::working_unsafe() const{
    return m_initialized && !m_terminated;
}

void ThreadPool::initialize(const size_t worker_count) {
    write_lock _(m_rw_lock);
    if (m_initialized || m_terminated) {
        return;
    }

    m_workers.reserve(worker_count);
    for (size_t id = 0; id < worker_count; id++) {
        m_workers.emplace_back(&ThreadPool::routine, this);
    }

    m_initialized = !m_workers.empty();
}

void ThreadPool::routine(){
    while (true){
        bool task_accquiered = false;
        std::function<void()> task;
        {
            write_lock _(m_rw_lock);
            auto wait_condition = [this, &task_accquiered, &task] {
                task_accquiered = m_tasks.pop(task);
                return m_terminated || task_accquiered;
            };
            m_task_waiter.wait(_, wait_condition);
        }
        if (m_terminated && !task_accquiered){
            return;
        }
        task();
    }
}

void ThreadPool::terminate(){
    {
        write_lock _(m_rw_lock);
        if (working_unsafe()){
            m_terminated = true;
        }
        else{
            m_workers.clear();
            m_terminated = false;
            m_initialized = false;
            return;
        }
    }
    m_task_waiter.notify_all();
    for (std::thread& worker : m_workers){
        worker.join();
    }
    m_workers.clear();
    m_terminated = false;
    m_initialized = false;
}