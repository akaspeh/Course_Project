#include <iostream>

#include "Server.h"

void test_removal_from_inverted_index() {
    InvertedIndex index;

    // Шаг 1: Добавляем документ
    std::string file_name = "test_file.txt";
    std::string content = "apple banana cherry apple";
    index.add_document(file_name, content);

    // Проверяем, что токены добавлены
    std::set<std::string> result_apple = index.search("apple");
    std::set<std::string> result_banana = index.search("banana");
    std::set<std::string> result_cherry = index.search("cherry");

    if (result_apple.find(file_name) != result_apple.end() &&
        result_banana.find(file_name) != result_banana.end() &&
        result_cherry.find(file_name) != result_cherry.end()) {
        std::cout << "Тест добавления прошёл: файл добавлен для всех токенов.\n";
    } else {
        std::cerr << "Ошибка: файл не добавлен для одного из токенов.\n";
        return;
    }

    // Шаг 2: Удаляем документ
    index.remove_document(file_name, content);

    // Шаг 3: Проверяем, что токены удалены
    result_apple = index.search("apple");
    result_banana = index.search("banana");
    result_cherry = index.search("cherry");

    if (result_apple.empty() && result_banana.empty() && result_cherry.empty()) {
        std::cout << "Тест удаления прошёл: файл успешно удалён из всех токенов.\n";
    } else {
        std::cerr << "Ошибка: файл всё ещё существует в одном из токенов.\n";
    }
}

int main() {
//    test_removal_from_inverted_index();
    Server server(18080, "src/storage/");
//    server.add_files_from_directory("Files/");
//    server.clear_files_from_directory("storage/");
    if (server.initialize()) {
        std::cout << "Server initialized.\n";
        server.accept_connections();
    } else {
        std::cerr << "Server initialization failed.\n";
    }
    return 0;
}