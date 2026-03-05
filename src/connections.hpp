#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <filesystem>
#include <vector>
#include <fstream>
#include <string.h>
#include "utils.hpp"
#include "sockets.hpp"
#define MAX_CHUNK_SIZE 1024

char* readAllBytes(std::string path){
    std::ifstream inFile(path, std::ios::binary | std::ios::ate);
    if (!inFile.is_open()){
        std::cerr << "[ERROR] Fail to open the file" << std::endl;
        return nullptr;
    }
    std::ifstream::pos_type fileLen = inFile.tellg();
    char* bytes = new char[(size_t)fileLen];
    // Back to beginning of the file
    inFile.seekg(0, std::ios::beg);
    inFile.read(bytes, fileLen); 
    inFile.close();
    return bytes;
}

std::vector<std::string> getAllFilePath(std::filesystem::path path){
    std::vector<std::string> filesPath;
    for(auto entry: std::filesystem::directory_iterator(path)){
        if (entry.is_regular_file())
            filesPath.push_back(entry.path().string());
    }
    return filesPath;
}

class Connection{
protected:
    void sendAllFiles(std::filesystem::path path, Socket& socket){
        std::vector<std::string> filesPath = getAllFilePath(path);
        std::string lenOfFilesPath = std::to_string(filesPath.size());
        std::string sizeOflen = std::to_string(lenOfFilesPath.size());

        std::string header = sizeOflen + lenOfFilesPath;
        socket.sendBuffer(header.c_str(), header.size());

        for(std::string fileName: filesPath)
            sendFile(fileName, socket);

    }

    void recvAllFiles(Socket& socket){
        
        char _len;
        socket.readBuffer(&_len, 1);
        
        unsigned short len = _len - '0';
        char* _file_counter = new char[len];

        socket.readBuffer(_file_counter, len);

        size_t file_counter = std::stoul(_file_counter);
        delete[] _file_counter;

        for(auto i = 0; i < file_counter; i++)
            recvFile(socket);
        
    }

    void sendFile(std::string path, Socket& socket){
        std::vector<std::string> file_tokens = splitText(path, '/');
        std::string fileName = file_tokens[file_tokens.size() - 1];
        /*
        Header: <name lenght size> <name lenght> <name> <bytes lenght size> <bytes len>
        */

        std::string nameLen = std::to_string(fileName.size());
                                   
        // Add name lenght size
        std::string header = std::to_string(nameLen.size());

        header += nameLen;
        header += fileName;

        char* data = readAllBytes(path);
        if (!data) return;
        
        const size_t bytesLen = strlen(data);
        std::string st_bytesLen = std::to_string(bytesLen); 

        header += std::to_string(st_bytesLen.size());
        header += st_bytesLen;

        socket.sendBuffer(header.c_str(), header.size());
        header.clear();

        size_t currentffSet = 0;
        size_t bytesRemaining = bytesLen;

        while(bytesRemaining > 0) {
            size_t chunkSize = (bytesRemaining >= MAX_CHUNK_SIZE)? MAX_CHUNK_SIZE : bytesRemaining; 
            size_t sendBytes = socket.sendBuffer(&data[currentffSet], chunkSize);
            currentffSet += sendBytes;
            bytesRemaining -= sendBytes;
        }
        memset(data, 0, bytesLen);
        delete[] data;
    }

    void recvFile(Socket& socket){
        
        
        char size;
        socket.readBuffer(&size, 1);
        std::size_t fileNameLenSize = size - '0';
        
        char* _fileNameLen = new char[fileNameLenSize];

        socket.readBuffer(_fileNameLen, fileNameLenSize);
        std::size_t fileNameLen = std::stoul(_fileNameLen);
        memset(_fileNameLen, 0, fileNameLenSize);
        delete[] _fileNameLen;
        
        char* fileName = new char[fileNameLen];
        socket.readBuffer(fileName, fileNameLen);
        

        std::ofstream outFile(fileName, std::ios::binary);
        if (!outFile.is_open()){
            std::cerr << "[ERROR] Fail to open the file" << std::endl;
            return;
        }

        memset(fileName, 0, fileNameLen);
        delete[] fileName;

        socket.readBuffer(&size, 1);

        std::size_t bytesLenSize = size - '0';
        
        char* _bytesLen = new char[bytesLenSize];
        
        socket.readBuffer(_bytesLen, bytesLenSize);
        std::size_t bytesLen = std::stoul(_bytesLen);
        memset(_bytesLen, 0, bytesLenSize);
        delete[] _bytesLen;
        
        char* data = new char[bytesLen];
        std::size_t bytesRemaining = bytesLen;
        std::size_t currentOffset = 0;

        while(bytesRemaining > 0){
            ssize_t requestSize = (bytesRemaining >= MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : bytesRemaining;
            ssize_t recvBytes = socket.readBuffer(&data[currentOffset],  requestSize);
            
            if (recvBytes <= 0) break;

            currentOffset += recvBytes;
            bytesRemaining -= recvBytes;
        }

        outFile.write(data, (std::streamsize)bytesLen);
        outFile.close();
        memset(data, 0, bytesLen);
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
    Client(ClientSocket &_client):client(_client){}

    void createClient(std::string ip){
        client.setServerAddress(ip.c_str());
        client.connectToServer();
    }

    void sendFilesToServer(std::string path){
        sendFile(path, client);
    }

    void sendAllFilesToServer(std::filesystem::path path){
        sendAllFiles(path, client);
    }

    void recvFilesFromServer(std::string path){
        recvFile(client);
    }

    void recvAllFilesFromServer(std::string pattern){
        recvAllFiles(client);
    }

    ssize_t sendMsgToServer(std::string msg){
        return sendMsg(msg, client);
    }

    ssize_t recvMsgFromServer(std::string &buffer){
        return recvMsg(buffer, client);
    }

private:
    ClientSocket& client;
};

class Server: public Connection{
public:
    Server(ServerSocket& _server):server(_server){
        server.bindSocket();
    }

    int connectClient(){
        return server.connectToClient();
    }
    
    void sendFilesToClient(std::string path){
        
        sendFile(path, server);
    }

    void sendAllFilesToClient(std::string dir){
        sendAllFiles(dir, server);
    }

    void recvFilesFromClient(std::string path){
        recvFile(server);
    }

    void recvAllFilesFromClient(std::string pattern){
        recvAllFiles(server);
    }

    ssize_t sendMsgToClient(std::string msg){
        return sendMsg(msg, server);
    }

    ssize_t recvMsgFromClient(std::string &buffer){
        return recvMsg(buffer, server);
    }
private:
    ServerSocket& server;
};

#endif