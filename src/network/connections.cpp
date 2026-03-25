#include "network/connections.hpp"

/* connections implementations */
void connection::sendFile(const std::filesystem::path& path, Socket& socket) {
    std::string fileName = path.filename();
    
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

void connection::recvFile(std::pair<std::string, std::vector<char>> &file, Socket& socket) {
    std::clog << "recieving file...\n";
    char header[MAX_CHUNK_SIZE];
    memset(header, 0, MAX_CHUNK_SIZE);
    size_t header_recv_bytes = 0;

    while(header_recv_bytes < MAX_CHUNK_SIZE){
        header_recv_bytes += socket.recieve(&header[header_recv_bytes], MAX_CHUNK_SIZE - header_recv_bytes);
    }

    // This funtion destroy the char*
    HeaderFile header_values = HeaderFile::parseHeaderFile(header);


    std::vector<char> data(header_values.bytesLen);
    std::size_t bytesRemaining = header_values.bytesLen;
    std::size_t currentOffset = 0;

    while(bytesRemaining > 0){
        ssize_t requestSize = (bytesRemaining >= MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : bytesRemaining;
        ssize_t recvBytes = socket.recieve(&data.data()[currentOffset],  requestSize);
        
        if (recvBytes <= 0) break;

        currentOffset += recvBytes;
        bytesRemaining -= recvBytes;
    }
    // memset(data, 0, header_values.bytesLen);
    file.first = header_values.name;
    file.second = data; 
    std::clog << "done recieving file...\n";
}

void connection::sendFiles(std::vector<std::filesystem::path> paths, Socket& socket){
    char header[MAX_CHUNK_SIZE];
    memset(header, 0, MAX_CHUNK_SIZE);

    sprintf(
        header,
        "Amount:%i\r\n\r\n",
        paths.size()
    );

    socket.send(header, MAX_CHUNK_SIZE);

    for(std::filesystem::path path: paths)
        connection::sendFile(path, socket);
}

void connection::recvFiles(std::map<std::string, std::vector<char>> &files, Socket &socket) {
    char buffer[MAX_CHUNK_SIZE];
    size_t recvAmountSize = 0;
    while(recvAmountSize < MAX_CHUNK_SIZE) {
        recvAmountSize += socket.recieve(buffer, MAX_CHUNK_SIZE);
    }
    size_t file_counter = HeaderFile::parseHeaderAmount(buffer);

    for(auto i = 0; i < file_counter; i++) {
        std::pair<std::string, std::vector<char>> file;
        connection::recvFile(file, socket);
        files[file.first] = file.second;   
    }
}

ssize_t connection::sendMsg(std::string msg, Socket& socket){
    return socket.send(&msg[0], msg.size());
}

ssize_t connection::recvMsg(std::string &buffer, Socket& socket){
    return socket.recieve(&buffer[0], buffer.size());
}