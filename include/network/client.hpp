#include "network/connections.hpp"
#include "utils/regex.hpp"

class IClient {
private:
    ConnectionProtocol m_cp;
public:
    
    IClient(ConnectionProtocol _cp): m_cp(_cp) {}

    void run(std::string ip) {
        ConnectionProtocol ip_type = ipRegex(ip);
        if (ip_type == ConnectionProtocol::UNDEF){
            std::cerr << "[ERROR] Invalid IP\n";
            return;
        }

        ClientSocket clientSocket(ip_type, -1);
        Client client(clientSocket);
        client.createClient(ip);

        client.sendMsg("r");

        std::filesystem::path path = std::filesystem::current_path();
        
        client.sendFiles(fileutils::getFilesFromFolder(path));
        
    }

    void update(std::string ip) {
        ClientSocket clientSocket(m_cp);
        Client client(clientSocket);
        if(!client.createClient(ip)) return;
        client.sendMsg("s");
        client.recvFiles();
    }
};

/*
class Client {
public:
    Client(ClientSocket &_client);
    bool createClient(std::string ip);
    void sendFile(std::string path);
    void sendFiles(std::vector<std::string> paths);
    void recvFile();
    void recvFiles();
    ssize_t sendMsg(std::string msg);
    ssize_t recvMsg(std::string &buffer);
private:
    ClientSocket& client;
};

class Server {
public:
    Server(ServerSocket& _server);
    ClientSocket connectClient();
private:
    ServerSocket& server;
};

Client::Client(ClientSocket &_client):client(_client){}

bool Client::createClient(std::string ip){
    client.setServerAddress(ip.c_str());
    return client.connectToServer();
}\

void Client::sendFile(std::string path){
    connection::sendFile(path, client);
}

void Client::sendFiles(std::vector<std::string> paths){
    connection::sendFiles(paths, client);
}

void Client::recvFile(){
    connection::recvFile(client);
}

void Client::recvFiles(){
    connection::recvFiles(client);
}

ssize_t Client::sendMsg(std::string msg){
    return connection::sendMsg(msg, client);
}

ssize_t Client::recvMsg(std::string &buffer){
    return connection::recvMsg(buffer, client);
}


*/