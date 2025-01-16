
#include "Server.h"

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cassert>
#include <atomic>
#include <random>
#include <barrier>

void print_results(const std::string& term, const std::set<std::string>& results) {
    std::cout << "Search results for term \"" << term << "\": ";
    if (results.empty()) {
        std::cout << "No documents found.";
    } else {
        for (const auto& doc : results) {
            std::cout << doc << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    Server server(18080, "storage/");
    server.add_files_from_directory("Files/");
    if (server.initialize()) {
        std::cout << "Server initialized.\n";
        server.accept_connections();
    } else {
        std::cerr << "Server initialization failed.\n";
    }
    return 0;
}