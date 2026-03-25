#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <filesystem>
#include <vector>
#include <fstream>
#include <string.h>
#include <map>

#include "network/sockets.hpp"
#include "utils/utils.hpp"
#include "models/file.hpp"
#include "models/data_header.hpp"

#define MAX_CHUNK_SIZE 1024

/* Protocol opcodes (4-byte ASCII) */
namespace Op {
    inline constexpr const char* HELLO    = "HELO"; // socket to socket handshake
    inline constexpr const char* SUBMIT   = "SUBM"; // client submits completed tasks
    inline constexpr const char* SYNC     = "SYNC"; // client sync available tasks
    inline constexpr const char* BYE      = "BYE0"; // Disconnect
}

namespace connection {
    void sendFile(const std::filesystem::path& path, Socket& socket);
    void sendFiles(std::vector<std::filesystem::path> paths, Socket& socket);
    void recvFile(std::pair<std::string, std::vector<char>> &file, Socket& socket);
    void recvFiles(std::map<std::string, std::vector<char>> &files, Socket& socket);
    ssize_t sendMsg(std::string msg, Socket& socket);
    ssize_t recvMsg(std::string &buffer, Socket& socket);
};

#endif