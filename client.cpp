#include "src/connections.hpp"

int main(){
    Client client;
    client.createClient("127.0.0.1");
    client.sendFilesToServer("lf-4-em-busca-do-tempo-perdido-marcel-proust.pdf");
    return 0;
}