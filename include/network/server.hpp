#include "network/connections.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <memory>
#include <stdexcept>

class Server {
public:
    Server(ConnectionProtocol cp) 
        : m_group(group::read_group_config()), 
          m_serverSocket(cp)
    {
        m_serverSocket.setAddress();
        
        if (!m_serverSocket.bindSocket()) {
            throw std::runtime_error("Failed to bind the server socket.");
        }
    }

    void run() {
        ClientSocket clientSocket = m_serverSocket.listen();
        std::string buffer = " ";
        
        connection::recvMsg(buffer, clientSocket);
        std::cout << "connection type: '" << buffer << "'\n";
        
        if (buffer == "r") {
            connection::recvFiles(clientSocket);
        } else if (buffer == "s") {
            std::filesystem::path path = std::filesystem::current_path();
            connection::sendFiles(fileutils::getFilesFromFolder(path), clientSocket);
        } else {
            std::cerr << "[WARNING] Unknown connection type received: " << buffer << "\n";
        }
    }

private:
    std::unique_ptr<Group> m_group;
    ServerSocket m_serverSocket;
};