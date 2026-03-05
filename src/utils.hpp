#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

#define WHITESPACE " \n\r\t\f\v"

namespace strutils {
    std::string trim(const std::string& s); // Declaration only
}

std::vector<std::string> splitText(std::string text, char delimeter){
    std::vector<std::string> tokens;
    std::string token;
    token.clear();
    for(char c: text){
        if(c != delimeter)
            token += c;
        else if (token != ""){
            tokens.push_back(token);
            token.clear();
        }
    }
    if (token != "")
        tokens.push_back(token);
    return tokens;
}

#endif