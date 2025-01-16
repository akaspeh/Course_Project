
#include "Server.h"

Server::Server(uint32_t port, const std::string& file_path, size_t threads_count): m_port(port){
    m_thread_pool.initialize(threads_count);
    m_file_storage_manager.init_path(file_path);
}

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


void Server::handle_request(SOCKET client_socket) {
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        auto request_type_int = static_cast<std::underlying_type_t<RequestType>>(buffer[0]);
        std::string request_data;
        if (bytes_received > 1) {
            request_data = std::string(buffer + 1, bytes_received - 1);
        }

        Request request{static_cast<RequestType>(request_type_int), request_data};
        std::cout << "INFO: Received request of type " << static_cast<int>(request.type);

        Response response;
//        switch (request.type) {
//            case RequestType::FUPLOAD:
//                response = handle_file_upload(request);
//                break;
//            case RequestType::FSEARCH:
//                response = handle_search_request(request);
//                break;
//            case RequestType::FDELETE:
//                response = handle_delete_file_request(request);
//                break;
//            default:
//                response = {400, "Unknown request type."};
//                std::cout << "WARNING: Received unknown request type: " << static_cast<int>(request.type) << std::endl;
//                break;
//        }
        std::string response_str = std::to_string(response.status_code) + "\n" + response.data;
        send(client_socket, response_str.c_str(), response_str.length(), 0);
        std::cout << "INFO: Sent response to client." << std::endl;
    } else if (bytes_received == 0) {
        std::cout << "INFO: Client disconnected." << std::endl;
    } else {
        perror("recv failed");
    }
    std::cout << "Client disconnected: " << client_socket << "\n";
    closesocket(client_socket); // Close the client socket
}

void Server::accept_connections() {
    std::cout << "Starting to accept connections.." << std::endl;
    while (true) {
        SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);

        // Check for any errors in accepting the connection
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << "\n";
            continue;  // Try to accept the next connection
        }

        // Log when a client is successfully accepted
        std::cout << "Accepted Client: " << clientSocket << "\n";

        // Handle the client request in a new thread
        m_thread_pool.add_task(std::bind(&Server::handle_request, this, clientSocket));
    }
}





Response Server::handle_file_upload(const Request &request) {
    try {
        // Extract filename and content from request data
        size_t separator_pos = request.data.find('\n');
        if (separator_pos == std::string::npos) {
            return {400, "Invalid request format. Expected: <filename>\\n<content>"};
        }

        std::string file_name = request.data.substr(0, separator_pos);
        std::string file_content = request.data.substr(separator_pos + 1);

        // Save the file using FileStorageManager
        if (m_file_storage_manager.save_file(file_name, file_content)) {
            // Update the inverted index
            m_inverted_index.add_document(file_name, file_content);
            return {200, "File uploaded successfully."};
        } else {
            return {500, "Failed to save file."};
        }
    } catch (const std::exception& ex) {
        return {500, std::string("Server error: ") + ex.what()};
    }
}
Response Server::handle_delete_file_request(const Request& request){
    try {
        // The request data contains the file name to delete
        std::string file_name = request.data;

        // Delete the file using FileStorageManager
        if (m_file_storage_manager.delete_file(file_name)) {
            // Optionally, remove the file from the inverted index
            // Assuming the inverted index has a method for removing a document
            std::string content = m_file_storage_manager.get_content(file_name);
            m_inverted_index.remove_document(file_name, content); // Ensure this method is implemented
            return {200, "File deleted successfully."};
        } else {
            return {404, "File not found."};
        }
    } catch (const std::exception& ex) {
        return {500, std::string("Server error: ") + ex.what()};
    }
}
Response Server::handle_search_request(const Request& request){
    try {
        // The request data contains the search term
        std::string search_term = request.data;

        // Search the term in the inverted index
        std::set<std::string> results = m_inverted_index.search(search_term);

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

void Server::add_files_from_directory(const std::string& directory_path) {
    namespace fs = std::filesystem;

    try {
        // Используем recursive_directory_iterator для рекурсивного обхода всех файлов и папок
        for (const auto& entry : fs::recursive_directory_iterator(directory_path)) {
            if (fs::is_regular_file(entry.status())) {
                std::string file_name = entry.path().filename().string();
                std::ifstream file(entry.path());

                if (!file.is_open()) {
                    std::cerr << "Не удалось открыть файл: " << file_name << std::endl;
                    continue;
                }

                std::ostringstream file_stream;
                file_stream << file.rdbuf();
                std::string file_content = file_stream.str();

                if (!m_file_storage_manager.save_file(file_name, file_content)) {
                    std::cerr << "Не удалось сохранить файл в хранилище: " << file_name << std::endl;
                    continue;
                }
                
                m_inverted_index.add_document(file_name, file_content);

            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при обработке папки: " << e.what() << std::endl;
    }
}

Server::~Server() {
    // Clean up
    closesocket(m_serverSocket);
    WSACleanup();
}