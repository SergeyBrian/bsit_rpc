#include "cli.hpp"

Cli::Cli(FileManager *fileManager) {
    m_fileManager = fileManager;
}

void Cli::run() {
    CMD cmd;
    do {
        if (!m_fileManager->isLoggedIn()) login();
        std::string command;
        std::cout << m_fileManager->getCurrentUser() << '@' << m_fileManager->getCurrentHost() << " > ";
        std::getline(std::cin >> std::ws, command);

        int argc;
        char **argv = nullptr;
        cmd = parse_command(command, &argc, &argv);
        ERR err = exec(cmd, argc, argv);
        if (err) {
            WARN("%s", errorText[err]);
        }
    } while (cmd != CMD_Exit);
    logout();
    std::cout << "Bye :)\n";
}

void Cli::login() {
    std::string username;
    std::string password;

    do {
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;
    } while (m_fileManager->Login(username.c_str(), password.c_str()));
}

std::vector<std::string> split(const std::string &s) {
    std::vector<std::string> args;
    std::string current;
    bool in_quotes = false;
    for (char c: s) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ' ' && !in_quotes) {
            if (!current.empty()) {
                args.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        args.push_back(current);
    }
    return args;
}

CMD Cli::parse_command(std::string command, int *argc, char ***argv) {
    if (*argv != nullptr) {
        delete *argv;
    }

    std::vector<std::string> args = split(command);

    CMD cmd = CMD_Count_;
    for (int i = 0; i < CMD_Count_; i++) {
        if (args[0] == commandText[i]) {
            cmd = static_cast<CMD>(i);
            break;
        }
    }

    if (cmd == CMD_Count_) {
        WARN("Unknown command: %s", args[0].c_str());
        return cmd;
    }

    *argc = static_cast<int>(args.size());

    size_t argv_size = (args.size() + 1) * sizeof(char *);
    size_t strings_size = 0;
    for (const auto &arg: args) {
        strings_size += arg.size() + 1;
    }

    size_t total_size = argv_size + strings_size;

    char *block = new char[total_size];

    *argv = reinterpret_cast<char **>(block);

    char *strings = block + argv_size;

    for (int i = 0; i < *argc; ++i) {
        (*argv)[i] = strings;
        std::memcpy(strings, args[i].c_str(), args[i].size() + 1);
        strings += args[i].size() + 1;
    }
    (*argv)[*argc] = nullptr;

    return cmd;
}

ERR Cli::exec(CMD cmd, int argc, char **argv) {
    ERR err = ERR_Ok;
    switch (cmd) {
        case CMD_Logout:
            logout();
            break;
        case CMD_Download:
            if (argc < 3) {
                err = ERR_Invalid_args;
                break;
            }
            err = download(argv[1], argv[2]);
            break;
        default:
            break;
    }

    return err;
}

ERR Cli::download(const char *src, const char *dest) {
    return m_fileManager->Download(src, dest);
}

void Cli::logout() {
    return m_fileManager->Logout();
}
