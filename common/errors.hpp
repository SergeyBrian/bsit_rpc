#ifndef BSIT_RPC_ERRORS_HPP
#define BSIT_RPC_ERRORS_HPP

enum ERR {
    ERR_Ok,
    ERR_Invalid_args,
    ERR_Invalid_creds,
    ERR_Unknown,
    ERR_File_open,
    ERR_Unauthorized,
    ERR_Count_
};

inline const char *errorText[ERR_Count_] = {
        "No error",
        "Invalid arguments",
        "Invalid credentials",
        "Unknown error",
        "Failed to open file",
        "User not authorized",
};

inline ERR winCodeToErr(int code) {
    switch (code) {
        case 0:
            return ERR_Ok;
        case 1326:
            return ERR_Invalid_creds;
        default:
            return ERR_Unknown;
    }
}

#endif //BSIT_RPC_ERRORS_HPP
