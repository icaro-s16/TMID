#include "group.hpp"
#include "connections.hpp"

class IServer {
    std::unique_ptr<Group> m_group;
public:
    IServer(): m_group(group::read_group_config()) {}
    void run() {
        #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
            std::cerr << "[ERROR] Error at WSAStartup()\n";
        }
        #endif

        ServerSocket serverSocket(ConnectionProtocol::IPV4);
        serverSocket.setAddress();
        Server server(serverSocket);
        server.connectClient();
        server.recvAllFilesFromClient("retorno");

        #if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
        #endif
    }
};