#include <iostream>
#include "Server.h"

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
