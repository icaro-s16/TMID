#include <iostream>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>
#include <map>
#include <vector>
#include "header.cpp"

struct Packet{
  std::vector<unsigned char> header;
  std::vector<unsigned char> body;
  
  Packet(): header(std::vector<unsigned char>()), body(std::vector<unsigned char>()){}

};


std::unique_ptr<Packet> tryCreatePacket(std::vector<unsigned char> content){
  Packet packet;  
  std::string stContent(content.begin(), content.end());
  
  for( auto i = 0; i < stContent.size() - 1; i ++ ){
   
    if (stContent[i] != '\n' && stContent[i+1] != '\n') continue;
    
    size_t offset = i + 1;

    packet.header.insert(
      packet.header.begin(), 
      stContent.data(), 
      stContent.data() + offset
    );
    
    if (stContent.size() > offset){
      packet.body.insert(
        packet.body.begin(), 
        stContent.data() + offset + 1, 
        stContent.data() + stContent.size()
      );
    }
    
    std::unique_ptr<Packet> up_packet = std::make_unique<Packet>(packet);
    return std::move(up_packet);
      
  }

  return nullptr;
}

HeaderContent parserMSG(std::vector<unsigned char> content, std::map<std::string, void(*)(const std::string&, HeaderContent&)> map){
  
  HeaderContent hdContent;
  std::stringstream stream;
  std::string token;
  
  stream.read((char*)content.data(), content.size());

  std::getline(stream, token, '\n');

  while( !stream.fail() ){

    size_t pos = token.find(':');
    
    if (pos != std::string::npos){
      std::string key;
      std::string arg;

      std::map<std::string, void(*)(const std::string&, HeaderContent&)>::iterator function;
      
      key.insert(0, token.c_str(), pos);
      arg.insert(0, token.data() + (pos + 1), token.size() - (pos + 1));

      function = map.find(key);
      
      if (function != map.end()) function->second(arg, hdContent);
    
    }


    token.clear();
    std::getline(stream, token, '\n');
  }
  

}

