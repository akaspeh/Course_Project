#include <iostream>
#include "Server.h"
#include "data_structure/ThreadSafeHashMap.h"

//int main() {
//    Server server(8080);
//    if (server.initialize()) {
//        std::cout << "Server initialized.\n";
//        server.acceptConnections();
//    } else {
//        std::cerr << "Server initialization failed.\n";
//    }
//    return 0;
//}
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cassert>
#include <atomic>
#include <random>
#include <barrier>

int main() {
    ThreadSafeHashMap<int, std::string> map;

    // Stress Test
    const int NUM_THREADS = 10;
    const int NUM_OPERATIONS = 1000;
    std::vector<std::thread> threads;

    // Barrier for synchronizing threads
    std::barrier sync_point(NUM_THREADS);

    // Atomic counters for tracking progress
    std::atomic<int> success_count(0);
    std::atomic<int> fail_count(0);

    auto start_time = std::chrono::high_resolution_clock::now();

    // Random number generator
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 10000);

    // Multithreaded operations: insert, get, and pop
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(std::thread([&map, &distribution, &generator, &success_count, &fail_count, &sync_point] {
            // Wait for all threads to reach this point
            sync_point.arrive_and_wait();

            for (int j = 0; j < NUM_OPERATIONS; ++j) {
                int key = distribution(generator);
                map.emplace(key, "Value " + std::to_string(key));
                if(!map.pop(key)){
                    std::cout << "false:" << key << "\n";
                }
            }
        }));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Output the result
    std::cout << "Test completed in " << duration.count() << "ms\n";
    std::cout << "Successful get operations: " << success_count.load() << "\n";
    std::cout << "Failed get operations: " << fail_count.load() << "\n";
    std::cout << "Map size: " << map.size() << "\n";

    return 0;
}