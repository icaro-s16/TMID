#include "utils/utils.hpp"

std::string strutils::trim(const std::string& s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    if (start == std::string::npos) {
        return ""; // The string is entirely whitespace
    }

    size_t end = s.find_last_not_of(WHITESPACE);

    return s.substr(start, end - start + 1);
}

std::vector<std::string> strutils::splitText(std::string text, char delimeter){
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

std::vector<char> fileutils::getBytesFromFile(const std::filesystem::path& path) {
    
    if (std::filesystem::directory_entry(path).is_directory())
    throw std::runtime_error("path given for reading bytes is a directory, not a file.");

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return {}; 

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        return buffer;
    }
    return {};
}

std::vector<std::filesystem::path> fileutils::getFilesFromFolder(const std::filesystem::path &st_dir_path) {
    
    std::vector<std::filesystem::path> paths;
    std::filesystem::path dir_path(st_dir_path); 
    
    for(auto entry: std::filesystem::directory_iterator(dir_path)) {
        if (entry.is_regular_file())
        paths.push_back(entry);
    }

    return paths;
}

std::vector<std::filesystem::path> fileutils::getFoldersFromFolder(const std::filesystem::path &st_dir_path) {
    
    std::vector<std::filesystem::path> paths;
    std::filesystem::path dir_path(st_dir_path); 
    
    for(auto entry: std::filesystem::directory_iterator(dir_path)) {
        if (!entry.is_directory())
        paths.push_back(entry);
    }

    return paths;
}