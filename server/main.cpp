#include <iostream>
#include <fstream>

#include "../rpc/Interface.h"
#include "../common/utils/logging.h"
#include "file_manager/file_manager.hpp"
#include "../common/errors.hpp"

RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE, void *) {
    return RPC_S_OK;
}

int Login(handle_t hBinding, unsigned char *username, unsigned char *password) {
    INFO("Login requested for %s", username);
    ERR err;
    int status = LogonUser(reinterpret_cast<LPSTR>(username),
                           nullptr,
                           reinterpret_cast<LPSTR>(password),
                           LOGON32_LOGON_INTERACTIVE,
                           LOGON32_PROVIDER_DEFAULT,
                           &hBinding);
    if (status) {
        err = FileManager::Instance().SetUser({
                                                      .username = reinterpret_cast<const char *const>(username),
                                                      .isAuthorized = true,
                                                      .hBinding = hBinding,
                                              });

    } else {
        err = winCodeToErr(GetLastError());
    }

    if (err) {
        WARN("Error during login for user %s: %lu", username, GetLastError());
    } else {
        OKAY("User %s logged in", username);
    }

    return err;
}

int Download(handle_t hBinding, unsigned char *filename, unsigned char *buffer, int *error) {
    if (!ImpersonateLoggedOnUser(FileManager::Instance().GetActiveUser().hBinding)) {
        WARN("Download attempt by unauthorized user");
        *error = ERR_Unauthorized;
        return 0;
    }

    return FileManager::Instance().Download(filename,
                                            buffer,
                                            reinterpret_cast<ERR *>(error),
                                            BUF_SIZE);
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

    g_fileManager = new FileManager();

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
