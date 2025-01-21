//
// Created by PC on 18.01.2025.
//

#include "Scheduler.h"


bool Scheduler::file_path_add(const std::string& file_path) {
    return m_file_storage_manager.init_path(file_path);
}

void Scheduler::build_index() {
    // Get all files from storage in the given path
    std::vector<std::string> files = m_file_storage_manager.get_all_files();

    // Iterate over each file
    for (const auto& file_name : files) {
        // Get file content
        std::string content = m_file_storage_manager.get_content(file_name);
        // Add content to the inverted index
        m_thread_pool.add_task(std::bind(&InvertedIndex::add_document, std::ref(m_inverted_index), file_name, content));
    }
    std::cerr << "Index has builded" << std::endl;
}

bool Scheduler::file_upload(const std::string& file_name, const std::string& content) {
    if (m_file_storage_manager.search_file(file_name)) {
        std::cerr << "Files with equals Names" << std::endl;
        std::string content_from_old_file = m_file_storage_manager.get_content(file_name);
        m_thread_pool.add_task(std::bind(&InvertedIndex::remove_document, std::ref(m_inverted_index), file_name, content_from_old_file));
        if (m_file_storage_manager.save_file(file_name, content)) {
            m_thread_pool.add_task(std::bind(&InvertedIndex::add_document, std::ref(m_inverted_index), file_name, content));
        }
        return true;
    }
    if (m_file_storage_manager.save_file(file_name, content)) {
        // Update the inverted index
        m_thread_pool.add_task(std::bind(&InvertedIndex::add_document, std::ref(m_inverted_index), file_name, content));
        return true;
    }
    return false;
}

// Method to handle file deletion requests
bool Scheduler::delete_file(const std::string& file_name) {
    // Delete the file using FileStorageManager
    std::string content = m_file_storage_manager.get_content(file_name);
    if (m_file_storage_manager.delete_file(file_name)) {
        m_thread_pool.add_task(std::bind(&InvertedIndex::remove_document,  std::ref(m_inverted_index), file_name, content));
        return true;
    }
    return false;
}

// Method to handle search requests
std::set<std::string> Scheduler::search(const std::string& search_term){
    std::set<std::string> results = m_inverted_index.search(search_term);
    return results;
}