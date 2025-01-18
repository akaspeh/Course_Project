#include <iostream>

#include "Server.h"

#include <iostream>
#include <atomic>
#include <cassert>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>



//
//int main() {
//    test_thread_pool_multiple_sort_tasks();
//    return 0;
//}

int main() {
    Server server(18080, "src/storage/", 16);
//    server.add_files_from_directory("Files/");
//    server.clear_files_from_directory("storage/");
    if (server.initialize()) {
        std::cout << "Server initialized." << std::endl;
        server.accept_connections();
    } else {
        std::cerr << "Server initialization failed.\n";
    }
    return 0;
}