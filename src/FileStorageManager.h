//
// Created by PC on 13.01.2025.
//

#ifndef COURSE_PROJECT_FILESTORAGEMANAGER_H
#define COURSE_PROJECT_FILESTORAGEMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <shared_mutex>
#include <experimental/filesystem>

class FileStorageManager {
private:
    std::string m_file_storage_path;
    std::shared_mutex m_rw_lock;
public:
    FileStorageManager(const std::string& path);
    bool save_file(const std::string& filename, const std::string& content);
    std::string get_content(const std::string& filename);
    std::vector<std::string> get_all_files();
    bool delete_file(const std::string& filename);
};


#endif //COURSE_PROJECT_FILESTORAGEMANAGER_H
