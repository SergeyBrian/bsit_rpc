#include <iostream>

#include "../common/utils/logging.h"
#include "file_manager/file_manager.hpp"
#include "rpc/rpc.hpp"

void show_help() {
    std::cout << "Usage: server.exe <port>\n";
}

int main(int argc, char **argv) {

    if (argc < 2) {
        WARN("Invalid arguments");
        show_help();
        exit(1);
    }

    g_fileManager = new FileManager();

    auto *port = reinterpret_cast<unsigned char *>(argv[1]);

    InitServer(port);
    Listen();

    return 0;
}
