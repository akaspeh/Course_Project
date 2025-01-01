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
#include <string>

#include "data_structure/ThreadPool.h"

#pragma comment(lib, "Ws2_32.lib")

class Server {
private:
    WSADATA m_wsaData;
    SOCKET m_serverSocket;
    SOCKADDR_IN m_serverAddr;
    ThreadPool m_thread_pool;
    int m_port;
public:
    Server(int port) : m_port(port){m_thread_pool.initialize(std::thread::hardware_concurrency());}
    bool initialize();
    std::string readHtmlFile(const std::string& filePath);
    void sendResponse(SOCKET clientSocket, const std::string& response);
    std::string getContentType(const std::string& path);
    bool endsWith(const std::string& str, const std::string& suffix);
    void handleRequest(SOCKET clientSocket);
    void acceptConnections();
    ~Server();
};


#endif //COURSE_PROJECT_SERVER_H
