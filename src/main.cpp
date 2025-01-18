#include <iostream>
#include <random>

#ifdef _WIN32
#include "Server_win.h"
#else
#include "Server_linux.h"
#endif

int main(int argc, char* argv[]) {
    size_t thread_amount = 8;
    size_t port = 8080;
    if (argc == 1){size_t port =std::stoi(argv[1]);}
    else if (argc == 2){size_t thread_amount = std::stoi(argv[2]);}
    else if (argc > 2){std::cerr << "init default param." << std::endl;}
#ifdef _WIN32
    Server_win server(port, "src/storage/", thread_amount);
#else
    Server_linux server(port, "src/storage/", thread_amount);
#endif
    if (server.initialize()) {
        std::cout << "Server initialized." << std::endl;
        server.accept_connections();
    } else {
        std::cerr << "Server initialization failed.\n";
    }
    return 0;
}

