#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "udpReceiver.h"
#include "udpSender.h"
#include "queue.h"
#include "message.h"
#include "Spinlock.h"

std::string IP = "127.0.0.1";
Queue<int> requests;

void controller(UDPReceiver receiver) {
    Spinlock lock;

    while(1){
        std::string msg; 
        bool isAvailable = 1;

        lock.acquire();
        if (isAvailable && !requests.isEmpty()){
            int port = requests.peek();
            lock.release();
            UDPSender sender(IP, port);
            Message grant(Message::Type::GRANT, port);
            sender.send(grant.text());
            isAvailable = 0;
        }
        lock.release();

        if(receiver.receive(msg)){
            Message received(msg);
            int type = received.getType();
            int id = received.getId();

            if (type == Message::Type::REQUEST){
                lock.acquire();
                requests.push(id);
                lock.release();
            }

            if (type == Message::Type::REQUEST){
                lock.acquire();
                if (id == requests.peek()){
                    requests.pop();
                    isAvailable = 1;
                }
                lock.release();
            }
        }
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