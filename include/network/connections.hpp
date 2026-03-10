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
    void sendFile(std::string path, Socket& socket);
    void recvFile(Socket& socket);
    void sendFiles(std::vector<std::string> paths, Socket& socket);
    void recvFiles(Socket& socket);
    ssize_t sendMsg(std::string msg, Socket& socket);
    ssize_t recvMsg(std::string &buffer, Socket& socket);
};

class Client {
public:
    Client(ClientSocket &_client);
    bool createClient(std::string ip);
    void sendFile(std::string path);
    void sendFiles(std::vector<std::string> paths);
    void recvFile();
    void recvFiles();
    ssize_t sendMsg(std::string msg);
    ssize_t recvMsg(std::string &buffer);
private:
    ClientSocket& client;
};

class Server {
public:
    Server(ServerSocket& _server);
    ClientSocket connectClient();
private:
    ServerSocket& server;
};

#endif