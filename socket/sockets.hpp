#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <err.h>


class Socket{
public:    
    Socket(
        int domain = AF_INET,
        int type = SOCK_STREAM    
    ){
        socket_fd = socket(domain, type, 0);
        if (socket_fd < 0) err(1, "Error: Fail to create a socket\n");
    }

    ~Socket(){
        close(socket_fd);
    }
protected:
    int socket_fd;
};

class Server: public Socket{
public:    
    Server(
        int *opt_value,
        int opt_name = SO_REUSEADDR,
        int level = SOL_SOCKET,
        int domain = AF_INET,
        int type = SOCK_STREAM
    ): Socket(domain, type){
        socklen_t opt_len = sizeof(opt_value);
        if (setsockopt(socket_fd, level, opt_name, opt_value, opt_len) < 0)
            err(1, "Error: Fail to set socket options\n");
        address_len = sizeof(address);
    }
    ~Server(){
        close(client_socket_fd);
    }
    void setAddress(
        uint16_t port,
        sa_family_t family = AF_INET,
        in_addr_t s_addr = INADDR_ANY
    ){
        address.sin_family = family;
        address.sin_addr.s_addr = s_addr;
        address.sin_port = htons(port);
    }

    void bindSocket(){
        if (bind(socket_fd, (sockaddr*)&address, sizeof(address)) < 0)
            err(1, "Error: Fail to bind the socket\n");
        if (listen(socket_fd, 3) < 0)
            err(1, "Error: Failt to listen to clients\n");
    }
    
    void connectToClient(){
        client_socket_fd = accept(socket_fd, (sockaddr*)&address, &address_len);
        if (client_socket_fd < 0)
            err(1, "Error: Fail to connect to the client");
    }

    ssize_t sendBuffer(const char* buffer, size_t size_buffer){
        return send(client_socket_fd, buffer, size_buffer, 0);
    }

    ssize_t readBuffer(char* buffer, size_t size_buffer){
        // - 1 to the null terminator 
        return read(client_socket_fd, buffer, size_buffer - 1);
    }

private:
    sockaddr_in address;
    socklen_t address_len;
    int client_socket_fd;
};


class Client: public Socket{
public:
    Client(
        int domain = AF_INET,
        int type = SOCK_STREAM   
    ): Socket(domain, type) {
        address_len = sizeof(server_address);
    }

    void setServerAddress(
        const char* addr,
        uint16_t port,
        sa_family_t family = AF_INET
    ){
        server_address.sin_family = family;
        server_address.sin_port = htons(port);
        if (inet_pton(family, addr, &server_address.sin_addr) <= 0)
            err(1, "Error: Fail to set the server address\n");
        
    }

    void connectToServer(){
        if (connect(socket_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0)
            err(1, "Error: Fail to connect to the server\n");
    }

    ssize_t sendBuffer(const char* buffer, size_t size_buffer){
        return send(socket_fd, buffer, size_buffer, 0);
    }

    ssize_t readBuffer(char* buffer, size_t size_buffer){
        // - 1 to the null terminator 
        return read(socket_fd, buffer, size_buffer - 1);
    }
private:
    sockaddr_in server_address;
    socklen_t address_len;

};








#endif