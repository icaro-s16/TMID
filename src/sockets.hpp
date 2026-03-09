#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#define PORT 8080

#if defined(_WIN32) || defined(_WIN64)
    #pragma comment(lib, "ws2_32.lib")
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define ENABLE(a)((char*)a)
    #define CLOSE(fd)(closesocket(fd))
    typedef int socklen_t;
    typedef unsigned short sa_family_t;
    #define SOL_TCP IPPROTO_TCP
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <sys/time.h>
    #include <poll.h>
    #include <fcntl.h>
    #define ENABLE(a)(a)
    #define CLOSE(fd)(close(fd))
#endif

#include <iostream>
#include "connection_protocol.hpp"

class Socket{
public:
    Socket(){}
    Socket(ConnectionProtocol cp);
    ~Socket();
    void setSocket(ConnectionProtocol cp);
    ConnectionProtocol getProtocol();
    virtual ssize_t sendBuffer(const void* buffer, size_t size_buffer) = 0;
    virtual ssize_t readBuffer(void *buffer, size_t size_buffer) = 0;
protected:
    int socket_fd;
    ConnectionProtocol m_cp;
};

class ServerSocket: public Socket{
public:
    ServerSocket(ConnectionProtocol cp);
    ~ServerSocket();
    void bindSocket();
    int connectToClient();
    void setAddress();
    ssize_t sendBuffer(const void* buffer, size_t size_buffer);
    ssize_t readBuffer(void *buffer, size_t size_buffer);

protected:
    socklen_t address_len;
    sockaddr_in ipv4_address;
    sockaddr_in6 ipv6_address;
    int client_socket_fd;
};

class ClientSocket: public Socket{
public:
    ClientSocket();
    ClientSocket(ConnectionProtocol cp);
    void setIp(ConnectionProtocol cp);
    void setServerAddress(const char* addr, sa_family_t family = 0);
    bool connectToServer();
    ssize_t sendBuffer(const void* buffer, size_t size_buffer);
    ssize_t readBuffer(void* buffer, size_t size_buffer);

protected:
    bool connectTimeOut(int socket_fd, sockaddr* addr);
    socklen_t address_len;
    sockaddr_in ipv4_server_address;
    sockaddr_in6 ipv6_server_address;
};

#endif