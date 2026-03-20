#include "parser.cpp"

void foo(const std::string& a, HeaderContent& b){}

void handleFileCount(const std::string& stValue, HeaderContent& headerContent){
    
}



std::map<const std::string, void(*)(const std::string&, HeaderContent&)> createParserRouter(){
    std::map<const std::string, void(*)(const std::string&, HeaderContent&)> map;
    map.insert({"File count", foo});
    map.insert({"Sizes", foo});
    map.insert({"Names", foo});
    map.insert({"Task alias", foo});
    map.insert({"Group alias", foo});
    map.insert({"Auth", foo});
    return map;
}