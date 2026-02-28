#ifndef TASK_HPP
#define TASK_HPP

struct Task {
    const char* name;
    const char* description;

    /* _n: task's name, _d: description */
    Task(
        const char* _n,
        const char* _d
    ) : name(_n), description(_d) {}
};

#endif