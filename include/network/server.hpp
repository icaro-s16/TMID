#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <iostream>
#include <string>
#include <filesystem>
#include <memory>
#include <stdexcept>

#include "models/group.hpp"
#include "network/connections.hpp"

class Server {
public:
    Server(ConnectionProtocol cp): m_serverSocket(cp)
    {
        m_serverSocket.setAddress();
        
        if (!m_serverSocket.bindSocket()) {
            throw std::runtime_error("Failed to bind the server socket.");
        }
    }

    void run(const Group& group)
    {
        std::cout << "server: " << group.m_name << "[" << group.m_alias << "]" << std::endl;
        ClientSocket clientSocket = m_serverSocket.listen();
        
        std::string buffer = "0000"; // TODO: Change this later. 4 byte messages
        std::map<std::string, std::vector<char>> files = {};

        connection::recvMsg(buffer, clientSocket);

        std::cout << "connection type: '" << buffer << "'\n";
        
        if (buffer == Op::SUBMIT) {
            connection::recvFiles(files, clientSocket);

        } else if (buffer == Op::SYNC) {
            std::filesystem::path path = OWNED_GROUPS_FOLDER / group.m_alias;
            auto tasksFolders = fileutils::getFoldersFromFolder(path);
            
            for (auto folder : tasksFolders) {
                auto taskContent = fileutils::getFilesFromFolder(folder);
                connection::sendFiles(taskContent, clientSocket);
            }
        } else {
            std::cerr << "[WARNING] Unknown connection type received: " << buffer << "\n";
        }

    }

private:
    ServerSocket m_serverSocket;
};

#endif