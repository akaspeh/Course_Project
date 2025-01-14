//
// Created by PC on 13.01.2025.
//

#include "FileStorageManager.h"

#include <iostream>
#include <fstream>
#include <shared_mutex>
#include <mutex>

FileStorageManager::FileStorageManager(const std::string& path) : m_file_storage_path(path) {
    std::experimental::filesystem::create_directory(m_file_storage_path);
}

bool FileStorageManager::save_file(const std::string& filename, const std::string& content) {
    std::unique_lock<std::shared_mutex> lock(m_rw_lock);
    std::ofstream file(m_file_storage_path + filename);
    if (file.is_open()) {
        file << content;
        return true;
    }
        return false;
}

std::string FileStorageManager::get_content(const std::string& filename) {
    std::shared_lock<std::shared_mutex> lock(m_rw_lock);
    std::ifstream file(m_file_storage_path + filename);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return "";
}

std::vector<std::string> FileStorageManager::get_all_files() {
    std::shared_lock<std::shared_mutex> lock(m_rw_lock);
    std::vector<std::string> files;
    for (const auto& entry : std::experimental::filesystem::directory_iterator(m_file_storage_path)) {
        if (std::experimental::filesystem::is_regular_file(entry)) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

bool FileStorageManager::delete_file(const std::string& filename) {
    std::unique_lock<std::shared_mutex> lock(m_rw_lock);
    std::error_code ec;
    if (std::experimental::filesystem::remove(m_file_storage_path + filename, ec)) {
        return true;
    }
    return false;
}