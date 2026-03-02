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
    config_file << "\n\n";
    config_file << "# Task name: usually the folder name\n";
    config_file << "task_name=\n\n";
    config_file << "# Description: a good and descritive text for the task\n";
    config_file << "description=\n\n";
    config_file << "# Required files: the list of necessary files for the program\n";
    config_file << "required_files=\n";

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
        }
        else if (key == "required_files") {
            std::clog << "[LOG] Required files found (Length: " << value.length() << ")" << std::endl;

            read_task->set_required_files(value);
        } else {
            std::cerr << "[WARN] Key '" << key << "' at line " << line_num << " not recognized. Skipping." << std::endl;
        }
    }

    std::clog << "[LOG] Finished loading task configuration." << std::endl;
    return read_task;
}

bool task::validate_required_files(const Task& _t) {
    const std::vector<std::string>& required_patterns = _t.required_files;

    if (required_patterns.empty()) return true;
    
    std::filesystem::path current_path = std::filesystem::current_path();
    
    std::clog << "[LOG] Validating " << required_patterns.size() 
              << " required file patterns in: " << current_path << std::endl;

    std::vector<std::string> disk_filenames;
    for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
        disk_filenames.push_back(entry.path().filename().string());
    }

    for (const auto& pattern : required_patterns) {
        bool found_match = false;
        
        std::regex re(pattern, std::regex_constants::icase); // May cause crashes

        for (const auto& filename : disk_filenames) {
            if (std::regex_match(filename, re)) {
                std::clog << "[LOG] Match found: '" << filename 
                          << "' -> '" << pattern << "'" << std::endl;
                found_match = true;
                break;
            }
        }

        if (!found_match) {
            std::cerr << "[ERROR] Required file '" << pattern << "' is missing." << std::endl;
            return false;
        }
    }

    std::clog << "[LOG] All required files validated successfully." << std::endl;
    return true;
}