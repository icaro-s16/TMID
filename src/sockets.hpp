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
    typedef SO_REUSEADDR SO_EXCLUSIVEADDRUSE
    typedef SOL_TCP IPPROTO_TCP
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #define CLOSE(fd)(close(fd))
#endif

#include <iostream>

enum class ConnectionProtocol { IPV4, IPV6 };
// =================
// SOCKET BASE CLASS
// =================
class Socket{
public:    
    Socket(ConnectionProtocol cp): m_cp(cp) {
        int domain, type = SOCK_STREAM, protocol = 0;

        if (m_cp == ConnectionProtocol::IPV4)
            domain = AF_INET;
        else
            domain = AF_INET6;

        socket_fd = socket(domain, type, protocol);
        if (socket_fd < 0) std::cerr << "[ERROR] Fail to create a socket" << std::endl;
    }

    ConnectionProtocol protocol() { return m_cp; }
    ~Socket(){ CLOSE(socket_fd); }
    virtual ssize_t sendBuffer(const char* buffer, size_t size_buffer) = 0;
    virtual ssize_t readBuffer(char *buffer, size_t size_buffer) = 0;

protected:
    int socket_fd;
    ConnectionProtocol m_cp;
};

// =================
// SERVER SOCKET
// =================
class ServerSocket: public Socket{
public:
    ServerSocket(ConnectionProtocol cp): Socket(cp) {
        if (m_cp == ConnectionProtocol::IPV4)
            address_len = sizeof(ipv4_address);
        else
            address_len = sizeof(ipv6_address);
        
        int opt_value = 0;
        if (setsockopt(socket_fd, SOL_TCP, TCP_NODELAY,  &opt_value, sizeof(int)) < 0)
            std::cerr << "[ERROR] Fail to set socket options" << std::endl;
    }

    ~ServerSocket(){ CLOSE(client_socket_fd); }
    
    void bindSocket() {
        sockaddr *addr;
        if (m_cp == ConnectionProtocol::IPV4)
            addr = (sockaddr*)&ipv4_address;
        else
            addr = (sockaddr*)&ipv6_address;

        int result = bind(socket_fd, addr, address_len);
        if (result < 0)
            std::cerr << "[ERROR] Fail to bind the socket" << std::endl;
        else
            std::clog << "[LOG] Successfully binded socket" << std::endl;
    };

    int connectToClient() {
        if (listen(socket_fd, 3) < 0)
            std::cerr << "[ERROR] Fail to listen for clients" << std::endl;
        else
            std::clog << "[LOG] listening for clients..." << std::endl;

        sockaddr *addr;
        if (m_cp == ConnectionProtocol::IPV4)
            addr = (sockaddr*)&ipv4_address;
        else
            addr = (sockaddr*)&ipv6_address;

        client_socket_fd = accept(socket_fd, addr, &address_len);
        if (client_socket_fd < 0)
            std::cerr << "[ERROR] Fail to connect to the client" << std::endl;
        else
            std::clog << "[LOG] Successfully connected with client" << std::endl;

        return client_socket_fd;
    };
    
    void setAddress() {
        if (m_cp == ConnectionProtocol::IPV4) {
            ipv4_address.sin_family = AF_INET;
            ipv4_address.sin_addr.s_addr = INADDR_ANY;
            ipv4_address.sin_port = htons(PORT);
        } else {
            ipv6_address.sin6_family = AF_INET6;
            ipv6_address.sin6_addr = in6addr_any;
            ipv6_address.sin6_port = htons(PORT); 
        }
    }

    ssize_t sendBuffer(const char* buffer, size_t size_buffer) override {
        return send(client_socket_fd, buffer, size_buffer, 0);
    }
 
    ssize_t readBuffer(char *buffer, size_t size_buffer) override {
        return recv(client_socket_fd, buffer, size_buffer, 0);
    }

protected:
    socklen_t address_len;
    sockaddr_in ipv4_address;
    sockaddr_in6 ipv6_address;
    int client_socket_fd;
};

// =================
// CLIENT SOCKET
// =================
class ClientSocket: public Socket{
public:
    ClientSocket(ConnectionProtocol cp): Socket(cp) {
        if (m_cp == ConnectionProtocol::IPV4)
            address_len = sizeof(ipv4_server_address);
        else
            address_len = sizeof(ipv6_server_address);
    }

    void setServerAddress(const char* addr, sa_family_t family = 0) {

        int result;
        if (m_cp == ConnectionProtocol::IPV4) {
            ipv4_server_address.sin_family = AF_INET;
            ipv4_server_address.sin_port = htons(PORT);
            result = inet_pton(AF_INET, addr, &ipv4_server_address.sin_addr.s_addr);
        } else {
            ipv6_server_address.sin6_family = AF_INET6;
            ipv6_server_address.sin6_port = htons(PORT);
            result = inet_pton(AF_INET6, addr, &ipv6_server_address.sin6_addr);   
        }

        if (result == 1)
            std::clog << "[LOG] Successfully setted server address" << std::endl;
        else if (result == 0)
            std::cerr << "[ERROR] String is not a valid address" << std::endl;
        if (result <= 0)
            std::cerr << "[ERROR] Fail to set the server address" << std::endl;
    
    }

    void connectToServer() {
        int result;
        sockaddr *addr;

        if (m_cp == ConnectionProtocol::IPV4)
            addr = (sockaddr*)&ipv4_server_address;
        else
            addr = (sockaddr*)&ipv6_server_address;
        
        result = connect(socket_fd, addr, address_len);
        if (result == 0)
            std::clog << "[LOG] Successfully connected to server" << std::endl;
        else
            std::cerr << "[ERROR] Fail to connect to server" << std::endl;

    }

    ssize_t sendBuffer(const char* buffer, size_t size_buffer) override {
        return send(socket_fd, buffer, size_buffer, 0);
    }

    ssize_t readBuffer(char* buffer, size_t size_buffer) override {
        return recv(socket_fd, buffer, size_buffer, 0);
    }

protected:
    socklen_t address_len;
    sockaddr_in ipv4_server_address;
    sockaddr_in6 ipv6_server_address;
};

#endif