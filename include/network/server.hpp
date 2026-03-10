#include "models/group.hpp"
#include "network/connections.hpp"

class IServer {
    std::unique_ptr<Group> m_group;
public:
    IServer(): m_group(group::read_group_config()) {}
    void run() {
        ServerSocket serverSocket(ConnectionProtocol::IPV4);
        serverSocket.setAddress();
        Server server(serverSocket);
        
        auto _client = server.connectClient();
        Client client(_client);
        std::string buffer = " ";
        
        client.recvMsg(buffer);
        std::cout << "connection type: " << buffer << "\n";
        
        if (buffer == "r")
            client.recvFiles();
        else if (buffer == "s") {
            std::filesystem::path path = std::filesystem::current_path();
            auto files = fileutils::getFilesFromFolder(path);
            client.sendFiles(files);
        }
    }
};