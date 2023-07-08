#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> 

class UDPReceiver {
private:
    int receiverSocket;
    struct sockaddr_in receiverAddr;

public:
    UDPReceiver(int receiverPort);
    ~UDPReceiver();

    bool receive(std::string& message);
};
