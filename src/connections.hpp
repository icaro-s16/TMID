#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <filesystem>
#include <vector>
#include <fstream>
#include <string.h>
#include "sockets.hpp"
#define PORT 8080
#define MAX_CHUNK_SIZE 1024

char* readAllBytes(std::string path){
    std::ifstream inFile(path, std::ios::binary);
    if (!inFile.is_open()){
        std::cerr << "[ERROR] Fail to open the file" << std::endl;
        return nullptr;
    }
    // Get the final position in the file
    inFile.seekg(0, std::ios::end);
    std::ifstream::pos_type fileLen = inFile.tellg();
    char* bytes = new char[(size_t)fileLen];
    inFile.seekg(0, std::ios::beg);
    inFile.read(bytes, fileLen); 
    inFile.close();
    return bytes;
}

class Connection{
protected:

    void sendFiles(std::string path, Socket& socket){
        
        char* data = readAllBytes(path);
        if (!data) return;
        
        const size_t lenOfBytes = strlen(data);
        
        std::string _lenOfBytes = std::to_string(lenOfBytes);
        std::string _lenOfSize = std::to_string(_lenOfBytes.size());
        const char* header = (_lenOfSize + _lenOfBytes).c_str();
        
        socket.sendBuffer(header, strlen(header));
        
        size_t currentOffset = 0;
        size_t bytesRemaining = lenOfBytes;

        while(bytesRemaining > 0) {
            size_t chunkSize = (bytesRemaining >= MAX_CHUNK_SIZE)? MAX_CHUNK_SIZE : bytesRemaining; 
            socket.sendBuffer(&data[currentOffset * MAX_CHUNK_SIZE], chunkSize);
            currentOffset++;
            bytesRemaining -= chunkSize;
        }
        delete[] data;
    }

    void recvFiles(std::string path, Socket& socket){
        
        std::ofstream outFile(path, std::ios::binary);
        if (!outFile.is_open()){
            std::cerr << "[ERROR] Fail to open the file" << std::endl;
            return;
        }

        char _lenOfSize;
        socket.readBuffer(&_lenOfSize, 1);
        std::size_t lenOfSize = _lenOfSize - '0';
        
        char* _lenOfBytes = new char[lenOfSize];
        socket.readBuffer(_lenOfBytes, lenOfSize);
        std::size_t lenOfBytes = std::stoul(_lenOfBytes);
        delete[] _lenOfBytes;
        
        char* data = new char[lenOfBytes];
        std::size_t bytesRemaining = lenOfBytes;
        std::size_t currentOffset = 0;

        while(bytesRemaining > 0){
            ssize_t requestSize = (bytesRemaining >= MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : bytesRemaining;
            ssize_t recvBytes = socket.readBuffer(&data[currentOffset],  requestSize);
            
            if (recvBytes <= 0) break;

            currentOffset += recvBytes;
            bytesRemaining -= recvBytes;
        }

        outFile.write(data, (std::streamsize)lenOfBytes);
        outFile.flush();
        outFile.close();

        delete[] data;
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