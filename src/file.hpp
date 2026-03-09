#include <string>
#include <vector>
#include "utils.hpp"

enum FileType {FOLDER, FILE};

class File {
private:
    std::filesystem::path m_path;
    FileType m_elementType;
    std::vector<char> m_data;

    void readBinaryData() {
        std::ifstream file(m_path, std::ios::binary);
        if (!file) throw std::runtime_error("Failed to open file to read binary data.");
        
        size_t fileLength = std::filesystem::file_size(m_path);
        char bytes[fileLength];
        file.read(bytes, fileLength);
        
        m_data = std::vector<char>(bytes, bytes + fileLength);
    }


public:
    File(const std::filesystem::path& _p, FileType _e = FILE): m_path(_p), m_elementType(_e) {
        if (m_elementType == FILE) {
            readBinaryData();
        } else {
            m_data = {};
        }
    }

    void writeToFile(const std::filesystem::path& path) {
        std::ofstream file(path, std::ios::binary);
        if (!file) throw std::runtime_error("Failed to open file to write binary data.");
        file.write(m_data.data(), m_data.size());
    }

    const FileType& getElementType() { return m_elementType; }
    const std::vector<char>& getData() { return m_data; }
    const std::filesystem::path& getPath() { return m_path; }
};

