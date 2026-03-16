
#include <iostream>
#include <vector>

#include "commands/group_commands.hpp"
#include "commands/task_commands.hpp"
#include "network/server.hpp"
#include "network/client.hpp"

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
        std::string ip = "127.0.0.1";
        if (argc > 2)
            std::string ip = argv[2];
        Client groupClient = Client(ip);
        
        groupClient.update();
        std::clog << "[LOG] Data successfully recieved from server.\n";

    }
    else if (command == "run")
    {

        /* Connection protocol is hardcoded for now. Change it latter. */
        Server groupServer = Server(ConnectionProtocol::IPV4);
        groupServer.run();
    }
    else if (command == "send")
    {
        std::string ip = "127.0.0.1";
        if (argc > 2)
            ip = argv[2];

        Client groupClient = Client(ip);
        std::clog << "[LOG] Validating files...\n";
        std::unique_ptr<Task> task = task::read_task_config();
        if (task::validate_required_files(*task)) {
            std::clog << "[LOG] Sending files...\n";
        } else {
            std::clog << "[LOG] Missing required files.\n";
            std::cerr << "[ERROR] Send process aborted.\n";
            return 0;
        }
        groupClient.run();
    }
    else
    {
        std::cout << "command not found.\n";
    }
}
