#include "src/sockets.hpp"
#include <fstream>
#include <string>
#include <vector>

void recvFiles(std::string path, Server& server){
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
    server.readBuffer(size, 4);
    auto fileSize = std::stoul(size);
    const auto constFileSize = fileSize;
    std::vector<char> bytes(fileSize);
    auto pos = 0;
    auto readBytes = 0;
    while(fileSize > 0){
        readBytes = (fileSize >= chunckSize) ? chunckSize : fileSize;
        auto recvBytes = server.readBuffer(&bytes[pos++ * chunckSize],  readBytes);
        if (recvBytes <= 0) break;
        fileSize -= readBytes;
    } 
    outFile.write(&bytes[0], constFileSize);
    outFile.flush();
    outFile.close();
}


int main(){
    int opt = 1;
    Server server(&opt);
    server.setAddress(8080);
    server.bindSocket();
    server.connectToClient();
    recvFiles("retorno.txt", server);
    return 0;
}