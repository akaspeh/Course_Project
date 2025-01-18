//
// Created by PC on 18.01.2025.
//

#ifndef SERVER_LINUX_H
#define SERVER_LINUX_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include <functional>
#include <fstream>
#include <string>
#include <filesystem>
#include "../src/Scheduler.h"

#include "../src/data_structure/ThreadPool.h"
#include "../src/utils/Response.h"
#include "../src/utils/Request.h"

class Server_linux {
private:
    int m_serverSocket;
    struct sockaddr_in m_serverAddr;
    uint32_t m_port;

    Scheduler m_scheduler;
    ThreadPool m_thread_pool;

public:
    Server_linux(uint32_t port, const std::string& file_path, size_t threads_count = std::thread::hardware_concurrency() - 1);
    ~Server_linux();
    bool initialize();
    void handle_request(int client_socket);
    void accept_connections();
    Response handle_file_upload(const Request& request);
    Response handle_delete_file_request(const Request& request);
    Response handle_search_request(const Request& request);
};



#endif //SERVER_LINUX_H
