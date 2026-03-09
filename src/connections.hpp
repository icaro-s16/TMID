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

class Connection{
protected:
    void sendFiles(std::vector<std::string> paths, Socket& socket);
    void sendAllFiles(std::string dir_path, Socket& socket);
    void recvAllFiles(Socket& socket);
    void sendFile(std::string path, Socket& socket);
    void recvFile(Socket& socket);
    ssize_t sendMsg(std::string msg, Socket& socket);
    ssize_t recvMsg(std::string &buffer, Socket& socket);

private:
    HeaderFile headerFileParser(char* header);
    int headerAmountParser(char* header);
};

// sendFiles
// sendMessages

class Client: public Connection{
public:
    Client(ClientSocket &_client);
    bool createClient(std::string ip);
    void sendFileToServer(std::string path);
    void sendAllFilesToServer(std::string dir);
    void recvFileFromServer();
    void recvAllFilesFromServer();
    ssize_t sendMsgToServer(std::string msg);
    ssize_t recvMsgFromServer(std::string &buffer);
private:
    ClientSocket& client;
};

class Server: public Connection{
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