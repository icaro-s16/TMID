#include "utils.hpp"

std::string strutils::trim(const std::string& s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    if (start == std::string::npos) {
        return ""; // The string is entirely whitespace
    }

    size_t end = s.find_last_not_of(WHITESPACE);

    return s.substr(start, end - start + 1);
}