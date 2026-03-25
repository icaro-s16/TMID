#ifndef _COMMANDS_HPP
#define _COMMANDS_HPP

#include <toml++/toml.hpp>

#include "network/server.hpp"
#include "network/client.hpp"
#include "models/group.hpp"

/* TODO: Change those constants for something more flexible in the future.
Those paths need to be relative to the user home path. Have that in mind
when using those constants. */
const std::filesystem::path MAIN_FOLDER          = "Documents/tmid/";
const std::filesystem::path GROUPS_FOLDER        = MAIN_FOLDER / "groups/";
const std::filesystem::path OWNED_GROUPS_FOLDER  = MAIN_FOLDER / "owned_groups/";

const std::filesystem::path TASKS_FOLDER         = "tasks/";
const std::filesystem::path SUBMISSIONS_FOLDER   = "submissions/";
const std::filesystem::path PARTICIPANTS_FILE    = "participants.toml";
const std::string GROUP_CONFIG_FILE              = ".tmid_group.toml";
const std::string TASK_CONFIG_FILE               = ".tmid_task.toml";


namespace files {
    /* part of initial setup */
char* getHomeFolder()           { return std::getenv("HOME"); }
void createMainFolder()         { std::filesystem::create_directories(MAIN_FOLDER); }
void createGroupsFolder()       { std::filesystem::create_directory(GROUPS_FOLDER); }
void createOwnedGroupsFolder()  { std::filesystem::create_directory(OWNED_GROUPS_FOLDER); }

void setupFolderStructure() {
    std::filesystem::current_path(getHomeFolder());
    createMainFolder();
    createGroupsFolder();
    createOwnedGroupsFolder();
}

/* When creating a single group */
std::filesystem::path createOwnedGroupFolder(const std::string &groupAlias) {
    auto path = OWNED_GROUPS_FOLDER / groupAlias;
    std::filesystem::create_directory(path);
    return path;
}

void createGroupConfigFile(const std::filesystem::path &path) {
    auto configFile = std::ofstream(path / GROUP_CONFIG_FILE);
    if (!configFile) throw std::runtime_error("Was not able to open config file.");
    configFile << toml::table {
        {"name", ""},
        {"description", ""}
    };
}

std::filesystem::path createTaskFolder(const std::string &groupAlias, const std::string &taskAlias) {
    auto path = OWNED_GROUPS_FOLDER / groupAlias / taskAlias;
    std::filesystem::create_directory(path);
    return path;
}

void createTaskConfigFile(const std::filesystem::path &path) {
    auto configFile = std::ofstream(path / TASK_CONFIG_FILE);
    if (!configFile) throw std::runtime_error("Was not able to open config file.");
    configFile << toml::table {
        {"title", ""},
        {"description", ""},
        {"due", toml::date_time {}},
        {"required_files", toml::array {}}
    };
}

Group getGroupByAlias(const std::string &groupAlias) {

    std::filesystem::current_path(files::getHomeFolder());

    
    auto path = OWNED_GROUPS_FOLDER / groupAlias;
    if (!std::filesystem::exists(path))
    throw std::runtime_error("Group of alias '" + groupAlias + "' was not found at " + path.string());

    auto configFile = toml::parse_file((path / GROUP_CONFIG_FILE).string());
    
    auto *name        = configFile["name"].as_string(),
         *description = configFile["description"].as_string();
    
    if (name == nullptr || description == nullptr)
    throw std::runtime_error("The " + GROUP_CONFIG_FILE + "does not contain necessary keys.");

    
    return Group(groupAlias, name->get(), description->get());
}

}

namespace commands {

void createGroup() {

    files::setupFolderStructure();
    
    std::string groupAlias;
    std::cout << "type groupAlias: ";
    std::cin >> groupAlias;

    auto groupFolder = files::createOwnedGroupFolder(groupAlias);
    std::clog << "[LOG] created owned group folder at " << groupFolder << std::endl;
    files::createGroupConfigFile(groupFolder);
}

void hostGroup() {

    std::string groupAlias;
    std::cout << "type groupAlias: ";
    std::cin >> groupAlias;

    auto group = files::getGroupByAlias(groupAlias);
    std::cout << "name: " << group.m_name << " | alias: " << group.m_alias << " | desc: " << group.m_description << std::endl;
    
    /* create Server and run it in a different thread */
    auto server = Server(ConnectionProtocol::IPV4);
    
    server.run(group);
}

void createTask() {
    files::setupFolderStructure();

    std::string groupAlias;
    std::cout << "type groupAlias: ";
    std::cin >> groupAlias;

    std::string taskAlias;
    std::cout << "type taskAlias: ";
    std::cin >> taskAlias;

    auto taskFolder = files::createTaskFolder(groupAlias, taskAlias);

    files::createTaskConfigFile(taskFolder);
}

void syncToGroup(const std::string &serverIp = "127.0.0.1") {
   
    Client client = Client(serverIp);
    // map<task, data>

    /* THIS LINE IS CAUSING CRASHES WHEN RUNNING tmid sync */
    std::map<Task, std::vector<char>> incomingTasks;
    client.getGroupTasks(incomingTasks);
    
    for (auto entry : incomingTasks) {
        auto task = entry.first;
        auto data = entry.second;
    }
}

}

#endif