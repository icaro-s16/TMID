#include "src/connections.hpp"

int main(){
    ServerSocket serverSocket(ConnectionProtocol::IPV4);
    serverSocket.setAddress();
    Server server(serverSocket);
    server.connectClient();
    server.sendAllFilesToClient("src");
    return 0;
}