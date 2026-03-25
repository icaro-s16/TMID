#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <iostream>
#include <stdexcept>
#include <utility>
#include <filesystem>

#include "network/connections.hpp"
#include "utils/regex.hpp"
#include "models/task.hpp"

class Client {
public:
    Client(std::string ip) 
        : m_ip(ip),
          m_socket(validateIp(m_ip), -1) 
    {
        m_socket.setServerAddress(m_ip.c_str());
    }
    
    void sendTaskFiles() {
        if (!m_socket.connectToServer()) {
            throw std::runtime_error("[ERROR] Failed to connect to server at " + m_ip);
        }
        
        connection::sendMsg("r", m_socket); // "server, Recieve those files from client"
        
        std::filesystem::path path = std::filesystem::current_path();
        auto files = fileutils::getFilesFromFolder(path);
        connection::sendFiles(files, m_socket);
    }

    // TODO: not doing what is suposed to do
    void getGroupTasks(std::map<Task, std::vector<char>>& tasks) {
        if (!m_socket.connectToServer())
            throw std::runtime_error("[ERROR] Failed to connect to server at " + m_ip);
        
        connection::sendMsg(Op::SYNC, m_socket);
        
        std::map<std::string, std::vector<char>> files;
        connection::recvFiles(files, m_socket);

        
        for (auto file : files) {
            Task newTask(file.first, "", {});
            tasks[newTask] = file.second;
        }
    }

    void updateLocalFiles() {
        // LEGACY. Remove this later
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

#endif