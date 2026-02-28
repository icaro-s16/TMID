#ifndef GROUP_CMD_HPP
#define GROUP_CMD_HPP

#include <fstream>
#include <filesystem>
#include <iostream>

#include "group.hpp"

#define GROUP_CONFIG_FILE_NAME ".group_config"

namespace group {
    void create_group_config_file();
    void set_config_file(std::ofstream& config_file);
    std::unique_ptr<Group> read_group_config();
}
#endif
