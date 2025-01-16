#include <iostream>

#include "Server.h"

int main() {
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