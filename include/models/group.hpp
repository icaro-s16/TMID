#ifndef GROUP_HPP
#define GROUP_HPP

#include <map>
#include <string>
#include <sstream>
#include <string>
#include <algorithm>

#include "utils/utils.hpp"

struct Group {
    std::string m_alias;
    std::string m_name;
    std::string m_description;

public:
    Group(
        const std::string &alias,
        const std::string &name,
        const std::string &description
    ) : m_alias(alias),
        m_name(name),
        m_description(description) {}
}; 

//struct Group {
//    std::string name;
//    std::map<std::string, std::string> participants;
//
//    /* _n: group's name */
//    void set_name(std::string _n) {
//        name = _n;
//    }
//
//    /* _p: participants in the format "id:name,id:name,id:name" */
//    void set_participants_from_string(const std::string& _p) {
//        
//        std::stringstream ss(_p);
//        std::string item;
//
//        while (std::getline(ss, item, ',')) {
//            if (item.empty()) continue;
//
//            size_t colon_pos = item.find(':');
//            if (colon_pos != std::string::npos) {
//                std::string id = item.substr(0, colon_pos);
//                std::string name = item.substr(colon_pos + 1);
//
//                id = strutils::trim(id);
//                name = strutils::trim(name);
//                
//                participants[id] = name;
//            }
//        }
//    }
//};

#endif