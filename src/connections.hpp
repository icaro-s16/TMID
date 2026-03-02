#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <filesystem>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string.h>
#include <thread>
#include <chrono>
#include "sockets.hpp"
#define PORT 8080

class ClassClient{
public:
    ClassClient(){}

    void createClient(std::string ip){
        client.setServerAddress(ip.c_str(), PORT);
        client.connectToServer();
    }

    std::string getClientMsg(){
        char buffer[1024];
        client.readBuffer(buffer, 1024);
        return buffer;
    }


    void sendMsg(const std::string msg){
        client.sendBuffer(msg.c_str(), msg.size());
    }

    

private:
    Client client;
};

class ClassHost{
public:
    ClassHost(int *opt):host(opt){
        host.setAddress(PORT);
        host.bindSocket();
    }

    int connectClient(){
        return host.connectToClient();
    }

    std::string getClientMsg(){
        char buffer[1024];
        host.readBuffer(buffer, 1024);
        return buffer;
    }

    void sendMsg(const std::string msg){
        host.sendBuffer(msg.c_str(), msg.size());
    }

private:
    Server host;
};

#endif