#ifndef TASK_QUEUE_TPP
#define TASK_QUEUE_TPP


#include "TaskQueue.h"

template <typename task_type_t>
bool TaskQueue<task_type_t>::empty() const{
    read_lock _(m_rw_lock);
    return m_tasks.empty();
}
template <typename task_type_t>
size_t TaskQueue<task_type_t>::size() const{
    read_lock _(m_rw_lock);
    return m_tasks.size();
}

template <typename task_type_t>
void TaskQueue<task_type_t>::clear(){
    write_lock _(m_rw_lock);
    while (!m_tasks.empty()){
        m_tasks.pop();
    }
}
template <typename task_type_t>
bool TaskQueue<task_type_t>::pop(task_type_t& task){
    write_lock _(m_rw_lock);
    if (m_tasks.empty()){
        return false;
    }
    else{
        task = std::move(m_tasks.front());
        m_tasks.pop();
        return true;
    }
}

template <typename task_type_t>
template <typename... arguments>
void TaskQueue<task_type_t>::emplace(arguments&&... parameters){
    write_lock _(m_rw_lock);
    m_tasks.emplace( std::forward<arguments>(parameters)...);
}

#endif // TASK_QUEUE_TPP