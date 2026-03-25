#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <vector>

#include "utils/utils.hpp"

struct Task {
    std::string fileName = "";
    std::string description = "";
    std::vector<std::string> required_files = {};

    Task(std::string n, std::string d, std::vector<std::string> rf = {}): fileName(n), description(d), required_files(rf) {}

    bool operator< (const Task &other) const {
        return fileName < other.fileName;
    }

};

#endif