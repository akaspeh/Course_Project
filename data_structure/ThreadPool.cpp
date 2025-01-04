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

void ThreadPool::initialize(const size_t worker_count, const float ratio_for_request_tasks_part) {
    ZoneScoped;
    write_lock _(m_rw_lock);
    if (m_initialized || m_terminated) {
        return;
    }

    // Если ratio_for_request_tasks_part не передан, будет использовано значение по умолчанию (1.0f)
    m_ratio_for_request_task_part = ratio_for_request_tasks_part;

    m_workers.reserve(worker_count);
    for (size_t id = 0; id < worker_count; id++) {
        m_workers.emplace_back(&ThreadPool::routine, this);
    }

    m_initialized = !m_workers.empty();
}

void ThreadPool::routine(){
    ZoneScoped;
    while (true){
        int8_t queue_choice_flag = 0; // 0 - start, 1 - inverted index, 2 - request task
        bool task_accquiered = false;
        std::function<void()> task;
        {
            write_lock _(m_rw_lock);
            auto wait_condition = [this, &task_accquiered, &task, &queue_choice_flag] {
                if(m_request_task_counter > m_workers.size()*m_ratio_for_request_task_part){
                    task_accquiered = m_inverted_index_tasks.pop(task);
                    if(task_accquiered) {
                        m_inverted_index_task_counter++;
                        queue_choice_flag = 1;
                    }
                }
                else{
                    task_accquiered = m_request_tasks.pop(task);
                    if(task_accquiered) {
                        m_request_task_counter++;
                        queue_choice_flag = 2;
                    }
                }
                return m_terminated || task_accquiered;
            };
            m_task_waiter.wait(_, wait_condition);
        }
        if (m_terminated && !task_accquiered){
            return;
        }
        task();
        if(queue_choice_flag == 2){m_request_task_counter--;}
        else if(queue_choice_flag == 1){m_inverted_index_task_counter--;}
    }
}

void ThreadPool::terminate(){
    ZoneScoped;
    {
        write_lock _(m_rw_lock);
        if (working_unsafe()){
            ZoneScopedN( "terminated 1" );
            m_terminated = true;
        }
        else{
            ZoneScopedN( "terminated 2" );
            m_workers.clear();
            m_terminated = false;
            m_initialized = false;
            return;
        }
    }
    m_task_waiter.notify_all();
    for (std::thread& worker : m_workers){
        ZoneScopedN( "terminated 3" );
        worker.join();
    }
    m_workers.clear();
    m_terminated = false;
    m_initialized = false;
}