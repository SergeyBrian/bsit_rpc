#include <iostream>

#include "cli/cli.hpp"

void show_help() {
    std::cout << "Usage: client.exe <host> <port>\n";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        WARN("Invalid arguments");
        show_help();
        exit(1);
    }

    unsigned char *host = reinterpret_cast<unsigned char *>(argv[1]);
    unsigned char *port = reinterpret_cast<unsigned char *>(argv[2]);

    FileManager fileManager(host, port);

    Cli cli(&fileManager);

    cli.run();

    return 0;
}
