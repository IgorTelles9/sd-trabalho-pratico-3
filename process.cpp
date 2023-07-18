#include <iostream>
#include <string>
#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include "udpReceiver.h"
#include "udpSender.h"
#include "message.h"
#include "getTime.h"
#include <fstream>

std::string IP = "127.0.0.1";

int main(int argc, char* argv[]) {
    int r,k,id,port;
    int controllerPort = 10000;
    std::istringstream(argv[1]) >> r; 
    std::istringstream(argv[2]) >> k; 
    std::istringstream(argv[3]) >> id; 
    port = controllerPort + id;
    UDPSender sender(IP, controllerPort);
    UDPReceiver receiver(port);

    for(int i = 0; i < r; i++){
        Message request(Message::Type::REQUEST, port);
        std::string msg;
        sender.send(request.text());

        if(receiver.receive(msg)){
            Message received(msg);
            if (received.getType() == Message::Type::GRANT){
                std::ofstream resultado;
                 resultado.open("resultado.txt", std::ios::app);

                 if (resultado.is_open()) {
                    resultado << getTime() << " - " << port << std::endl; 
                    resultado.close();
                }
                sleep(k);
                Message release(Message::Type::RELEASE, port);
                sender.send(release.text());
            }
        }
    }
    return 0;
}