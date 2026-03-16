#include "network/connections.hpp"

/* connections implementations */
void connection::sendFile(const std::string& path, Socket& socket) {
    std::vector<std::string> file_tokens = strutils::splitText(path, '/');
    std::string fileName = file_tokens.back();
    
    std::vector<char> fileData = fileutils::getBytesFromFile(path);
    if (fileData.empty()) return; 

    HeaderFile header_values;
    header_values.name = fileName;
    header_values.bytesLen = fileData.size();

    char header[MAX_CHUNK_SIZE];
    memset(header, 0, MAX_CHUNK_SIZE);

    snprintf(header, MAX_CHUNK_SIZE, "Name:%s\r\nFileSize:%zu\r\n\r\n", 
             header_values.name.c_str(), 
             header_values.bytesLen);

    socket.send(header, MAX_CHUNK_SIZE);

    size_t currentOffset = 0;
    size_t bytesRemaining = header_values.bytesLen;

    while(bytesRemaining > 0) {
        size_t chunkSize = (bytesRemaining >= MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : bytesRemaining; 
        
        ssize_t sentBytes = socket.send(fileData.data() + currentOffset, chunkSize);
        
        if (sentBytes <= 0) break;

        currentOffset += sentBytes;
        bytesRemaining -= sentBytes;
    }
    
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
        connection::sendFile(fileName, socket);
}

void connection::recvFiles(Socket& socket){
    char buffer[MAX_CHUNK_SIZE];
    size_t recvAmountSize = 0;
    while(recvAmountSize < MAX_CHUNK_SIZE){
        recvAmountSize += socket.recieve(buffer, MAX_CHUNK_SIZE);
    }
    size_t file_counter = HeaderFile::parseHeaderAmount(buffer);

    for(auto i = 0; i < file_counter; i++)
        connection::recvFile(socket);
}

ssize_t connection::sendMsg(std::string msg, Socket& socket){
    return socket.send(&msg[0], msg.size());
}

ssize_t connection::recvMsg(std::string &buffer, Socket& socket){
    return socket.recieve(&buffer[0], buffer.size());
}