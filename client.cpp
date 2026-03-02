#include "src/connections.hpp"

int main(){
    Client client;
    client.createClient("127.0.0.1");
    client.sendFilesToServer("envio.txt");
    return 0;
}