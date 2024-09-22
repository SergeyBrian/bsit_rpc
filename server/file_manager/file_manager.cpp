#include "file_manager.hpp"

ERR FileManager::SetUser(const User &user) {
    m_activeUser = user;

    return ERR_Ok;
}

FileManager &FileManager::Instance() {
    return *g_fileManager;
}

int FileManager::Download(const unsigned char *filename, unsigned char *buffer, ERR *err, int size) {
    int offset = m_activeUser.downloadProgress;

    if (!m_activeFile.is_open() || m_activeFileName.empty() ||
        std::strcmp(m_activeFileName.c_str(), reinterpret_cast<const char *>(filename)) != 0) {
        INFO("Opening file %", filename);
        if (m_activeFile.is_open()) m_activeFile.close();
        m_activeFile = std::ifstream(reinterpret_cast<const char *>(filename), std::ios::binary);
        if (!m_activeFile.is_open()) WARN("Error opening file: %s", strerror(errno));
        m_activeFileName = reinterpret_cast<const char *const>(filename);
    }

    if (!m_activeFile.is_open()) {
        WARN("Error opening file %s", filename);
        *err = ERR_File_open;
        return 0;
    }

//    m_activeFile.seekg(offset);
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
