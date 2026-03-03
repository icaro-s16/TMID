#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <vector>

#include "utils.hpp"

struct Task {
    std::string name = "";
    std::string description = "";
    std::vector<std::string> required_files = {};

    /* _n: task's name */
    void set_name(const std::string& _n) {
        name = _n;
    }

    /* _d: description */
    void set_description(const std::string& _d) {
        description = _d;
    }

    /* _r: required files */
    void set_required_files(const std::string& _r) {
        std::stringstream ss(_r);
        std::string item;

        while (std::getline(ss, item, ',')) {
            if (item.empty()) continue;
            item = strutils::trim(item);
            required_files.push_back(item);
        }
    }
};

#endif