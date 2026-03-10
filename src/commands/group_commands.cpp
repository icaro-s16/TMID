#include "commands/group_commands.hpp"
    
void group::create_group_config_file()
{
    if (std::filesystem::exists(GROUP_CONFIG_FILE_NAME)) {
        std::cerr << GROUP_CONFIG_FILE_NAME << " exists. Aborting file creation" <<std::endl;
        return;
    }

    std::ofstream config_file(GROUP_CONFIG_FILE_NAME);
    if (config_file.is_open()) {
        std::clog << "[LOG] Group created at " << std::filesystem::current_path() << std::endl;
    } else {
        throw std::runtime_error("Failed to create group config file");
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
    std::clog << "[LOG] Attempting to open config file: " << GROUP_CONFIG_FILE_NAME << std::endl;
    
    std::ifstream config_file(GROUP_CONFIG_FILE_NAME);
    if (!config_file.is_open()) {
        std::cerr << "[ERROR] Group config file is not present or could not be opened: " << GROUP_CONFIG_FILE_NAME << std::endl;
        throw std::runtime_error("Could not open group config file");
    }

    auto read_group = std::make_unique<Group>();
    std::string line;
    int line_count = 0;

    while (std::getline(config_file, line)) {
        line_count++;
        
        // Basic sanitization
        if (line.empty() || line[0] == '#') continue;

        size_t delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos) {
            std::clog << "[WARN] Line " << line_count << ": Missing '=' delimiter. Skipping." << std::endl;
            continue;
        }

        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);

        if (key == "group_name") {
            std::clog << "[LOG] Setting group name to: " << value << std::endl;
            read_group->set_name(value); 
        } 
        else if (key == "participants") {
            std::clog << "[LOG] Loading participants list..." << std::endl;
            read_group->set_participants_from_string(value);
        } else {
            std::cerr << "[WARN] Key '" << key << "' at line " << line_count << " not recognized. Skipping." << std::endl;
        }
    }

    std::clog << "[LOG] Successfully finished parsing " << GROUP_CONFIG_FILE_NAME << std::endl;
    return read_group;
}