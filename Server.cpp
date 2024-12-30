
#include "Server.h"

bool Server::initialize(){
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return false;
    }

    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return false;
    }

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverAddr.sin_port = htons(m_port);
    if (bind(m_serverSocket, (SOCKADDR*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(m_serverSocket);
        WSACleanup();
        return false;
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(m_serverSocket);
        WSACleanup();
        return false;
    }

    return true;
}

std::string Server::readHtmlFile(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Server::endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && str.find(suffix, str.size() - suffix.size()) != std::string::npos;
}

std::string Server::getContentType(const std::string& path) {
    if (endsWith(path, ".html")) return "text/html";
    if (endsWith(path, ".css")) return "text/css";
    if (endsWith(path, ".js")) return "application/javascript";
    return "text/plain";
}

void Server::sendResponse(SOCKET clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), response.size(), 0);
}

void Server::handleRequest(SOCKET clientSocket) {
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::istringstream request(buffer);
        std::string method, path, version;
        request >> method >> path >> version;

        if (path == "/") {
            path = "/index.html";
        }

        std::string filesPath = "C:/Users/PC/CLionProjects/WebServerHTTP" + path;

        if (method == "GET") {
            std::string content = readHtmlFile(filesPath);
            std::string contentType = getContentType(filesPath);

            if (!content.empty()) {
                std::string httpResponse =
                        "HTTP/1.1 200 OK\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
                        std::to_string(content.size()) + "\r\n\r\n" + content;
                sendResponse(clientSocket, httpResponse);
            } else {
                std::string httpResponse = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                sendResponse(clientSocket, httpResponse);
            }
        }
    }
}

void Server::acceptConnections() {
    while (true) {
        // Accept connection
        SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed.\n";
            continue;
        }


        std::thread(&Server:: handleRequest, this, clientSocket).detach();
        std::cout << "Accepted Client: " << clientSocket << "\n";
    }
}

Server::~Server() {
    // Clean up
    closesocket(m_serverSocket);
    WSACleanup();
}