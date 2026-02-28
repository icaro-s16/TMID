#include "task_commands.hpp"

void task::create_task_config_file()
{
    if (std::filesystem::exists(TASK_CONFIG_FILE_NAME)) {
        std::cout << TASK_CONFIG_FILE_NAME << " exists. Aborting file creation" <<std::endl;
        return;
    }

    std::ofstream config_file(TASK_CONFIG_FILE_NAME);
    if (config_file.is_open()) {
        std::cout << "file created." << std::endl;
    } else {
        std::cerr << "failed to create file." << std::endl;
        throw;
    }

    set_config_file(config_file);
}

void task::set_config_file(std::ofstream &config_file)
{
    config_file << "# task's config file.\n";
    config_file << "# lines starting with '#' will be ignored by the system.\n";
    config_file << "\n";
    config_file << "# task name: usually the folder name\n";
    config_file << "task_name=\n";
    config_file << "# description: a good and descritive text for the task\n";
    config_file << "description=\n";

    config_file.close();
}

std::unique_ptr<Task> task::read_task_config()
{
    std::ifstream config_file(TASK_CONFIG_FILE_NAME);
    if (!config_file.is_open()) {
        std::cerr << "task config file is not present or could not be opened.\n";
        throw std::runtime_error("Could not open task config file");
    }

    std::unique_ptr<Task> read_task = std::unique_ptr<Task>();

    std::string line;
    while (config_file >> line) {
        if (line.empty() || line[0] == '#') continue;

        size_t delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos) continue;

        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);
        if (key == "task_name") {
            read_task->set_name(value); 
        } 
        else if (key == "description") {
            read_task->set_description(value);
        } else {
            std::cerr << "key '" << key << "' not recognized. Skipping line\n" << std::endl;
        }
    }

    return read_task;
}