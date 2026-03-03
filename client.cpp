#include "src/connections.hpp"

int main(){
    
    #ifdef _WIN32
        WSADATA wsaData;
        int iResult;

        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0){
            std::cerr << "[ERROR] Fail to initialize the WSAStartup" << std::endl;
            return -1;
        }
    #endif

    Client client;
    client.createClient("127.0.0.1");
    client.sendFilesToServer("lf-4-em-busca-do-tempo-perdido-marcel-proust.pdf");
    
    #ifdef _WIN32
        WSACleanup();
    #endif
    
    return 0;
}