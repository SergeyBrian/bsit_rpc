#ifndef BSIT_RPC_FILE_MANAGER_HPP
#define BSIT_RPC_FILE_MANAGER_HPP

#include <map>
#include <string>
#include <fstream>

#include "../../common/errors.hpp"
#include "../../common/utils/logging.h"

struct User {
    std::string username;
    bool isAuthorized;
    void *hBinding;
    int downloadProgress;
};

class FileManager {
public:
    ERR SetUser(const User &user);

    static FileManager &Instance();

    int Download(const unsigned char *filename, unsigned char *buffer, ERR *err, int size);

    User &GetActiveUser();

    void Logout();

private:
    User m_activeUser;
    std::ifstream m_activeFile;
    std::string m_activeFileName;
};

inline FileManager *g_fileManager;


#endif //BSIT_RPC_FILE_MANAGER_HPP
