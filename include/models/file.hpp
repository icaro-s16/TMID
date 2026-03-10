#ifndef _FILE_HPP
#define _FILE_HPP

#include <string>
#include <vector>
#include "utils/utils.hpp"

class File {
private:
    std::filesystem::path m_path;
    std::vector<char> m_data;
    /* Will open the file located at m_path and read all binary data from it, storing the read values at m_data */
    void readBinaryData() {
        std::ifstream file(m_path, std::ios::binary);
        if (!file) throw std::runtime_error("Failed to open file to read binary data.");
        
        size_t fileLength = std::filesystem::file_size(m_path);
        char bytes[fileLength];
        file.read(bytes, fileLength);
        
        m_data = std::vector<char>(bytes, bytes + fileLength);
    }

public:

    File(const std::filesystem::path& _p): m_path(_p) {
        if (std::filesystem::is_regular_file(_p)) readBinaryData();
        else m_data = {};
    }

    File(File&& other) noexcept 
        : m_path(std::move(other.m_path)),
          m_data(std::move(other.m_data)) {}

    File& operator=(File&& other) noexcept {
        if (this != &other) {
            m_path = std::move(other.m_path);
            m_data = std::move(other.m_data);
        }
        return *this;
    }

    File(const File&) = default;
    File& operator=(const File&) = default;

    void writeToFile(const std::filesystem::path& path) {
        std::ofstream file(path, std::ios::binary);
        if (!file) throw std::runtime_error("Failed to open file to write binary data.");
        file.write(m_data.data(), m_data.size());
    }

    const std::vector<char>& getData() { return m_data; }
    const std::filesystem::path& getPath() { return m_path; }
};

#endif

