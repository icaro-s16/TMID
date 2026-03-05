#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

#define WHITESPACE " \n\r\t\f\v"

namespace strutils {
    std::string trim(const std::string& s);
}

std::vector<std::string> splitText(std::string text, char delimeter);

#endif