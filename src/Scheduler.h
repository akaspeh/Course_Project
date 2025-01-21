//
// Created by PC on 18.01.2025.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FileStorageManager.h"
#include "set"
#include "data_structure/InvertedIndex.h"
#include "data_structure/ThreadPool.h"


class Scheduler {
private:
    ThreadPool m_thread_pool;
    InvertedIndex m_inverted_index;
    FileStorageManager m_file_storage_manager;
public:
    Scheduler(size_t threads_count = std::thread::hardware_concurrency()/2) {
        m_thread_pool.initialize(threads_count);
    };
    bool file_path_add(const std::string& file_path);
    bool file_upload(const std::string& file_name, const std::string& contnent);
    bool delete_file(const std::string& file_name);
    std::set<std::string> search(const std::string& search_term);
    void build_index();
};

#endif //SCHEDULER_H
