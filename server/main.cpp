#include <iostream>
#include <fstream>

#include "../rpc/Interface.h"

RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE, void *) {
    return RPC_S_OK;
}

void *__RPC_USER midl_user_allocate(size_t size) {
    return ::operator new(size);
}

void __RPC_USER midl_user_free(void *p) {
    ::operator delete(p);
}

int Login(handle_t h, unsigned char *Username, unsigned char *Password) {
    std::cout << "User " << Username << " logged in with password " << Password << std::endl;
    return 0;
}


int main() {
    RPC_STATUS status;
    status = RpcServerUseProtseqEp(
            const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>("ncacn_ip_tcp")),
            RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
            const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>("6969")),
            nullptr
    );

    if (status)
        exit(status);

    status = RpcServerRegisterIf2(FileManagerRPC_v1_0_s_ifspec,
                                  nullptr,
                                  nullptr,
                                  RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
                                  RPC_C_LISTEN_MAX_CALLS_DEFAULT,
                                  -1,
                                  SecurityCallback);

    if (status)
        exit(status);

    return RpcServerListen(
            1,
            RPC_C_LISTEN_MAX_CALLS_DEFAULT,
            false
    );
}
