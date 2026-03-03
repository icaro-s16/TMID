#include "src/connections.hpp"

int main(){
    int opt = 1;
    Server server(&opt);
    server.connectClient();
    server.recvFilesFromClient("retorno2.pdf");
    return 0;
}