
#include <iostream>
#include <vector>

#include "network/server.hpp"
#include "network/client.hpp"
#include "commands/commands.hpp"

int main(int argc, char* argv[]) {

    // temporary main: Will be replaced with something more sturdy
    if (argc < 2)
        return 1;

    std::string command = argv[1];

    if (command == "create")
    {
        std::string arg = argv[2];
        if      (arg == "group") commands::createGroup();
        else if (arg == "task") commands::createTask();
    }
    else if (command == "sync") commands::syncToGroup();
    else if (command == "update")
    {
        std::string ip = "127.0.0.1";
        if (argc > 2)
            std::string ip = argv[2];
        Client groupClient = Client(ip);
        
        groupClient.updateLocalFiles();
        std::clog << "[LOG] Data successfully recieved from server.\n";

    }
    else if (command == "run")
    {
        commands::hostGroup();
        /* Connection protocol is hardcoded for now. Change it latter. */
        // Server groupServer = Server(ConnectionProtocol::IPV4);
        // groupServer.run();
    }
    else if (command == "send")
    {
        std::string ip = "127.0.0.1";
        if (argc > 2)
            ip = argv[2];

        Client groupClient = Client(ip);
        groupClient.sendTaskFiles();
    }
    else
    {
        std::cout << "command not found.\n";
    }
}
