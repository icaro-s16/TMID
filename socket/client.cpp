#include "sockets.hpp"
#include <string.h>

int main(){
    char* s = "FALA MEUS MANOS AI!";
    Client client;
    client.setServerAddress("127.0.0.1", 8080);
    client.connectToServer();
    client.sendBuffer(s, strlen(s));
    return 0;
}