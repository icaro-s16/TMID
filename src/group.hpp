#ifndef GROUP_HPP
#define GROUP_HPP

#include <map>

struct Group {
    const char* name;
    const std::map<unsigned short int, const char *> participants;

    /* _n: group's name, _p: participants */
    Group(
        const char* _n,
        const std::map<unsigned short int, const char *> _p
    ) : name(_n), participants(_p) {}
};

#endif