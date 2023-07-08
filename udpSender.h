#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> 

class UDPSender {
private:
    int senderSocket;
    struct sockaddr_in receiverAddr;

public:
    UDPSender(const std::string& receiverIP, int receiverPort);
    ~UDPSender();
    bool send(const std::string& message);
};