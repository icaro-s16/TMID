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
    std::vector<char> bytes((size_t)pos);
    inFile.seekg(0, std::ios::beg);
    inFile.read(&bytes[0], pos); 
    inFile.close();
    return bytes;
}

class Connection{
protected:

    void sendFiles(std::string path, Socket& socket){
        std::vector<char> bytes = readAllBytes(path);
        size_t sizeBytes = bytes.size();
        std::string st_sizeBytes = std::to_string(sizeBytes);
        std::string digits_size_of_sizeBytes = std::to_string(st_sizeBytes.size());
        std::string header = digits_size_of_sizeBytes + st_sizeBytes;
        socket.sendBuffer((char*)&header[0], header.size());
        size_t pos = 0;
        size_t chunckSize = 1024;
        size_t sendChunck = 0;
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
        char digitSize[1]; 
        socket.readBuffer(&digitSize[0], 1);
        std::vector<char> fileSize(std::stoul(digitSize));
        socket.readBuffer((char*)&fileSize[0], fileSize.size());
        size_t chunckSize = 1024;
        std::string s(fileSize.begin(), fileSize.end());
        size_t size = std::stoul(s);
        size_t writeSize = size;
        size_t pos = 0;
        size_t readBytes = 0;
        std::vector<char> bytes(size);
        while(size > 0){
            readBytes = (size >= chunckSize) ? chunckSize : size;
            ssize_t recvBytes = socket.readBuffer(&bytes[pos++ * chunckSize],  readBytes);
            if (recvBytes <= 0) break;
            size -= readBytes;
        } 
        writeSize = (bytes[writeSize - 1] == '\0') ? writeSize - 1 : writeSize;
        outFile.write(&bytes[0], (std::streamsize)writeSize);
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