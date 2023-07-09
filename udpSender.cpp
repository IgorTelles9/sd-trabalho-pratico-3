#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "udpSender.h"

UDPSender::UDPSender(const std::string& receiverIP, int receiverPort) {
    senderSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (senderSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(receiverPort);
    receiverAddr.sin_addr.s_addr = inet_addr(receiverIP.c_str());
}

UDPSender::~UDPSender() {
    close(senderSocket);
}

bool UDPSender::send(const std::string& message) {
    if (message.length() != 7) {
        std::cerr << "Invalid message length. Expected 10 characters." << std::endl;
        return false;
    }

    if (sendto(senderSocket, message.c_str(), message.length(), 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr)) < 0) {
        std::cerr << "Failed to send message." << std::endl;
        return false;
    }
    return true;
}

