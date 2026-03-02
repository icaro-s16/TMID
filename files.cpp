#include "src/sockets.hpp"
#include <fstream>
#include <filesystem>
#include <string.h>
#include <vector>

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

/*
Chuncks => 1024 bytes
*/
void sendFiles(std::string path, Client& client){
    auto bytes = readAllBytes(path);
    auto sizeBytes = bytes.size();
    std::string st_sizeBytes = std::to_string(sizeBytes);
    client.sendBuffer(st_sizeBytes.c_str(), st_sizeBytes.size());
    auto pos = 0;
    auto chunckSize = 1024;
    auto sendChunck = 0;
    while(sizeBytes > 0){
        sendChunck = (sizeBytes >= chunckSize)? chunckSize : sizeBytes ; 
        client.sendBuffer(&bytes[pos++ * chunckSize], sendChunck);
        sizeBytes -= sendChunck;
    }
}

int main(){
    Client client;
    client.setServerAddress("127.0.0.1", 8080);
    client.connectToServer();
    sendFiles("envio.txt", client);
    return 0;
}