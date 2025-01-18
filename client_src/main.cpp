

#pragma comment(lib, "Ws2_32.lib")

#include "Client.h"


void stressTest(const std::string& serverAddress, uint32_t serverPort, int numClients, int numOperations) {
    std::vector<std::thread> threads;
    std::atomic<int> successfulRequests(0);  // Счётчик успешных запросов
    std::atomic<int> failedRequests(0);      // Счётчик неудачных запросов
    std::mutex outputMutex;  // Мьютекс для синхронизации вывода в консоль
    std::cout << "Start" << std::endl;
    // Функция для выполнения операций каждым клиентом
    auto clientTask = [&]() {
        Client client(serverAddress, serverPort);

        for (int i = 0; i < numOperations; ++i) {
            try {
                auto start = std::chrono::high_resolution_clock::now();

                // Выбираем случайную операцию для выполнения
                int operation = rand() % 3;
                std::string fileName = "file_" + std::to_string(rand() % 1000) + ".txt";
                std::string content = "Test content for " + fileName;
                std::string searchTerm = "test";

                if (!client.connectToServer()) {
                    failedRequests++;
                    std::cout << "fail" << std::endl;
                    return;
                }
                switch (operation) {
                    case 0:  // Загрузка файла
                        client.uploadFile(fileName, content);
                        break;
                    case 1:  // Поиск термина
                        client.searchTerm(searchTerm);
                        break;
                    case 2:  // Удаление файла
                        client.deleteFile(fileName);
                        break;
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;

                // Логируем успешное выполнение операции
                {
                    std::lock_guard<std::mutex> lock(outputMutex);
                    successfulRequests++;
                    std::cout << "Operation completed in: " << duration.count() << " seconds" << std::endl;
                }
            }
            catch (...) {
                failedRequests++;
            }
        }
    };

    // Создаём потоки для выполнения теста
    for (int i = 0; i < numClients; ++i) {
        threads.push_back(std::thread(clientTask));
    }

    // Ожидаем завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    // Выводим статистику
    std::cout << "Stress Test Completed" << std::endl;
    std::cout << "Total successful requests: " << successfulRequests.load() << std::endl;
    std::cout << "Total failed requests: " << failedRequests.load() << std::endl;
}

int main() {
//    std::string serverAddress = "127.0.0.1";  // IP адрес сервера
//    uint32_t serverPort = 18080;  // Порт сервера
//
//    int numClients = 100;  // Количество клиентов
//    int numOperations = 10;  // Количество операций на каждого клиента
//
//    // Запуск стресс-теста
//    stressTest(serverAddress, serverPort, numClients, numOperations);
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