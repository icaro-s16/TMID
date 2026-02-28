#include "group_commands.hpp"

    
void group::create_group_config_file()
{
    if (std::filesystem::exists(GROUP_CONFIG_FILE_NAME)) {
        std::cout << GROUP_CONFIG_FILE_NAME << " exists. Aborting file creation" <<std::endl;
        return;
    }

    std::ofstream config_file(GROUP_CONFIG_FILE_NAME);
    if (config_file.is_open()) {
        std::cout << "file created." << std::endl;
    } else {
        throw std::runtime_error("failed to create group config file");
    }

    set_config_file(config_file);
}

void group::set_config_file(std::ofstream& config_file)
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

std::unique_ptr<Group> group::read_group_config()
{
    std::ifstream config_file(GROUP_CONFIG_FILE_NAME);
    if (!config_file.is_open()) {
        std::cerr << "group config file is not present or could not be opened.\n";
        throw std::runtime_error("Could not open group config file");
    }

    std::unique_ptr<Group> read_group = std::unique_ptr<Group>();

    std::string line;
    while (config_file >> line) {
        if (line.empty() || line[0] == '#') continue;

        size_t delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos) continue;

        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);
        if (key == "group_name") {
            read_group->set_name(value); 
        } 
        else if (key == "participants") {
            read_group->set_participants_from_string(value);
        } else {
            std::cerr << "key '" << key << "' not recognized. Skipping line\n" << std::endl;
        }
    }

    return read_group;
}
