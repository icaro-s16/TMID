#include "group.hpp"

class Server {
    std::unique_ptr<Group> m_group;
public:
    Server(): m_group(group::read_group_config()) {}
    void run() {
        std::string cmd;
        std::cout << "starting group...\n"
                  << "group name:" << m_group->name
                  << "\nparticipants ammount:" << m_group->participants.size()
                  << std::endl;

        while (true) {
            std::cout << "tmid group server rodando... [q -> sair]\n> ";
            std::cin >> cmd;
            
            if (cmd == "q") break;
        }
    }
};