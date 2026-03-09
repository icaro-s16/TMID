#include "connections.hpp"
#include "utils.hpp"

HeaderFile::HeaderFile(): name(""), bytesLen(0), checkSum(0){};

void Connection::sendFiles(std::vector<std::string> paths, Socket& socket){
    char header[MAX_CHUNK_SIZE];
    memset(header, 0, MAX_CHUNK_SIZE);

    sprintf(
        header,
        "Amount:%i\r\n\r\n",
        paths.size()
    );

    socket.sendBuffer(header, MAX_CHUNK_SIZE);

    for(std::string fileName: paths)
        sendFile(fileName, socket);
}

void Connection::sendAllFiles(std::string dir_path, Socket& socket){
    std::vector<std::string> paths = fileutils::getFilesFromFolder(dir_path);
    sendFiles(paths, socket);
}

void Connection::recvAllFiles(Socket& socket){
    char buffer[MAX_CHUNK_SIZE];
    size_t recvAmountSize = 0;
    while(recvAmountSize < MAX_CHUNK_SIZE){
        recvAmountSize += socket.readBuffer(buffer, MAX_CHUNK_SIZE);
    }
    size_t file_counter = headerAmountParser(buffer);

    for(auto i = 0; i < file_counter; i++)
        recvFile(socket);
    
}

void Connection::sendFile(std::string path, Socket& socket){
    std::vector<std::string> file_tokens = strutils::splitText(path, '/');
    std::string fileName = file_tokens[file_tokens.size() - 1];
    
    char* data = fileutils::getBytesFromFile(path);
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

void Connection::recvFile(Socket& socket){
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

ssize_t Connection::sendMsg(std::string msg, Socket& socket){
    return socket.sendBuffer(&msg[0], msg.size());
}

ssize_t Connection::recvMsg(std::string &buffer, Socket& socket){
    return socket.readBuffer(&buffer[0], buffer.size());
}

HeaderFile Connection::headerFileParser(char* header){
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

int Connection::headerAmountParser(char* header){
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


Client::Client(ClientSocket &_client):client(_client){}

bool Client::createClient(std::string ip){
    client.setServerAddress(ip.c_str());
    client.connectToServer();
}

void Client::sendFileToServer(std::string path){
    sendFile(path, client);
}

void Client::sendAllFilesToServer(std::string dir){
    sendAllFiles(dir, client);
}

void Client::recvFileFromServer(){
    recvFile(client);
}

void Client::recvAllFilesFromServer(){
    recvAllFiles(client);
}

ssize_t Client::sendMsgToServer(std::string msg){
    return sendMsg(msg, client);
}

ssize_t Client::recvMsgFromServer(std::string &buffer){
    return recvMsg(buffer, client);
}


Server::Server(ServerSocket& _server):server(_server){
    server.bindSocket();
}

int Server::connectClient(){
    return server.connectToClient();
}

void Server::sendFileToClient(std::string path){
    
    sendFile(path, server);
}

void Server::sendAllFilesToClient(std::string dir){
    sendAllFiles(dir, server);
}

void Server::recvFileFromClient(){
    recvFile(server);
}

void Server::recvAllFilesFromClient(){
    recvAllFiles(server);
}

ssize_t Server::sendMsgToClient(std::string msg){
    return sendMsg(msg, server);
}

ssize_t Server::recvMsgFromClient(std::string &buffer){
    return recvMsg(buffer, server);
}
