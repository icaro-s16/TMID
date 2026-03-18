#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <filesystem>
#include <vector>
#include <fstream>
#include <string.h>

#include "network/sockets.hpp"
#include "utils/utils.hpp"
#include "models/file.hpp"
#include "models/data_header.hpp"

#define MAX_CHUNK_SIZE 1024

namespace connection {
    void sendFile(const std::filesystem::path& path, Socket& socket);
    void recvFile(Socket& socket);
    void sendFiles(std::vector<std::filesystem::path> paths, Socket& socket);
    void recvFiles(Socket& socket);
    ssize_t sendMsg(std::string msg, Socket& socket);
    ssize_t recvMsg(std::string &buffer, Socket& socket);
};

#endif