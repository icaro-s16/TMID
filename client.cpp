#include "src/connections.hpp"

int main(){
    ClientSocketIPv4 clientIPv4;
    Client client(clientIPv4);
    client.createClient("127.0.0.1");
    client.sendFilesToServer("teste.txt");
    return 0;
}