//
// Created by PC on 29.12.2024.
//

#ifndef COURSE_PROJECT_SERVER_H
#define COURSE_PROJECT_SERVER_H


#include <winsock2.h>
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include <functional>
#include <fstream>
#include <string>
#include <filesystem>

#include "../src/data_structure/ThreadPool.h"
#include "../src/Scheduler.h"
#include "../src/utils/Response.h"
#include "../src/utils/Request.h"

#pragma comment(lib, "Ws2_32.lib")

class Server_win {

private:
    WSADATA m_wsaData;
    SOCKET m_serverSocket;
    SOCKADDR_IN m_serverAddr;
    uint32_t m_port;

    Scheduler m_scheduler;
    ThreadPool m_thread_pool;

public:
    Server_win(uint32_t port, const std::string& file_path,
        size_t threads_count = std::thread::hardware_concurrency()/2, bool build_index_from_storage = 1);
    ~Server_win();
    bool initialize();
    void handle_request(SOCKET client_socket);
    void accept_connections();
    Response handle_file_upload(const Request& request);
    Response handle_delete_file_request(const Request& request);
    Response handle_search_request(const Request& request);
};


#endif //COURSE_PROJECT_SERVER_H
