#include "utils.hpp"

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

char* fileutils::getBytesFromFile(std::string path) {
    std::ifstream inFile(path, std::ios::binary | std::ios::ate);
    if (!inFile.is_open()){
        std::cerr << "[ERROR] Fail to open the file" << std::endl;
        return nullptr;
    }
    std::ifstream::pos_type fileLen = inFile.tellg();
    char* bytes = new char[(size_t)fileLen];
    // Back to beginning of the file
    inFile.seekg(0, std::ios::beg);
    inFile.read(bytes, fileLen); 
    inFile.close();
    return bytes;
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
