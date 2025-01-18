//
// Created by PC on 18.01.2025.
//

#include "Server_linux.h"

Server_linux::Server_linux(uint32_t port, const std::string& file_path, size_t threads_count) : m_port(port) {
    m_thread_pool.initialize(threads_count);
    m_scheduler.file_path_add(file_path);
}

// Деструктор
Server_linux::~Server_linux() {
    // Закрытие сокета
    close(m_serverSocket);
}

// Инициализация сервера
bool Server_linux::initialize() {
    // Создание сокета
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == -1) {
        std::cerr << "Socket creation failed.\n";
        return false;
    }

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Любой IP
    m_serverAddr.sin_port = htons(m_port); // Устанавливаем порт

    // Привязка сокета к адресу
    if (bind(m_serverSocket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == -1) {
        std::cerr << "Bind failed.\n";
        close(m_serverSocket);
        return false;
    }

    // Ожидание подключений
    if (listen(m_serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Listen failed.\n";
        close(m_serverSocket);
        return false;
    }

    return true;
}

// Обработка запроса клиента
void Server_linux::handle_request(int client_socket) {
    try {
        // Получаем размер данных (4 байта)
        uint32_t data_length = 0;

        int bytes_received = recv(client_socket, reinterpret_cast<char*>(&data_length), sizeof(data_length), 0);

        if (bytes_received == sizeof(data_length)) {
            // Выделяем память для получения данных
            std::unique_ptr<char[]> buffer(new char[data_length + 1]);  // +1 для null-терминатора

            // Получаем данные
            uint32_t total_received = 0;
            while (total_received < data_length) {
                int received = recv(client_socket, buffer.get() + total_received, data_length - total_received, 0);
                if (received <= 0) {
                    std::cerr << "Error receiving data or client disconnected. Bytes received: " << received << std::endl;
                    return;
                }
                total_received += received;
            }

            buffer[total_received] = '\0';

            std::string request_data(buffer.get(), total_received);  // Конструируем строку из байтового буфера

            if (request_data.empty()) {
                std::cerr << "Error: Received empty request data." << std::endl;
                return;
            }

            // Извлекаем тип запроса (первый байт)
            uint8_t request_type_int = static_cast<uint8_t>(request_data[0]);
            if (request_type_int < 0 || request_type_int > 2) {
                std::cerr << "Error: Invalid request type." << std::endl;
                return;
            }

            // Создаём объект запроса
            Request request{static_cast<RequestType>(request_type_int), request_data.substr(1)};

            // Обработка запроса
            Response response;
            switch (request.type) {
                case RequestType::FUPLOAD:
                    response = handle_file_upload(request);
                    break;
                case RequestType::FSEARCH:
                    response = handle_search_request(request);
                    break;
                case RequestType::FDELETE:
                    response = handle_delete_file_request(request);
                    break;
                default:
                    response = {400, std::string("Unknown request type.")};
                    std::cerr << "WARNING: Received unknown request type: " << static_cast<int>(request.type) << std::endl;
                    break;
            }

            // Создаём строку ответа
            std::string response_str = std::to_string(response.status_code) + "\n" + response.data;

            // Отправляем длину ответа
            uint32_t response_length = response_str.length();
            send(client_socket, reinterpret_cast<char*>(&response_length), sizeof(response_length), 0);  // Отправляем длину ответа
            send(client_socket, response_str.c_str(), response_length, 0);  // Отправляем сам ответ

        } else {
            std::cerr << "Error: Failed to receive valid data length or connection issue." << std::endl;
        }
        close(client_socket);  // Закрываем сокет клиента

    } catch (const std::exception& ex) {
        std::cerr << "Exception while handling request: " << ex.what() << std::endl;
    }
}

// Ожидание подключений
void Server_linux::accept_connections() {
    while (true) {
        int client_socket = accept(m_serverSocket, NULL, NULL);

        // Проверка ошибок при принятии подключения
        if (client_socket == -1) {
            std::cerr << "Accept failed with error: " << strerror(errno) << "\n";
            continue;  // Пытаемся принять следующее подключение
        }

        // Обрабатываем запрос клиента в отдельном потоке
        m_thread_pool.add_task(std::bind(&Server_linux::handle_request, this, client_socket));
    }
}

// Обработка запроса на загрузку файла
Response Server_linux::handle_file_upload(const Request& request) {
    try {
        size_t separator_pos = request.data.find('\n');
        if (separator_pos == std::string::npos) {
            return {400, "Invalid request format. Expected: <filename>\n<content>"};
        }

        std::string file_name = request.data.substr(0, separator_pos);
        std::string file_content = request.data.substr(separator_pos + 1);

        if (file_name.empty() || file_content.empty()) {
            return {400, "Filename or content is empty."};
        }

        if (m_scheduler.file_upload(file_name,file_content)) {
            return {200, "File uploaded successfully."};
        } else {
            return {500, "Failed to save file."};
        }
    } catch (const std::exception& ex) {
        return {500, "Server error: " + std::string(ex.what())};
    }
}

// Обработка запроса на удаление файла
Response Server_linux::handle_delete_file_request(const Request& request) {
    try {
        // The request data contains the file name to delete
        std::string file_name = request.data;
        if (m_scheduler.delete_file(file_name)) {
            return {200, std::string("File deleted successfully.")};
        } else {
            return {404, std::string("File not found.")};
        }
    } catch (const std::exception& ex) {
        return {500, std::string("Server error: ") + ex.what()};
    }
}

// Обработка запроса на поиск
Response Server_linux::handle_search_request(const Request& request) {
    try {
        // The request data contains the search term
        std::string search_term = request.data;
        // Search the term in the inverted index
        std::set<std::string> results = m_scheduler.search(search_term);

        if (results.empty()) {
            return {404, "No files found for the given term."};
        }

        // Format the response as a newline-separated list of file names
        std::string response_data;
        for (const auto& file_name : results) {
            response_data += file_name + "\n";
        }

        return {200, response_data};
    } catch (const std::exception& ex) {
        return {500, std::string("Server error: ") + ex.what()};
    }
}


// Очистка файлов из директории
