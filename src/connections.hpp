#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <filesystem>
#include <vector>
#include <fstream>
#include <string.h>
#include "sockets.hpp"
#define PORT 8080

std::vector<char> readAllBytes(std::string path){
    std::ifstream inFile(path, std::ios::binary);
    if (!inFile.is_open()){
        std::cerr << "[ERROR] Fail to open the file" << std::endl;
        return std::vector<char>();
    }
    // Get the final position in the file
    inFile.seekg(0, std::ios::end);
    std::ifstream::pos_type pos = inFile.tellg();
    std::vector<char> bytes(pos);
    inFile.seekg(0, std::ios::beg);
    inFile.read(&bytes[0], pos); 
    inFile.close();
    return bytes;
}

class Connection{
protected:

    void sendFiles(std::string path, Socket& socket){
        auto bytes = readAllBytes(path);
        auto sizeBytes = bytes.size();
        std::string st_sizeBytes = std::to_string(sizeBytes);
        socket.sendBuffer(st_sizeBytes.c_str(), st_sizeBytes.size());
        auto pos = 0;
        auto chunckSize = 1024;
        auto sendChunck = 0;
        while(sizeBytes > 0){
            sendChunck = (sizeBytes >= chunckSize)? chunckSize : sizeBytes ; 
            socket.sendBuffer(&bytes[pos++ * chunckSize], sendChunck);
            sizeBytes -= sendChunck;
        }
    }

    void recvFiles(std::string path, Socket& socket){
        std::ofstream outFile(path, std::ios::binary);
        if (!outFile.is_open()){
            std::cerr << "[ERROR] Fail to open the file" << std::endl;
            return;
        }
        auto chunckSize = 1024;
        /*
            use just 4 bytes to get the exact size of a unsigned long int
        */
        char size[4];
        socket.readBuffer(size, 4);
        auto fileSize = std::stoul(size);
        auto writeSize = fileSize;
        std::vector<char> bytes(fileSize);
        auto pos = 0;
        auto readBytes = 0;
        while(fileSize > 0){
            readBytes = (fileSize >= chunckSize) ? chunckSize : fileSize;
            auto recvBytes = socket.readBuffer(&bytes[pos++ * chunckSize],  readBytes);
            if (recvBytes <= 0) break;
            fileSize -= readBytes;
        } 
        writeSize = (bytes[writeSize - 1] == '\0') ? writeSize - 1 : writeSize;
        outFile.write(&bytes[0], writeSize);
        outFile.flush();
        outFile.close();
    }

    ssize_t sendMsg(std::string msg, Socket& socket){
        return socket.sendBuffer(&msg[0], msg.size());
    }

    ssize_t recvMsg(std::string &buffer, Socket& socket){
        return socket.readBuffer(&buffer[0], buffer.size());
    }
};

class Client: public Connection{
public:
    Client(){}

    void createClient(std::string ip){
        client.setServerAddress(ip.c_str(), PORT);
        client.connectToServer();
    }

    void sendFilesToServer(std::string path){
        sendFiles(path, client);
    }

    void recvFilesFromServer(std::string path){
        recvFiles(path, client);
    }

    ssize_t sendMsgToServer(std::string msg){
        return sendMsg(msg, client);
    }

    ssize_t recvMsgFromServer(std::string &buffer){
        return recvMsg(buffer, client);
    }

private:
    ClientSocket client;
};

class Server: public Connection{
public:
    Server(int *opt): server(opt){
        server.setAddress(PORT);
        server.bindSocket();
    }

    int connectClient(){
        return server.connectToClient();
    }
    
    void sendFilesToClient(std::string path){
        sendFiles(path, server);
    }

    void recvFilesFromClient(std::string path){
        recvFiles(path, server);
    }

    ssize_t sendMsgToClient(std::string msg){
        return sendMsg(msg, server);
    }

    ssize_t recvMsgFromClient(std::string &buffer){
        return recvMsg(buffer, server);
    }
private:
    ServerSocket server;
};

#endif