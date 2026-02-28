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
