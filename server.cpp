#include "src/connections.hpp"

int main(){
    int setOpt = 1;
    ServerSocketIPv4 serverIPv4(&setOpt);
    serverIPv4.setAddress();
    Server server(serverIPv4);
    server.connectClient();
    server.recvFilesFromClient("retorno.txt");
    return 0;
}