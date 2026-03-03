#ifndef TASK_CMD_HPP
#define TASK_CMD_HPP

#include <fstream>
#include <filesystem>
#include <iostream>
#include <regex>

#include "task.hpp"

#define TASK_CONFIG_FILE_NAME ".task_config"

namespace task {
    void create_task_config_file();
    void set_config_file(std::ofstream& config_file);
    std::unique_ptr<Task> read_task_config();
    bool validate_required_files(const Task&);
}
#endif