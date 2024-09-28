#include "file_manager.hpp"

ERR FileManager::SetUser(const User &user) {
    m_activeUser = user;

    return ERR_Ok;
}

FileManager &FileManager::Instance() {
    return *g_fileManager;
}

int FileManager::Download(const unsigned char *filename, unsigned char *buffer, ERR *err, int size) {
    if (!m_activeFile.is_open() || m_activeFileName.empty() ||
        std::strcmp(m_activeFileName.c_str(), reinterpret_cast<const char *>(filename)) != 0) {
        LOG("User %s requested download for file %s",
            FileManager::Instance().GetActiveUser().username.c_str(),
            filename
        );
        INFO("Opening file %", filename);
        if (m_activeFile.is_open()) m_activeFile.close();
        m_activeFile = std::ifstream(reinterpret_cast<const char *>(filename), std::ios::binary);
        m_activeFileName = reinterpret_cast<const char *const>(filename);
    }

    if (!m_activeFile.is_open()) {
        WARN("Error opening file %s: %s", filename, strerror(errno));
        INFO("Error: %d", errno);
        *err = winCodeToErr(errno);
        return 0;
    }

    m_activeFile.read(reinterpret_cast<char *>(buffer), size);
    int read_size = m_activeFile.gcount();
    m_activeUser.downloadProgress += read_size;
    OKAY("Sent %d bytes", read_size);
    if (read_size == 0) m_activeFile.close();

    return read_size;
}

User &FileManager::GetActiveUser() {
    return m_activeUser;
}

void FileManager::Logout() {
    m_activeUser = User{};
}
