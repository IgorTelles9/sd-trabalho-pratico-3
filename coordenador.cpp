#include <iostream>
#include <string>
#include <sstream>
#include <pthread.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "udpReceiver.h"
#include "udpSender.h"
#include "queue.h"
#include "message.h"
#include "Spinlock.h"
#include "signal.h"
#include "getTime.h"
#include <fstream>

std::string IP = "127.0.0.1";
int PORT = 10000;
Queue<int> requests;
Spinlock lock; // used to control the access to the shared variable requests
void logger(Message msg, std::string t){
    std::ofstream log;
    log.open("log.txt", std::ios::app);
    if (log.is_open()) {
        log << t << " - " << msg.text(1) << std::endl; 
        log.close();
    }
}

/**
 * Controls the access of a critic region between multiple process using UDP
*/
void* controller(void* arg) {
    UDPReceiver receiver(PORT);
    while(1){
        std::string msg;  // received message

        if(receiver.receive(msg)){
            Message received(msg);
            int type = received.getType(); // message's received type
            int port = received.getId(); // id = port of the process sending the message 

            if (type == Message::Type::REQUEST){ // request to access the CR
                logger(msg, getTime());
                lock.acquire();
                if(requests.isEmpty()){
                    lock.release();
                    UDPSender sender(IP, port); // creates sender object
                    Message grant(Message::Type::GRANT, port); // creates GRANT message
                    sender.send(grant.text()); // sends GRANT message
                    logger(grant, getTime());
                }
                lock.release();
                lock.acquire();
                requests.push(port);
                lock.release();
            }

            if (type == Message::Type::RELEASE){ // process that owns CR's access releases it
                lock.acquire();
                if (port == requests.peek()){ // check if the msg came from the process that has the CR
                    logger(msg, getTime());
                    requests.pop(); // removes first element of the queue
                    if (!requests.isEmpty()){
                       int newPort = requests.peek();
                       lock.release();
                        UDPSender sender(IP, newPort); // creates sender object
                        Message grant(Message::Type::GRANT, newPort); // creates GRANT message
                        sender.send(grant.text()); // sends GRANT message
                        logger(grant, getTime());
                    }
                }
                lock.release();
            }
        }
    }
    
}

void* monitor(void* arg){
    while(1){
        int option = 0;
        std::cin >> option;

        if(option == 1){
            lock.acquire();
            int proc = requests.peek();
            lock.release();
            std::cout << "Identificador/Porta do processo acessando a região crítica: " << proc << ".\n";
        }
        else if (option == 2){
            lock.acquire();
            std::string procList = requests.getQueue(); 
            lock.release();
            std::cout << "Fila de processos para acesso à região crítica: " << procList << ".\n";
        }
        else if (option == 3){
            pid_t pid = getpid();
            kill(pid, SIGKILL);
        }
    }

}

int main() {
    std::remove("resultado.txt");
    std::remove("log.txt");
    pthread_t threadController; 
    pthread_t threadMonitor; 

    pthread_create(&threadController, NULL, controller, NULL);
    pthread_create(&threadMonitor, NULL, monitor, NULL);
    pthread_join(threadController, NULL);
    pthread_join(threadMonitor, NULL);

    return 0;
}