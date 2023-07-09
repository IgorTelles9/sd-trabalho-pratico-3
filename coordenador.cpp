#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "udpReceiver.h"
#include "udpSender.h"
#include "queue.h"
#include "message.h"

void controller(UDPReceiver receiver) {
    Queue<std::string> requests;

    std::string msg; 
    if(receiver.receive(msg)){
        
    }
}


int main() {
    // Create a receiver object
    UDPReceiver receiver(12345);

    // Create a sender object
    UDPSender sender("127.0.0.1", 12345);

    // Send a message
    Message m(Message::REQUEST, 12345);
    sender.send(m.text());

    // Receive a message

    std::string message;
    if (receiver.receive(message)) {
        Message received(message);
        int type = received.getType();
        int id = received.getId();
        std::cout << "Received message: " << message << std::endl;
        std::cout << "Sender port: " << id << std::endl;
        std::cout << "Sender request: " << type << std::endl;
    }

    return 0;
}