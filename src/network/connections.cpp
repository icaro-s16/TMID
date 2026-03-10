#include "network/connections.hpp"

/* connections implementations */
void connection::sendFile(std::string path, Socket& socket){
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

    socket.send(&header, MAX_CHUNK_SIZE);

    size_t currentffSet = 0;
    size_t bytesRemaining = bytesLen;

    while(bytesRemaining > 0) {
        size_t chunkSize = (bytesRemaining >= MAX_CHUNK_SIZE)? MAX_CHUNK_SIZE : bytesRemaining; 
        size_t sendBytes = socket.send(&data[currentffSet], chunkSize);
        currentffSet += sendBytes;
        bytesRemaining -= sendBytes;
    }
    memset(data, 0, bytesLen);
    delete[] data;
    
}

void connection::recvFile(Socket& socket){
    char header[MAX_CHUNK_SIZE];
    memset(header, 0, MAX_CHUNK_SIZE);
    size_t header_recv_bytes = 0;

    while(header_recv_bytes < MAX_CHUNK_SIZE){
        
        header_recv_bytes += socket.recieve(&header[header_recv_bytes], MAX_CHUNK_SIZE - header_recv_bytes);
    
    }

    // This funtion destroy the char*
    HeaderFile header_values = HeaderFile::parseHeaderFile(header);

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
        ssize_t recvBytes = socket.recieve(&data[currentOffset],  requestSize);
        
        if (recvBytes <= 0) break;

        currentOffset += recvBytes;
        bytesRemaining -= recvBytes;
    }

    outFile.write(data, (std::streamsize) header_values.bytesLen);
    outFile.close();
    memset(data, 0, header_values.bytesLen);
    delete[] data;
    
}

void connection::sendFiles(std::vector<std::string> paths, Socket& socket){
    char header[MAX_CHUNK_SIZE];
    memset(header, 0, MAX_CHUNK_SIZE);

    sprintf(
        header,
        "Amount:%i\r\n\r\n",
        paths.size()
    );

    socket.send(header, MAX_CHUNK_SIZE);

    for(std::string fileName: paths)
        sendFile(fileName, socket);
}

void connection::recvFiles(Socket& socket){
    char buffer[MAX_CHUNK_SIZE];
    size_t recvAmountSize = 0;
    while(recvAmountSize < MAX_CHUNK_SIZE){
        recvAmountSize += socket.recieve(buffer, MAX_CHUNK_SIZE);
    }
    size_t file_counter = HeaderFile::parseHeaderAmount(buffer);

    for(auto i = 0; i < file_counter; i++)
        recvFile(socket);
}

ssize_t connection::sendMsg(std::string msg, Socket& socket){
    return socket.send(&msg[0], msg.size());
}

ssize_t connection::recvMsg(std::string &buffer, Socket& socket){
    return socket.recieve(&buffer[0], buffer.size());
}

/* Client implementations */
Client::Client(ClientSocket &_client):client(_client){}

bool Client::createClient(std::string ip){
    client.setServerAddress(ip.c_str());
    return client.connectToServer();
}\

void Client::sendFile(std::string path){
    connection::sendFile(path, client);
}

void Client::sendFiles(std::vector<std::string> paths){
    connection::sendFiles(paths, client);
}

void Client::recvFile(){
    connection::recvFile(client);
}

void Client::recvFiles(){
    connection::recvFiles(client);
}

ssize_t Client::sendMsg(std::string msg){
    return connection::sendMsg(msg, client);
}

ssize_t Client::recvMsg(std::string &buffer){
    return connection::recvMsg(buffer, client);
}

/* server implementations */
Server::Server(ServerSocket& _server):server(_server){
    server.bindSocket();
}

ClientSocket Server::connectClient(){
    return server.listen();
}