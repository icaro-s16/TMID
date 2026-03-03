#include <iostream>
#include <vector>

#include "group_commands.hpp"
#include "task_commands.hpp"
#include "server.hpp"

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
        /* fetch data from server... */
    }
    else if (command == "run")
    {
        /* create and host a group server... */
        Server groupServer = Server();
        groupServer.run();
    }
    else if (command == "send")
    {
        std::clog << "[LOG] Validating files...\n";
        std::unique_ptr<Task> task = task::read_task_config();
        if (task::validate_required_files(*task)) {
            std::clog << "[LOG] Sending \n";
        } else {
            std::clog << "[LOG] Missing required files.\n";
            std::cerr << "[ERROR] Send process aborted.\n";
        }
    }
    else
    {
        std::cout << "command not found.\n";
    }
    
    return 0;
}