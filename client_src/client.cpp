#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>

#pragma comment(lib, "Ws2_32.lib")

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

Client::Client(const std::string& serverAddress, uint32_t serverPort)
        : m_serverAddress(serverAddress), m_serverPort(serverPort) {}

bool Client::connectToServer() {
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return false;
    }

    m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(m_serverAddress.c_str());

    if (connect(m_clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        closesocket(m_clientSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool Client::sendRequest(RequestType type, const std::string& data) {
    std::stringstream requestStream;
    requestStream << static_cast<uint8_t>(type) << data;

    std::string requestData = requestStream.str();
    if (send(m_clientSocket, requestData.c_str(), requestData.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed.\n";
        return false;
    }

    return true;
}

void Client::uploadFile(const std::string& file_name, const std::string& content) {
    std::string requestData = file_name + "\n" + content;
    if (sendRequest(RequestType::FUPLOAD, requestData)) {
        std::cout << "File uploaded: " << file_name << std::endl;
    } else {
        std::cerr << "Failed to upload file.\n";
    }
}

void Client::searchTerm(const std::string& term) {
    if (sendRequest(RequestType::FSEARCH, term)) {
        char buffer[1024];
        int bytesReceived = recv(m_clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  // Null-terminate the string
            std::cout << "Search results: " << buffer << std::endl;
        } else {
            std::cerr << "Error receiving search results.\n";
        }
    } else {
        std::cerr << "Failed to send search request.\n";
    }
}

void Client::deleteFile(const std::string& file_name) {
    if (sendRequest(RequestType::FDELETE, file_name)) {
        std::cout << "File deleted: " << file_name << std::endl;
    } else {
        std::cerr << "Failed to delete file.\n";
    }
}

Client::~Client() {
    closesocket(m_clientSocket);
    WSACleanup();
}

int main() {
    std::string serverAddress = "127.0.0.1";  // Change if necessary
    uint32_t serverPort = 18080;               // Change if necessary

    Client client(serverAddress, serverPort);

    if (!client.connectToServer()) {
        std::cerr << "Connection to server failed.\n";
        return -1;
    }

    // Test upload file
    std::string file_name = "file_to_upload.txt";
    std::string file_content = "This is a test file content.";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client.uploadFile(file_name, file_content);

    // Test search term
    std::string search_term = "test";
    if (!client.connectToServer()) {
        std::cerr << "Connection to server failed.\n";
        return -1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client.searchTerm(search_term);
    if (!client.connectToServer()) {
        std::cerr << "Connection to server failed.\n";
        return -1;
    }
    // Test delete file
    client.deleteFile(file_name);

    if (!client.connectToServer()) {
        std::cerr << "Connection to server failed.\n";
        return -1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client.searchTerm(search_term);

    return 0;
}