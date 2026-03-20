#include <string>
#include <sstream>
#include <vector>

template<typename T>
void formatVector(const std::vector<T>& vector, std::stringstream& stream);

struct Auth{
    u_int8_t pin;
};

struct MetaData{
    std::vector<std::string> names;
    std::vector<std::string> taskAlias;
    std::vector<std::string> groupAlias;
};

struct BlobProperties{
    size_t fileCount;
    std::vector<size_t> sizes;
};

struct HeaderContent{
    BlobProperties blProperties;
    MetaData mtData;
    Auth auth;

    std::vector<char> getHeader(){
        std::stringstream stream;
        
        stream << "File count:" << blProperties.fileCount << "\n";
        
        stream << "Sizes:";
        formatVector<size_t>(blProperties.sizes, stream);
        
        stream << "Names:";
        formatVector<std::string>(mtData.names, stream);

        stream << "Task alias:";
        formatVector<std::string>(mtData.names, stream);
        
        stream << "Group alias:";
        formatVector<std::string>(mtData.groupAlias, stream);

        stream << "Auth:" << auth.pin << "\n\n";

    }
};


template<typename T>
void formatVector(const std::vector<T>& vector, std::stringstream& stream){
    for (const T index: vector){
        stream << index << ";";
    }
    stream << "\n";
}