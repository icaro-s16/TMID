#include "network/sockets.hpp"

/* Socket implementations */
Socket::Socket(ConnectionProtocol _cp, int _fd): m_cp(_cp), socket_fd(_fd) {
    if (socket_fd == -1)
        setSocket();
}

Socket::~Socket(){ close(socket_fd); }

void Socket::setSocket() {
    int domain, type = SOCK_STREAM, protocol = 0;

    if (m_cp == ConnectionProtocol::IPV4)
        domain = AF_INET;
    else
        domain = AF_INET6;

    socket_fd = socket(domain, type, protocol);
    if (socket_fd < 0) std::cerr << "[ERROR] Fail to create a socket" << std::endl;
}

ssize_t Socket::send(const void* buffer, size_t size_buffer) {
    return ::send(socket_fd, buffer, size_buffer, 0);
}

ssize_t Socket::recieve(void *buffer, size_t size_buffer) {
    return ::recv(socket_fd, buffer, size_buffer, 0);
}

/* ServerSocket implementations */
ServerSocket::ServerSocket(ConnectionProtocol cp): Socket(cp) {
    if (m_cp == ConnectionProtocol::IPV4)
        address_len = sizeof(ipv4_address);
    else
        address_len = sizeof(ipv6_address);
    
    int optvalue = 1;
    if (setsockopt(socket_fd, SOL_TCP, TCP_NODELAY, &optvalue, sizeof(optvalue)) < 0)
        std::cerr << "[ERROR] Fail to set socket options" << std::endl;
    else
        std::clog << "[LOG] successfully setted socket options" << std::endl;
}

bool ServerSocket::bindSocket() {
    sockaddr *addr;
    if (m_cp == ConnectionProtocol::IPV4)
        addr = (sockaddr*)&ipv4_address;
    else
        addr = (sockaddr*)&ipv6_address;

    int result = bind(socket_fd, addr, address_len);
    return !(result < 0);
};

ClientSocket ServerSocket::listen() {
    if (::listen(socket_fd, 3) < 0)
        std::cerr << "[ERROR] Fail to listen for clients" << std::endl;
    else
        std::clog << "[LOG] listening for clients..." << std::endl;

    sockaddr *addr;
    if (m_cp == ConnectionProtocol::IPV4)
        addr = (sockaddr*)&ipv4_address;
    else
        addr = (sockaddr*)&ipv6_address;

    int client_fd = accept(socket_fd, addr, &address_len);
    if (client_fd < 0) {
        std::cerr << "[ERROR] Fail to connect to the client" << std::endl;
        throw std::runtime_error("Failed to connect with client.");
    } else
        std::clog << "[LOG] Successfully connected with client" << std::endl;
    
    ClientSocket clientSocket(m_cp, client_fd);
    return clientSocket;
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

/* ClientSocket implementations */
ClientSocket::ClientSocket(ConnectionProtocol cp, int _fd): Socket(cp, _fd) {
    if (m_cp == ConnectionProtocol::IPV4)
        address_len = sizeof(ipv4_server_address);
    else if (m_cp == ConnectionProtocol::IPV4)
        address_len = sizeof(ipv6_server_address);
    else throw std::runtime_error("IP protocol in use is undefined.");
}

void ClientSocket::setServerAddress(const char* addr) {

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

bool ClientSocket::connectTimeOut(int socket_fd, sockaddr* addr) {
    // Two synRetries equiv 7 seconds
    int synRetries = 2;
    if(setsockopt(socket_fd, IPPROTO_TCP, TCP_SYNCNT, &synRetries, sizeof(synRetries)) < 0) return true;
    if(::connect(socket_fd, addr, address_len) < 0) return true;

    return false;
}