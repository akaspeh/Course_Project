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
    InvertedIndex index;

    // Add documents to the index
    index.add_document("doc1.txt", "The quick brown fox jumps over the lazy dog");
    index.add_document("doc2.txt", "The quick fox jumped over the fence");
    index.add_document("doc3.txt", "Lazy dogs are quick and brown");

    // Test the search functionality
    std::set<std::string> results = index.search("fence");
    std::set<std::string> results2 = index.search("lazy");
    std::set<std::string> results3 = index.search("dogs");

    print_results("fence", results);
    print_results("lazy", results2);
    print_results("dogs", results3);

    return 0;
}