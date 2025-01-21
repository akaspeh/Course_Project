//
// Created by PC on 13.01.2025.
//

#include "FileStorageManager.h"

#include <iostream>
#include <fstream>
#include <shared_mutex>
#include <mutex>


bool FileStorageManager::init_path(const std::string &filepath) {
    m_file_storage_path = filepath;
    return std::experimental::filesystem::create_directory(m_file_storage_path);
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

bool FileStorageManager::clear_path() {
    std::unique_lock<std::shared_mutex> lock(m_rw_lock);
    namespace fs = std::filesystem;

    try {
        fs::path dir_path(m_file_storage_path);

        // is path exist
        if (!fs::exists(dir_path)) {
            std::cerr << "do not exist: " << m_file_storage_path << std::endl;
            return false;
        }

        // is path directory
        if (!fs::is_directory(dir_path)) {
            std::cerr << "not a directory directory: " << m_file_storage_path << std::endl;
            return false;
        }

        // delete directory
        for (const auto& entry : fs::directory_iterator(dir_path)) {
            fs::remove_all(entry.path());
        }

        std::cout << "directory cleared: " << m_file_storage_path << std::endl;
        return true;

    } catch (const fs::filesystem_error& e) {
        std::cerr << "File system error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
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

bool FileStorageManager::search_file(const std::string& filename) {
    std::shared_lock<std::shared_mutex> lock(m_rw_lock);
    for (const auto& entry : std::experimental::filesystem::directory_iterator(m_file_storage_path)) {
        if (std::experimental::filesystem::is_regular_file(entry) && entry.path().filename() == filename) {
            return true; // File found
        }
    }
    return false; // File not found
}