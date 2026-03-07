
#include <iostream>
#include <vector>

#include "group_commands.hpp"
#include "task_commands.hpp"
#include "server.hpp"
#include "client.hpp"

int main(int argc, char* argv[]) {

    // MAIN TEMPORARIO: Trocar por uma logica de chamamento de comandos mais robusta
    if (argc < 2)
        return 1;

    std::string command = argv[1];

    if (command == "create")
    {
        std::string arg = argv[2];
        if (arg == "group")
        {
            group::create_group_config_file();
        }
        else if(arg == "task")
        {
            task::create_task_config_file();
        }
    }
    else if (command == "update")
    {
        std::clog << "[LOG] Updating data...\n";
        std::string ip = "127.0.0.1";
        ConnectionProtocol cp = ConnectionProtocol::IPV4;
        if (argc > 2)
            std::string ip = argv[2];
        // TODO: validate ip and identify the protocol for connection;
        IClient groupClient = IClient(cp);
        
        groupClient.update(ip);
        std::clog << "[LOG] Data successfully recieved from server.\n";

    }
    else if (command == "run")
    {
        
        IServer groupServer = IServer();
        groupServer.run();
    }
    else if (command == "send")
    {
        std::string ip = "127.0.0.1";
        if (argc > 2)
            ip = argv[2];

        ConnectionProtocol cp = ipRegex(ip);
        if (cp == ConnectionProtocol::UNDEF){
            std::cerr << "[ERROR] Invalid IP\n";
            return EXIT_FAILURE;
        }
        // TODO: validate ip and identify the protocol for connection;
        IClient groupClient = IClient(cp);
        std::clog << "[LOG] Validating files...\n";
        std::unique_ptr<Task> task = task::read_task_config();
        if (task::validate_required_files(*task)) {
            std::clog << "[LOG] Sending files...\n";
        } else {
            std::clog << "[LOG] Missing required files.\n";
            std::cerr << "[ERROR] Send process aborted.\n";
            return 0;
        }
        groupClient.run(ip);
    }
    else
    {
        std::cout << "command not found.\n";
    }
    
    return 0;
}
