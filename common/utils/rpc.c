#include "../../rpc/Interface.h"

void *__RPC_USER midl_user_allocate(size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(void *p) {
    free(p);
}

