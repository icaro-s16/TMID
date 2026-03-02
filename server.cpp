#include "src/connections.hpp"

int main(){
    int opt = 1;
    Server server(&opt);
    server.connectClient();
    server.recvFilesFromClient("retorno.txt");
    return 0;
}