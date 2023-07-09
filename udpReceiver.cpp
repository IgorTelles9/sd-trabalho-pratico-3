#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> 
#include "udpReceiver.h"

UDPReceiver::UDPReceiver(int receiverPort) {
    receiverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (receiverSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(receiverPort);
    receiverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(receiverSocket, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr)) < 0) {
        std::cerr << "Failed to bind socket." << std::endl;
        return;
    }
}

UDPReceiver::~UDPReceiver() {
    close(receiverSocket);
}

bool UDPReceiver::receive(std::string& message) {
    // 00000|0 = port/id (5 char) |(separator) 0(type of message)
    char buffer[8]; // Allow an extra byte for null-terminator
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);
    ssize_t bytesRead = recvfrom(receiverSocket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&senderAddr, &senderAddrLen);
    if (bytesRead < 0) {
        std::cerr << "Failed to receive message." << std::endl;
        return false;
    }
    buffer[bytesRead] = '\0'; // Null-terminate the received message
    message = buffer;
    return true;
}
