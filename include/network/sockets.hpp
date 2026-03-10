#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#define PORT 8080

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>

#include "connection_protocol.hpp"

class Socket{
public:
    Socket(ConnectionProtocol _cp, int _fd = -1);
    
    /* Copy, self-asignment guard, move and destructor implementation */
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept : socket_fd(other.socket_fd) {
        other.socket_fd = -1;
    }
    ~Socket();
    
    void setSocket();
    ssize_t send(const void* buffer, size_t size_buffer);
    ssize_t recieve(void *buffer, size_t size_buffer);
protected:
    int socket_fd;
    ConnectionProtocol m_cp;
};

class ClientSocket: public Socket {
public:
    ClientSocket(ConnectionProtocol cp, int _fd = -1);
    void setIp(ConnectionProtocol cp);
    void setServerAddress(const char* addr);
    bool connectToServer();
private:
    socklen_t address_len;
    sockaddr_in ipv4_server_address;
    sockaddr_in6 ipv6_server_address;
    bool connectTimeOut(int socket_fd, sockaddr* addr);
};

class ServerSocket: public Socket{
public:
    ServerSocket(ConnectionProtocol cp);
    void bindSocket();
    ClientSocket listen();
    void setAddress();
private:
    socklen_t address_len;
    sockaddr_in ipv4_address;
    sockaddr_in6 ipv6_address;
};

#endif