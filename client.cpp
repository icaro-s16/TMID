#include "src/connections.hpp"

int main(){
    ClientSocketIPv4 clientIPv4;
    Client client(clientIPv4);
    client.createClient("127.0.0.1");
    client.sendAllFilesToServer("src");
    return 0;
}