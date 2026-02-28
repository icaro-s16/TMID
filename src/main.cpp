#include <iostream>
#include <vector>

#include "group_commands.hpp"
#include "task_commands.hpp"



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
    else
    {
        std::cout << "command not found.\n";
    }
    
    return 0;
}