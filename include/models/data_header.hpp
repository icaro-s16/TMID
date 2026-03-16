#ifndef _DATA_HEADER_HPP
#define _DATA_HEADER_HPP

struct HeaderFile{
    std::string name;
    uint32_t bytesLen;
    uint32_t checkSum;

    HeaderFile(): name(""), bytesLen(0), checkSum(0){};
    
    static HeaderFile parseHeaderFile(char* header) {
        HeaderFile header_values;
        char* line = strtok(header, "\r\n");
        while (line != nullptr){
            char* delimeter = strchr(line, ':');
            if(delimeter != nullptr){
                delimeter[0] = '\0';
                char* label = line;
                char* value = delimeter + 1;
                if (strcmp(label, "Name") == 0)
                    header_values.name = value;
                else if(strcmp(label, "FileSize") == 0)
                    header_values.bytesLen = std::stoul(value);
            }
            line = strtok(NULL, "\r\n");
        }
        return header_values;
    }

    static int parseHeaderAmount(char* header){
        char* delimeter = strchr(header, ':');
        if(delimeter != nullptr){
            delimeter[0] = '\0';
            char* label = header;
            char* value = delimeter + 1;
            if (strcmp(label, "Amount") == 0){
                return std::stoul(value);
            }
        }
        return 0;
    }
};

#endif