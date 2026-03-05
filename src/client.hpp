#include "connections.hpp"

class IClient {
public:
    void run(std::string ip) {
        #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
            std::cerr << "[ERROR] Error at WSAStartup()\n";
        }
        #endif

        ClientSocket clientSocket(ConnectionProtocol::IPV4);
        Client client(clientSocket);
        client.createClient(ip);
        std::filesystem::path path = std::filesystem::current_path();
        client.sendAllFilesToServer(path);
        
        #if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
        #endif
    }
};