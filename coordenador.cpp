#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "udpReceiver.h"
#include "udpSender.h"

void controller(UDPReceiver receiver) {
    
}


int main() {
    // Create a receiver object
    UDPReceiver receiver(12345);

    // Create a sender object
    UDPSender sender("127.0.0.1", 12345);

    // Send a message
    sender.send("0000000000");

    // Receive a message
    std::string message;
    if (receiver.receive(message)) {
        std::cout << "Received message: " << message << std::endl;
    }

    return 0;
}