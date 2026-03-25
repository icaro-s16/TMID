#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

#define WHITESPACE " \n\r\t\f\v"

namespace strutils {
    std::string trim(const std::string& s);
    std::vector<std::string> splitText(std::string text, char delimeter);
}

namespace fileutils {
    std::vector<char> getBytesFromFile(const std::filesystem::path& path);
    std::vector<std::filesystem::path> getFilesFromFolder(const std::filesystem::path &st_dir_path);
    std::vector<std::filesystem::path> getFoldersFromFolder(const std::filesystem::path &st_dir_path);

}


#endif