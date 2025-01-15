#include <iostream>
#include "Server.h"
#include "data_structure/InvertedIndex.h"
#include "data_structure/ThreadSafeHashMap.h"

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
//    Server server(8080);
//    if (server.initialize()) {
//        std::cout << "Server initialized.\n";
//        server.acceptConnections();
//    } else {
//        std::cerr << "Server initialization failed.\n";
//    }
//    return 0;

    return 0;
}