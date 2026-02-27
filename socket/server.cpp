#include "sockets.hpp"
#include <iostream>
#define PORT 8080

int main(){
    char buffer[1024] = {0};
    int opt = 1;
    Server s(&opt);
    s.setAddress(PORT);
    s.bindSocket();
    s.connectToClient();
    s.readBuffer(buffer, 1024);
    std::string x = buffer;
    std::cout << x << std::endl;
    return 0;
}