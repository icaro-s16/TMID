#include "group_commands.hpp"

namespace group {
    
    void create_group_config_file()
    {
        if (std::filesystem::exists(GROUP_CONFIG_FILE_NAME)) {
            std::cout << GROUP_CONFIG_FILE_NAME << " exists. Aborting file creation" <<std::endl;
            return;
        }

        std::ofstream config_file(GROUP_CONFIG_FILE_NAME);
        if (config_file.is_open()) {
            std::cout << "file created." << std::endl;
        } else {
            std::cerr << "failed to create file." << std::endl;
            throw;
        }

        set_config_file(config_file);
    }

    void set_config_file(std::ofstream& config_file)
    {
        config_file << "# group's config file.\n";
        config_file << "# lines starting with '#' will be ignored by the system.\n";
        config_file << "\n";
        config_file << "# group name: usually the folder name\n";
        config_file << "group_name=\n";
        config_file << "\n";
        config_file << "# participants: all the participants for the group, with their unique id\n";
        config_file << "participants=\n";

        config_file.close();
    }

    std::unique_ptr<Group> read_group_config()
    {
        return std::unique_ptr<Group>();
    }
}