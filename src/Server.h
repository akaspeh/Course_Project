//
// Created by PC on 29.12.2024.
//

#ifndef COURSE_PROJECT_SERVER_H
#define COURSE_PROJECT_SERVER_H

#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include <functional>
#include <fstream>
#include <string>
#include <filesystem>

#include "../src/data_structure/ThreadPool.h"
#include "../src/data_structure/InvertedIndex.h"
#include "../src/FileStorageManager.h"
#include "../src/utils/Response.h"
#include "../src/utils/Request.h"

#pragma comment(lib, "Ws2_32.lib")

class Server {
private:
    WSADATA m_wsaData;
    SOCKET m_serverSocket;
    SOCKADDR_IN m_serverAddr;
    uint32_t m_port;

    FileStorageManager m_file_storage_manager;
    InvertedIndex m_inverted_index;
    ThreadPool m_thread_pool;

public:
    Server(uint32_t port, const std::string& file_path, size_t threads_count = std::thread::hardware_concurrency()-1);
    ~Server();
    bool initialize();
    void handle_request(SOCKET client_socket);
    void accept_connections();
    Response handle_file_upload(const Request& request);
    Response handle_delete_file_request(const Request& request);
    Response handle_search_request(const Request& request);
    void add_files_from_directory(const std::string& directory_path);
    void clear_files_from_directory(const std::string& directory_path);
};


#endif //COURSE_PROJECT_SERVER_H
