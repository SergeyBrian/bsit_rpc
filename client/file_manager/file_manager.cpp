#include "file_manager.hpp"

FileManager::FileManager(const unsigned char *host, const unsigned char *port) {
    INFO("Connecting to %s:%s", host, port);

    RPC_STATUS status;

    INFO("Creating string binding");
    status = RpcStringBindingCompose(
            nullptr,
            const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>("ncacn_ip_tcp")),
            const_cast<unsigned char *>(host),
            const_cast<unsigned char *>(port),
            nullptr,
            &m_szStringBinding
    );

    if (status) {
        PRINT_ERROR("RpcStringBindingCompose", status);
        exit(status);
    }

    OKAY("Created string binding");

    INFO("Creating binding from string");
    status = RpcBindingFromStringBinding(
            m_szStringBinding,
            &m_hBinding
    );


    if (status) {
        PRINT_ERROR("RpcBindingFromStringBinding", status);
        exit(status);
    }

    OKAY("Created binding from string");

    m_host = std::string(reinterpret_cast<const char *const>(host));
    g_fileManager = this;
}

FileManager::~FileManager() {
    int status = RpcStringFree(
            &m_szStringBinding);

    if (status) {
        PRINT_ERROR("RpcStringFree", status);
        exit(status);
    }

    status = RpcBindingFree(
            &m_hBinding);

    if (status) {
        PRINT_ERROR("RpcBindingFree", status);
        exit(status);
    }
}

ERR FileManager::Login(const char *username, const char *password) {
    ERR err;
    RpcTryExcept{
                        err = static_cast<ERR>(::Login(m_hBinding,
                                                       const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(username)),
                                                       const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(password))));
                }
    RpcExcept(1)
        {
            PRINT_ERROR("Login", RpcExceptionCode());
            return ERR_Unknown;
        }
    RpcEndExcept

    if (err != ERR_Ok) {
        WARN("%s", errorText[err]);
        return err;
    }

    m_username = username;
    m_isLoggedIn = true;
    return ERR_Ok;
}

std::string FileManager::getCurrentUser() {
    return m_username;
}

std::string FileManager::getCurrentHost() {
    return m_host;
}

ERR FileManager::Download(const char *src, const char *dest) {
    unsigned char buf[BUF_SIZE];
    ERR err = ERR_Ok;
    int size;
    std::ofstream file(dest, std::ios::binary);
    if (!file.is_open()) {
        return ERR_File_open;
    }

    INFO("Downloading from %s to %s", src, dest);
    do {
        size = ::Download(m_hBinding,
                          const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(src)),
                          buf,
                          reinterpret_cast<int *>(&err));
        if (err) {
            break;
        }
        file.write(reinterpret_cast<const char *>(buf), size);
        OKAY("Downloaded %d bytes", size);
    } while (size);
    if (!err) {
        OKAY("Download finished");
    }

    file.close();

    return err;
}

bool FileManager::isLoggedIn() {
    return m_isLoggedIn;
}

void FileManager::Logout() {
    ::Logout(m_hBinding);
    m_isLoggedIn = false;
}

ERR FileManager::Upload(const char *src, const char *dest) {
    unsigned char buf[BUF_SIZE];
    ERR err = ERR_Ok;
    std::ifstream file(src, std::ios::binary);
    if (!file.is_open()) {
        return ERR_File_open;
    }
    INFO("Uploading from %s to %s", src, dest);

    while (file) {
        file.read(reinterpret_cast<char *>(buf), BUF_SIZE);
        err = static_cast<ERR>(::Upload(m_hBinding,
                                        const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(dest)),
                                        static_cast<unsigned char *>(buf),
                                        static_cast<int>(file.gcount()),
                                        !static_cast<bool>(file)));
        if (err) break;
    }

    if (!err) {
        OKAY("Upload finished");
    }

    return err;
}

ERR FileManager::Delete(const char *filename) {
    ERR err = static_cast<ERR>(::Delete(m_hBinding,
                               const_cast<unsigned char *>(reinterpret_cast<const unsigned char*>(filename))));
    if (!err) {
        OKAY("Delete finished");
    }

    return err;
}

FileManager &FileManager::Instance() {
    return *g_fileManager;
}
