#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#define WHITESPACE " \n\r\t\f\v"

namespace strutils {

    std::string trim(const std::string& s) {

        size_t start = s.find_first_not_of(WHITESPACE);
        if (start == std::string::npos) {
            return ""; // The string is entirely whitespace
        }

        size_t end = s.find_last_not_of(WHITESPACE);

        return s.substr(start, end - start + 1);
    }
}

#endif