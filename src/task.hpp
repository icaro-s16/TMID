#ifndef TASK_HPP
#define TASK_HPP

#include <string>

struct Task {
    std::string name;
    std::string description;

    /* _n: task's name */
    void set_name(const std::string& _n) {
        name = _n;
    }

    /* _d: description */
    void set_description(const std::string& _d) {
        description = _d;
    }
};

#endif