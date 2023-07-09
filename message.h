#include <iostream>

class Message{
    public:
        enum Type {
            REQUEST = 1,
            GRANT = 2,
            RELEASE = 3
        };

    private:
        Type type;
        int id; 
    public:
        Message(Type type, int id);
        Message(std::string text);
        std::string text();
        int getId();
        int getType();
};