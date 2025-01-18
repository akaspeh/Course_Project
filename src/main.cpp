#include <iostream>
#include <random>

#ifdef _WIN32
#include "Server_win.h"
#else
#include "Server_linux.h"
#endif



int main() {
#ifdef _WIN32
    Server_win server(8080, "src/storage/", 8);
#else
    Server_linux server(8080, "src/storage/", 8);
#endif
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

