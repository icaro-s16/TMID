#include "network/connections.hpp"
#include "utils/regex.hpp"

#include "network/connections.hpp"
#include "utils/regex.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <filesystem>

class Client {
public:
    Client(std::string ip) 
        : m_ip(ip),
          m_socket(validateIp(m_ip), -1) 
    {
        m_socket.setServerAddress(m_ip.c_str());
    }
    
    void run() {
        if (!m_socket.connectToServer()) {
            throw std::runtime_error("[ERROR] Failed to connect to server at " + m_ip);
        }
        
        connection::sendMsg("r", m_socket); // "server, Recieve those files from client"
        
        std::filesystem::path path = std::filesystem::current_path();
        connection::sendFiles(fileutils::getFilesFromFolder(path), m_socket);
    }

    void update() {
        connection::sendMsg("s", m_socket); // "please, server, Send files to me"
        connection::recvFiles(m_socket);
    }

private:
    static ConnectionProtocol validateIp(const std::string& ip) {
        ConnectionProtocol ipType = ipRegex(ip);
        if (ipType == ConnectionProtocol::UNDEF) {
            throw std::runtime_error("The IP provided for connection is undefined / has invalid format: " + ip);
        }
        return ipType;
    }

    std::string m_ip; 
    ClientSocket m_socket;
};