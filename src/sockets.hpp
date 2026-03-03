#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#define PORT 8080

#ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define CLOSE(fd)(closesocket(fd))
    typedef int socklen_t;
    typedef unsigned short sa_family_t;
    #define SO_REUSEADDR SO_EXCLUSIVEADDRUSE
    #define SOL_TCP IPPROTO_TCP
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #define CLOSE(fd)(close(fd))
#endif

#include <iostream>

class Socket{
public:    
    Socket(
        int domain = AF_INET6,
        int type = SOCK_STREAM    
    ){
        socket_fd = socket(domain, type, 0);
        if (socket_fd < 0) std::cerr << "[ERROR] Fail to create a socket" << std::endl;
    }
    virtual ~Socket(){
        CLOSE(socket_fd);
    }
    
    virtual ssize_t sendBuffer(const char* buffer, size_t size_buffer) = 0;

    virtual ssize_t readBuffer(char *buffer, size_t size_buffer) = 0;

protected:
    int socket_fd;
};


class ServerSocket: public Socket{
public:
    ServerSocket(
        int *opt_value,
        int opt_name,
        int level,
        int domain,
        int type
    ):Socket(domain, type){
        if (setsockopt(socket_fd, level, opt_name,  opt_value, sizeof(int)) < 0)
            std::cerr << "[ERROR] Fail to set socket options" << std::endl;
            
    }
    virtual ~ServerSocket(){
        CLOSE(client_socket_fd);
    }

    virtual void bindSocket() = 0;
    
    virtual int connectToClient() = 0;

    ssize_t sendBuffer(const char* buffer, size_t size_buffer) override {
        return send(client_socket_fd, buffer, size_buffer, 0);
    }
 
    ssize_t readBuffer(char *buffer, size_t size_buffer) override {
        return recv(client_socket_fd, buffer, size_buffer, 0);
    }

protected:
    socklen_t address_len;
    int client_socket_fd;
};

class ServerSocketIPv6: public ServerSocket{
public:    
    ServerSocketIPv6(
        int *opt_value,
        int opt_name = TCP_NODELAY,
        int level = SOL_TCP,
        int domain = AF_INET6,
        int type = SOCK_STREAM
    ): ServerSocket(opt_value, opt_name, level, domain, type){
        address_len = sizeof(address);
    }
    void setAddress(
        uint16_t port = PORT,
        sa_family_t family = AF_INET6,
        in6_addr s_addr = in6addr_any
    ){
        address.sin6_family = family;
        address.sin6_addr = s_addr;
        address.sin6_port = htons(port);
    }

    void bindSocket() override{
        if (bind(socket_fd, (sockaddr*)&address, sizeof(address)) < 0)
            std::cerr << "[ERROR] Fail to bind the socket" << std::endl;
    }
    
    int connectToClient() override{
        if (listen(socket_fd, 3) < 0)
            std::cerr << "[ERROR] Fail to listen to clients" << std::endl;
        client_socket_fd = accept(socket_fd, (sockaddr*)&address, &address_len);
        if (client_socket_fd < 0)
            std::cerr << "[ERROR] Fail to connect to the client" << std::endl;
        return client_socket_fd;
    }

private:
    sockaddr_in6 address;
};

class ServerSocketIPv4: public ServerSocket{
public:    
    ServerSocketIPv4(
        int *opt_value,
        int opt_name = TCP_NODELAY,
        int level = SOL_TCP,
        int domain = AF_INET,
        int type = SOCK_STREAM
    ): ServerSocket(opt_value, opt_name, level, domain, type){
        address_len = sizeof(address);
    }

    void setAddress(
        uint16_t port = PORT,
        sa_family_t family = AF_INET,
        in_addr_t s_addr = INADDR_ANY
    ){
        address.sin_family = family;
        address.sin_addr.s_addr = s_addr;
        address.sin_port = htons(port);
    }

    void bindSocket() override{
        if (bind(socket_fd, (sockaddr*)&address, sizeof(address)) < 0)
            std::cerr << "[ERROR] Fail to bind the socket" << std::endl;
    }
    
    int connectToClient() override{
        if (listen(socket_fd, 3) < 0)
            std::cerr << "[ERROR] Fail to listen to clients" << std::endl;
        client_socket_fd = accept(socket_fd, nullptr, nullptr);
        if (client_socket_fd < 0)
            std::cerr << "[ERROR] Fail to connect to the client" << std::endl;
        return client_socket_fd;
    }

private:
    sockaddr_in address;
};

class ClientSocket: public Socket{
public:
    ClientSocket(
        int domain,
        int type    
    ): Socket(domain, type) {}

    virtual void setServerAddress(
        const char* addr,
        uint16_t port = PORT,
        sa_family_t family = 0
    ){
        (void)(addr);
        (void)(port);
        (void)(family);
    }

    virtual void connectToServer(){}

    ssize_t sendBuffer(const char* buffer, size_t size_buffer) override {
        return send(socket_fd, buffer, size_buffer, 0);
    }

    ssize_t readBuffer(char* buffer, size_t size_buffer) override {
        return recv(socket_fd, buffer, size_buffer, 0);
    }

protected:
    socklen_t address_len;

};

class ClientSocketIPv6: public ClientSocket{
public:
    ClientSocketIPv6(
        int domain = AF_INET6,
        int type = SOCK_STREAM   
    ): ClientSocket(domain, type){
        address_len = sizeof(server_address);
    }

    void setServerAddress(
        const char* addr,
        uint16_t port,
        sa_family_t family
    ) override{
        if(family == 0) family = AF_INET6;
        server_address.sin6_family = family;
        server_address.sin6_port = htons(port);
        if (inet_pton(family, addr, &server_address.sin6_addr) <= 0)
            std::cerr << "[ERROR] Fail to set the server address" << std::endl;       
    }

    void connectToServer() override{
        if (connect(socket_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0)
            std::cerr << "[ERROR] Fail to connect to the server" << std::endl;
    }

private:
    sockaddr_in6 server_address;
};

class ClientSocketIPv4: public ClientSocket{
public:
    ClientSocketIPv4(
        int domain = AF_INET,
        int type = SOCK_STREAM   
    ): ClientSocket(domain, type){
        address_len = sizeof(server_address);
    }

    void setServerAddress(
        const char* addr,
        uint16_t port,
        sa_family_t family
    ) override{
        if(family == 0) family = AF_INET;
        server_address.sin_family = family;
        server_address.sin_port = htons(port);
        if (inet_pton(family, addr, &server_address.sin_addr.s_addr) <= 0)
            std::cerr << "[ERROR] Fail to set the server address" << std::endl;       
    }

    void connectToServer() override{
        if (connect(socket_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0)
            std::cerr << "[ERROR] Fail to connect to the server" << std::endl;
    }

private:
    sockaddr_in server_address;
};

#endif