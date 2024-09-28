#ifndef BSIT_RPC_FILE_MANAGER_HPP
#define BSIT_RPC_FILE_MANAGER_HPP

#include <map>
#include <string>
#include <fstream>
#include <windows.h>

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

    ERR Upload(const unsigned char *filename, const unsigned char *buffer, int size, int final);

    User &GetActiveUser();

    void Logout();

    ERR Delete(unsigned char *filename);

private:
    User m_activeUser;
    std::ifstream m_activeFile;
    std::string m_activeFileName;
    std::ofstream m_activeUploadFile;
    std::string m_activeUploadFileName;
};

inline FileManager *g_fileManager;


#endif //BSIT_RPC_FILE_MANAGER_HPP
