//
// Created by PC on 16.01.2025.
//

#ifndef COURSE_CLIENT_CLIENT_H
#define COURSE_CLIENT_CLIENT_H

#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>

enum class RequestType : uint8_t {
    FUPLOAD = 0,
    FSEARCH = 1,
    FDELETE = 2
};

class Client {
private:
    WSADATA m_wsaData;
    SOCKET m_clientSocket;
    std::string m_serverAddress;
    uint32_t m_serverPort;

public:
    Client(const std::string& serverAddress, uint32_t serverPort);
    bool connectToServer();
    bool sendRequest(RequestType type, const std::string& data);
    void uploadFile(const std::string& file_name, const std::string& content);
    void searchTerm(const std::string& term);
    void deleteFile(const std::string& file_name);
    ~Client();
};


#endif //COURSE_CLIENT_CLIENT_H
