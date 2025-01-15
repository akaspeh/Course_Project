#include <iostream>
#include "Server.h"
#include "data_structure/ThreadSafeHashMap.h"

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cassert>
#include <atomic>
#include <random>
#include <barrier>

int main() {
    Server server(8080);
    if (server.initialize()) {
        std::cout << "Server initialized.\n";
        server.acceptConnections();
    } else {
        std::cerr << "Server initialization failed.\n";
    }
    return 0;
}