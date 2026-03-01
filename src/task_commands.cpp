#include "task_commands.hpp"

void task::create_task_config_file()
{
    if (std::filesystem::exists(TASK_CONFIG_FILE_NAME)) {
        std::cout << TASK_CONFIG_FILE_NAME << " exists. Aborting file creation" <<std::endl;
        return;
    }

    std::ofstream config_file(TASK_CONFIG_FILE_NAME);
    if (config_file.is_open()) {
        std::cout << "task created at " << std::filesystem::current_path() << std::endl;
    } else {
        throw std::runtime_error("failed to create task config file");
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
    std::clog << "[LOG] Attempting to open config file: " << TASK_CONFIG_FILE_NAME << std::endl;

    std::ifstream config_file(TASK_CONFIG_FILE_NAME);
    if (!config_file.is_open()) {
        std::cerr << "[ERROR] Task config file is not present or could not be opened: " << TASK_CONFIG_FILE_NAME << std::endl;
        throw std::runtime_error("Could not open task config file");
    }

    // Initialize the object so we can actually call methods on it
    auto read_task = std::make_unique<Task>();
    
    std::string line;
    int line_num = 0;

    // Using std::getline to ensure we don't stop at spaces in descriptions
    while (std::getline(config_file, line)) {
        line_num++;

        if (line.empty() || line[0] == '#') continue;

        size_t delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos) {
            std::clog << "[WARN] Line " << line_num << ": Missing '=' delimiter. Skipping." << std::endl;
            continue;
        }

        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);

        if (key == "task_name") {
            std::clog << "[LOG] Setting task name to: " << value << std::endl;
            read_task->set_name(value); 
        } 
        else if (key == "description") {
            std::clog << "[LOG] Description found (Length: " << value.length() << ")" << std::endl;
            read_task->set_description(value);
        } else {
            std::cerr << "[WARN] Key '" << key << "' at line " << line_num << " not recognized. Skipping." << std::endl;
        }
    }

    std::clog << "[LOG] Finished loading task configuration." << std::endl;
    return read_task;
}