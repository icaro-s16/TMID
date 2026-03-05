#include "src/connections.hpp"

int main(){
    ClientSocket clientSocket(ConnectionProtocol::IPV4);
    Client client(clientSocket);
    client.createClient("127.0.0.1");
    client.recvAllFilesFromServer();
    return 0;
}