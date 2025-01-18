//
// Created by PC on 16.01.2025.
//

#include "Client.h"

Client::Client(const std::string& serverAddress, uint32_t serverPort)
        : m_serverAddress(serverAddress), m_serverPort(serverPort) {}

bool Client::connectToServer() {
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return false;
    }

    m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(m_serverAddress.c_str());

    if (connect(m_clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(m_clientSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool Client::sendRequest(RequestType type, const std::string& data) {
    // Преобразуем тип запроса в целое число и формируем строку запроса
    std::stringstream requestStream;
    requestStream << static_cast<uint8_t>(type) << data;

    std::string requestData = requestStream.str();

    // Получаем длину данных
    uint32_t dataLength = requestData.size();
    // Отправляем размер данных (4 байта)
    if (send(m_clientSocket, reinterpret_cast<char*>(&dataLength), sizeof(dataLength), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send data length." << std::endl;
        return false;
    }

    // Отправляем сами данные
    if (send(m_clientSocket, requestData.c_str(), dataLength, 0) == SOCKET_ERROR) {
        std::cerr << "Send failed." << std::endl;
        return false;
    }

    return true;
}

void Client::uploadFile(const std::string& file_name, const std::string& content) {
    // Формируем данные запроса
    std::string requestData = file_name + "\n" + content;

    // Отправляем запрос
    if (sendRequest(RequestType::FUPLOAD, requestData)) {
        // Получаем размер ответа
        uint32_t data_length;
        int bytes_received = recv(m_clientSocket, reinterpret_cast<char*>(&data_length), sizeof(data_length), 0);

        if (bytes_received == sizeof(data_length)) {
            // Получаем сами данные
            std::unique_ptr<char[]> buffer(new char[data_length + 1]);  // +1 для null-терминатора
            uint32_t total_received = 0;
            while (total_received < data_length) {
                int received = recv(m_clientSocket, buffer.get() + total_received, data_length - total_received, 0);
                if (received <= 0) {
                    std::cerr << "Error receiving data or client disconnected." << std::endl;
                    return;
                }
                total_received += received;
            }

            buffer[total_received] = '\0';  // Null-терминируем строку
            std::cout << "Upload response: " << buffer.get() << std::endl;
        } else {
            std::cerr << "Failed to receive valid data length or connection issue." << std::endl;
        }
    } else {
        std::cerr << "Failed to upload file." << std::endl;
    }
}

void Client::searchTerm(const std::string& term) {
    if (sendRequest(RequestType::FSEARCH, term)) {
        // Получаем размер ответа
        uint32_t data_length;
        int bytes_received = recv(m_clientSocket, reinterpret_cast<char*>(&data_length), sizeof(data_length), 0);

        if (bytes_received == sizeof(data_length)) {
            // Получаем сами данные
            std::unique_ptr<char[]> buffer(new char[data_length + 1]);  // +1 для null-терминатора
            uint32_t total_received = 0;
            while (total_received < data_length) {
                int received = recv(m_clientSocket, buffer.get() + total_received, data_length - total_received, 0);
                if (received <= 0) {
                    std::cerr << "Error receiving data or client disconnected." << std::endl;
                    return;
                }
                total_received += received;
            }

            buffer[total_received] = '\0';  // Null-терминируем строку
            std::cout << "Search results: " << buffer.get() << std::endl;
        } else {
            std::cerr << "Failed to receive valid data length or connection issue." << std::endl;
        }
    } else {
        std::cerr << "Failed to send search request." << std::endl;
    }
}

void Client::deleteFile(const std::string& file_name) {
    if (sendRequest(RequestType::FDELETE, file_name)) {
        // Получаем размер ответа
        uint32_t data_length;
        int bytes_received = recv(m_clientSocket, reinterpret_cast<char*>(&data_length), sizeof(data_length), 0);

        if (bytes_received == sizeof(data_length)) {
            // Получаем сами данные
            std::unique_ptr<char[]> buffer(new char[data_length + 1]);  // +1 для null-терминатора
            uint32_t total_received = 0;
            while (total_received < data_length) {
                int received = recv(m_clientSocket, buffer.get() + total_received, data_length - total_received, 0);
                if (received <= 0) {
                    std::cerr << "Error receiving data or client disconnected." << std::endl;
                    return;
                }
                total_received += received;
            }

            buffer[total_received] = '\0';  // Null-терминируем строку
            std::cout << "Delete file response: " << buffer.get() << std::endl;
        } else {
            std::cerr << "Failed to receive valid data length or connection issue." << std::endl;
        }
    } else {
        std::cerr << "Failed to send delete file request." << std::endl;
    }
}


Client::~Client(){
    closesocket(m_clientSocket);
    WSACleanup();
}