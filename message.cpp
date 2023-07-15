#include <iostream>
#include <string>
#include "message.h"

Message::Message(Type messageType, int id): type(messageType), id(id){}

Message::Message(std::string text){
    id = std::stoi(text.substr(0, 5));
    type = Type(std::stoi(text.substr(6)));
}

std::string Message::text(const bool& verbose){
    std::string identifier = std::to_string(id);
    std::string req;
    if(verbose) {
        switch (type){
            case 1: 
                req = " REQUEST";
                break;
            case 2: 
                req = " GRANT";
                break;
            case 3: 
                req = " RELEASE";
                break;
        }
    } else {
       req = std::to_string(type); 
    }
    std::string text = identifier + "|" + req;
    return text;
}

int Message::getId(){
    return id;
}

Message::Type Message::getType(){
    return type;
}
