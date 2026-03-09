#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <filesystem>
#include <vector>
#include <fstream>
#include <string.h>
#include "utils.hpp"
#include "sockets.hpp"
#define MAX_CHUNK_SIZE 1024

struct HeaderFile{
    std::string name;
    uint32_t bytesLen;
    uint32_t checkSum;

    HeaderFile();
};

namespace connection {
    void sendFile(std::string path, Socket& socket);
    void recvFile(Socket& socket);
    void sendFiles(std::vector<std::string> paths, Socket& socket);
    void recvFiles(Socket& socket);
    ssize_t sendMsg(std::string msg, Socket& socket);
    ssize_t recvMsg(std::string &buffer, Socket& socket);
    HeaderFile headerFileParser(char* header);
    int headerAmountParser(char* header);
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
    int connectClient();
    void sendFileToClient(std::string path);
    void sendAllFilesToClient(std::string dir);
    void recvFileFromClient();
    void recvAllFilesFromClient();
    ssize_t sendMsgToClient(std::string msg);
    ssize_t recvMsgFromClient(std::string &buffer);
private:
    ServerSocket& server;
};

#endif