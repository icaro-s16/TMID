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

std::vector<char> fileutils::getBytesFromFile(const std::string& path) {
    // Open in binary mode and seek to the end immediately to get the size
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return {}; 

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg); // Rewind to the start

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        return buffer;
    }
    return {}; // Return empty vector if the read fails
}

std::vector<std::string> fileutils::getFilesFromFolder(std::string st_dir_path) {
    std::vector<std::string> filesPath;
    std::filesystem::path dir_path(st_dir_path); 
    for(auto entry: std::filesystem::directory_iterator(dir_path)){
        if (entry.is_regular_file())
            filesPath.push_back(entry.path().string());
    }
    return filesPath;
}
