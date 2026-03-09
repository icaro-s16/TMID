#include "sockets.hpp"

/* Socket implementations */
Socket::Socket(){}

Socket::Socket(ConnectionProtocol cp): m_cp(cp) {
    int domain, type = SOCK_STREAM, protocol = 0;

    if (m_cp == ConnectionProtocol::IPV4)
        domain = AF_INET;
    else
        domain = AF_INET6;

    socket_fd = socket(domain, type, protocol);
    if (socket_fd < 0) std::cerr << "[ERROR] Fail to create a socket" << std::endl;
}

Socket::~Socket(){ CLOSE(socket_fd); }

void Socket::setSocket(ConnectionProtocol cp){
    m_cp = cp;
    int domain, type = SOCK_STREAM, protocol = 0;

    if (m_cp == ConnectionProtocol::IPV4)
        domain = AF_INET;
    else
        domain = AF_INET6;

    socket_fd = socket(domain, type, protocol);
    if (socket_fd < 0) std::cerr << "[ERROR] Fail to create a socket" << std::endl;
}

ConnectionProtocol Socket::getProtocol() { return m_cp; }


/* ServerSocket implementations */
ServerSocket::ServerSocket(ConnectionProtocol cp): Socket(cp) {
    if (m_cp == ConnectionProtocol::IPV4)
        address_len = sizeof(ipv4_address);
    else
        address_len = sizeof(ipv6_address);
    
    #if defined(_WIN32) || defined(_WIN64)
    bool _optvalue = true;
    char *optvalue = (char*) &_optvalue; 
    #else
    int _optvalue = 1;
    int *optvalue = &_optvalue;
    #endif

    if (setsockopt(socket_fd, SOL_TCP, TCP_NODELAY,  optvalue, sizeof(_optvalue)) < 0)
        std::cerr << "[ERROR] Fail to set socket options" << std::endl;
    else
        std::clog << "[LOG] successfully setted socket options" << std::endl;
}

ServerSocket::~ServerSocket(){ CLOSE(client_socket_fd); }

void ServerSocket::bindSocket() {
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

int ServerSocket::connectToClient() {
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

void ServerSocket::setAddress() {
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

ssize_t ServerSocket::sendBuffer(const void* buffer, size_t size_buffer) {
    return send(client_socket_fd, buffer, size_buffer, 0);
}

ssize_t ServerSocket::readBuffer(void *buffer, size_t size_buffer) {
    return recv(client_socket_fd, buffer, size_buffer, 0);
}

/* ClientSocket implementations */
ClientSocket::ClientSocket(){}

ClientSocket::ClientSocket(ConnectionProtocol cp): Socket(cp) {
    if (m_cp == ConnectionProtocol::IPV4)
        address_len = sizeof(ipv4_server_address);
    else
        address_len = sizeof(ipv6_server_address);
}

void ClientSocket::setIp(ConnectionProtocol cp){
    setSocket(cp);
    if (m_cp == ConnectionProtocol::IPV4)
        address_len = sizeof(ipv4_server_address);
    else
        address_len = sizeof(ipv6_server_address);
}

void ClientSocket::setServerAddress(const char* addr, sa_family_t family = 0) {

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

bool ClientSocket::connectToServer() {
    int result;
    sockaddr *addr;

    if (m_cp == ConnectionProtocol::IPV4)
        addr = (sockaddr*)&ipv4_server_address;
    else
        addr = (sockaddr*)&ipv6_server_address;
    if (connectTimeOut(socket_fd, addr)){
        std::cerr << "[ERROR] Fail to connect to server" << std::endl;
        return false;
    }
    std::clog << "[LOG] Successfully connected to server" << std::endl;
    return true;
}

ssize_t ClientSocket::sendBuffer(const void* buffer, size_t size_buffer) {
    return send(socket_fd, buffer, size_buffer, 0);
}

ssize_t ClientSocket::readBuffer(void* buffer, size_t size_buffer) {
    return recv(socket_fd, buffer, size_buffer, 0);
}

bool ClientSocket::connectTimeOut(int socket_fd, sockaddr* addr){
    // Two synRetries equiv 7 seconds
    int synRetries = 2;
    if(setsockopt(socket_fd, IPPROTO_TCP, TCP_SYNCNT, &synRetries, sizeof(synRetries)) < 0) return true;
    if(connect(socket_fd, addr, address_len) < 0) return true;

    return false;
}