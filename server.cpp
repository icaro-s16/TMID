#include "src/connections.hpp"

int main(){
    #ifdef _WIN32
        WSADATA wsaData;
        int iResult;

        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0){
            std::cerr << "[ERROR] Fail to initialize the WSAStartup" << std::endl;
            return 1;
        }
    #endif
    int opt = 1;
    Server server(&opt);
    server.connectClient();
    server.recvFilesFromClient("retorno2.pdf");
    #ifdef _WIN32
        WSACleanup();
    #endif
    return 0;
}