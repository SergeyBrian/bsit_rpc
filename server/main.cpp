#include <iostream>
#include <fstream>

#include "../rpc/Interface.h"
#include "../common/utils/logging.h"

RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE, void *) {
    return RPC_S_OK;
}

int Login(handle_t h, unsigned char *Username, unsigned char *Password) {
    std::cout << "User " << Username << " logged in with password " << Password << std::endl;
    return 0;
}

void show_help() {
    std::cout << "Usage: server.exe <port>\n";
}

int main(int argc, char **argv) {

    if (argc < 2) {
        WARN("Invalid arguments");
        show_help();
        exit(1);
    }

    unsigned char *port = reinterpret_cast<unsigned char *>(argv[1]);

    INFO("Starting server on port %s", port);

    RPC_STATUS status;
    INFO("Creating server");
    status = RpcServerUseProtseqEp(
            const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>("ncacn_ip_tcp")),
            RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
            port,
            nullptr
    );

    if (status) {
        PRINT_ERROR("RpcServerUseProtseqEp", status);
        exit(status);
    }

    OKAY("Created server");

    INFO("Registering interface");
    status = RpcServerRegisterIf2(FileManagerRPC_v1_0_s_ifspec,
                                  nullptr,
                                  nullptr,
                                  RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
                                  RPC_C_LISTEN_MAX_CALLS_DEFAULT,
                                  -1,
                                  SecurityCallback);

    if (status) {
        PRINT_ERROR("RpcServerRegisterIf2", status);
        exit(status);
    }

    OKAY("Registered interface");

    INFO("Start listening");
    status = RpcServerListen(
            1,
            RPC_C_LISTEN_MAX_CALLS_DEFAULT,
            false
    );
    if (status) {
        PRINT_ERROR("RpcServerListen", status);
        exit(status);
    }

    return 0;
}
