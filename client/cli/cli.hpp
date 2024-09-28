#ifndef BSIT_RPC_CLI_HPP
#define BSIT_RPC_CLI_HPP

#include <vector>

#include <string>

#include "../file_manager/file_manager.hpp"

enum CMD {
    CMD_Exit,
    CMD_Logout,
    CMD_Download,
    CMD_Upload,
    CMD_Delete,
    CMD_Count_
};

inline const char *commandText[CMD_Count_] = {
        "exit",
        "logout",
        "download",
        "upload",
        "delete",
};

class Cli {
public:
    Cli(FileManager *fileManager);

    void run();

private:
    FileManager *m_fileManager;

    void login();

    void logout();

    ERR download(const char *src, const char *dest);

    ERR upload(const char *src, const char *dest);

    ERR delete_file(const char *filename);

    CMD parse_command(std::string command, int *argc, char ***argv);

    ERR exec(CMD cmd, int argc, char **argv);

    static BOOL WINAPI graceful_shutdown(DWORD dwCtrlType);
};

#endif //BSIT_RPC_CLI_HPP
