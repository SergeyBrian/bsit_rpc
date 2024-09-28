#ifndef BSIT_RPC_ERRORS_HPP
#define BSIT_RPC_ERRORS_HPP

enum ERR {
    ERR_Ok,
    ERR_Invalid_args,
    ERR_Invalid_creds,
    ERR_Unknown,
    ERR_File_open,
    ERR_Unauthorized,
    ERR_Permission_denied,
    ERR_Logout_required,
    ERR_Invalid_filename,
    ERR_Server_busy,
    ERR_Count_
};

inline const char *errorText[ERR_Count_] = {
        "No error",
        "Invalid arguments",
        "Invalid credentials",
        "Unknown error",
        "Failed to open file",
        "User not authorized",
        "Permission denied",
        "Logout required after permission change",
        "File not found",
        "Server is busy"
};

inline ERR winCodeToErr(int code) {
    switch (code) {
        case 0:
            return ERR_Ok;
        case 1326:
            return ERR_Invalid_creds;
        case 13:
        case 5:
        case 22:
            return ERR_Permission_denied;
        case 2:
            return ERR_Invalid_filename;
        default:
            return ERR_Unknown;
    }
}

#endif //BSIT_RPC_ERRORS_HPP
