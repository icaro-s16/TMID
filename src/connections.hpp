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

std::vector<std::string> getAllFilePath(std::string st_dir_path){
    std::vector<std::string> filesPath;
    std::filesystem::path dir_path(st_dir_path); 
    for(auto entry: std::filesystem::directory_iterator(dir_path)){
        if (entry.is_regular_file())
            filesPath.push_back(entry.path().string());
    }
    return filesPath;
}

struct HeaderFile{
    std::string name;
    uint32_t bytesLen;
    uint32_t checkSum;

    HeaderFile(): name(""), bytesLen(0), checkSum(0){};
};

class Connection{
protected:
    void sendAllFiles(std::string dir_path, Socket& socket){
        std::vector<std::string> filesPath = getAllFilePath(dir_path);
        
        char header[MAX_CHUNK_SIZE];
        memset(header, 0, MAX_CHUNK_SIZE);

        sprintf(
            header,
            "Amount:%i\r\n\r\n",
            filesPath.size()
        );

        socket.sendBuffer(header, MAX_CHUNK_SIZE);

        for(std::string fileName: filesPath)
            sendFile(fileName, socket);

    }

    void recvAllFiles(Socket& socket){
        char buffer[MAX_CHUNK_SIZE];
        size_t recvAmountSize = 0;
        while(recvAmountSize < MAX_CHUNK_SIZE){
            recvAmountSize += socket.readBuffer(buffer, MAX_CHUNK_SIZE);
        }
        size_t file_counter = headerAmountParser(buffer);

        for(auto i = 0; i < file_counter; i++)
            recvFile(socket);
        
    }

    void sendFile(std::string path, Socket& socket){
        std::vector<std::string> file_tokens = splitText(path, '/');
        std::string fileName = file_tokens[file_tokens.size() - 1];
        
        char* data = readAllBytes(path);
        if (!data) return;

        const size_t bytesLen = strlen(data);

        HeaderFile header_values;
        
        header_values.name = fileName;
        header_values.bytesLen = bytesLen;
        
        char header[MAX_CHUNK_SIZE];
        memset(header, 0, MAX_CHUNK_SIZE);

        // Create a template the Header Size to calculate the Check Sum
        sprintf(
            header,
            "Name:%s\r\nFileSize:%i\r\n\r\n", 
            header_values.name.c_str(), 
            header_values.bytesLen
        );

        socket.sendBuffer(&header, MAX_CHUNK_SIZE);

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
        char header[MAX_CHUNK_SIZE];
        memset(header, 0, MAX_CHUNK_SIZE);
        size_t header_recv_bytes = 0;

        while(header_recv_bytes < MAX_CHUNK_SIZE){
            
            header_recv_bytes += socket.readBuffer(&header[header_recv_bytes], MAX_CHUNK_SIZE - header_recv_bytes);
        
        }

        // This funtion destroy the char*
        HeaderFile header_values = headerFileParser(header);

        std::ofstream outFile(header_values.name, std::ios::binary);
        if (!outFile.is_open()){
            std::cerr << "[ERROR] Fail to open the file" << std::endl;
            return;
        }

        char* data = new char[header_values.bytesLen];
        std::size_t bytesRemaining = header_values.bytesLen;
        std::size_t currentOffset = 0;

        while(bytesRemaining > 0){
            ssize_t requestSize = (bytesRemaining >= MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : bytesRemaining;
            ssize_t recvBytes = socket.readBuffer(&data[currentOffset],  requestSize);
            
            if (recvBytes <= 0) break;

            currentOffset += recvBytes;
            bytesRemaining -= recvBytes;
        }

        outFile.write(data, (std::streamsize) header_values.bytesLen);
        outFile.close();
        memset(data, 0, header_values.bytesLen);
        delete[] data;
        
    }

    ssize_t sendMsg(std::string msg, Socket& socket){
        return socket.sendBuffer(&msg[0], msg.size());
    }

    ssize_t recvMsg(std::string &buffer, Socket& socket){
        return socket.readBuffer(&buffer[0], buffer.size());
    }
private:
    HeaderFile headerFileParser(char* header){
        HeaderFile header_values;
        char* line = strtok(header, "\r\n");
        while (line != nullptr){
            char* delimeter = strchr(line, ':');
            if(delimeter != nullptr){
                delimeter[0] = '\0';
                char* label = line;
                char* value = delimeter + 1;
                if (strcmp(label, "Name") == 0)
                    header_values.name = value;
                else if(strcmp(label, "FileSize") == 0)
                    header_values.bytesLen = std::stoul(value);
            }
            line = strtok(NULL, "\r\n");
        }
        return header_values;
    }

    int headerAmountParser(char* header){
        char* delimeter = strchr(header, ':');
        if(delimeter != nullptr){
            delimeter[0] = '\0';
            char* label = header;
            char* value = delimeter + 1;
            if (strcmp(label, "Amount") == 0){
                return std::stoul(value);
            }
        }
        return 0;
    }
};

class Client: public Connection{
public:
    Client(ClientSocket &_client):client(_client){}

    void createClient(std::string ip){
        client.setServerAddress(ip.c_str());
        client.connectToServer();
    }

    void sendFileToServer(std::string path){
        sendFile(path, client);
    }

    void sendAllFilesToServer(std::string dir){
        sendAllFiles(dir, client);
    }

    void recvFileFromServer(){
        recvFile(client);
    }

    void recvAllFilesFromServer(){
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
    
    void sendFileToClient(std::string path){
        
        sendFile(path, server);
    }

    void sendAllFilesToClient(std::string dir){
        sendAllFiles(dir, server);
    }

    void recvFileFromClient(){
        recvFile(server);
    }

    void recvAllFilesFromClient(){
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