#ifndef BSIT_RPC_FILE_MANAGER_HPP
#define BSIT_RPC_FILE_MANAGER_HPP

#include <fstream>

#include "../../common/utils/logging.h"
#include "../../rpc/Interface.h"
#include "../../common/errors.hpp"

class FileManager {
public:
    FileManager(const unsigned char *host, const unsigned char *port);

    ~FileManager();

    ERR Login(const char *username, const char *password);

    void Logout();

    ERR Download(const char *src, const char *dest);

    std::string getCurrentUser();

    std::string getCurrentHost();

    bool isLoggedIn();

    ERR Upload(const char *src, const char *dest);

    ERR Delete(const char *filename);

    static FileManager &Instance();

private:
    RPC_BINDING_HANDLE m_hBinding;
    unsigned char *m_szStringBinding;
    std::string m_username;
    std::string m_host;
    bool m_isLoggedIn = false;
};

inline FileManager *g_fileManager = nullptr;


#endif //BSIT_RPC_FILE_MANAGER_HPP
