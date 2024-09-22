#include <iostream>

#include "../rpc/Interface.h"
#include "../common/utils/logging.h"

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

    INFO("Connecting to %s:%s", host, port);

    RPC_STATUS status;
    RPC_BINDING_HANDLE hBinding;
    unsigned char *szStringBinding = nullptr;

    INFO("Creating string binding");
    status = RpcStringBindingCompose(
            nullptr,
            const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>("ncacn_ip_tcp")),
            host,
            port,
            nullptr,
            &szStringBinding
    );

    if (status) {
        PRINT_ERROR("RpcStringBindingCompose", status);
        exit(status);
    }

    OKAY("Created string binding");

    INFO("Creating binding from string");
    status = RpcBindingFromStringBinding(
            szStringBinding,
            &hBinding
    );


    if (status) {
        PRINT_ERROR("RpcBindingFromStringBinding", status);
        exit(status);
    }

    OKAY("Created binding from string");

    INFO("Try login");
    RpcTryExcept{
                        Login(hBinding,
                              (unsigned char *) "testuser",
                              (unsigned char *) "testpassword");
                }
    RpcExcept(1)
        {
            PRINT_ERROR("Login", RpcExceptionCode());
        }
    RpcEndExcept

    status = RpcStringFree(
            &szStringBinding);

    if (status) {
        PRINT_ERROR("RpcStringFree", status);
        exit(status);
    }

    status = RpcBindingFree(
            &hBinding);

    if (status) {
        PRINT_ERROR("RpcBindingFree", status);
        exit(status);
    }

    return 0;
}
