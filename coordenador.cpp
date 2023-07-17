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
#include <unordered_map>

std::string IP = "127.0.0.1";
int PORT = 10000;
Queue<int> requests;
std::unordered_map<int, int> requestsTracker;
Spinlock lock; // used to control the access to the shared variable requests

// Used to write in the log file.
void logger(Message msg, std::string t){
    std::ofstream log;
    log.open("log.txt", std::ios::app);
    if (log.is_open()) {
        log << t << " - " << msg.text(1) << std::endl; 
        log.close();
    }
}

// Used to keep track of how many requests the controller received from each process.
void tracker(int id){
    auto proc = requestsTracker.find(id);
    if (proc != requestsTracker.end()) {
        proc->second++;
    } else {
        requestsTracker[id] = 1;
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
                    tracker(port); // registers that request was granted
                    lock.release();
                    UDPSender sender(IP, port); // creates sender object
                    Message grant(Message::Type::GRANT, port); // creates GRANT message
                    sender.send(grant.text()); // sends GRANT message
                    logger(grant, getTime()); // logs
                }
                lock.release();
                lock.acquire();
                requests.push(port);
                lock.release();
            }

            if (type == Message::Type::RELEASE){ // process that owns CR's access releases it
                lock.acquire();
                if (port == requests.peek()){ // check if the msg came from the process that has the CR
                    logger(msg, getTime()); // logs 
                    requests.pop(); // removes first element of the queue
                    if (!requests.isEmpty()){ // there are processes waiting to use the CR.
                        int newPort = requests.peek(); // first element will receive access
                        tracker(newPort); // registers that the request was granted 
                        lock.release();
                        UDPSender sender(IP, newPort); // creates sender object
                        Message grant(Message::Type::GRANT, newPort); // creates GRANT message
                        sender.send(grant.text()); // sends GRANT message
                        logger(grant, getTime()); // logs 
                    }
                }
                lock.release();
            }
        }
    }
    
}

void* monitor(void* arg){
    std::cout << "1: fila de processos;\n2: lista de atendimentos;\n3: finalizar o programa." << "\n";
    std::cout << "aguardando..." << "\n";
    while(1){
        int option = 0;
        std::cin >> option;

        if (option == 1){
            lock.acquire();
            std::string procList = requests.getQueue(); 
            std::cout << "Fila de processos para acesso à região crítica: " << procList << ".\n";
            lock.release();
        }
        else if (option == 2){
            lock.acquire();
            std::cout << "ID" << "    | "  << " Requests atendidos" <<  "\n"; 
            for (const auto& entry : requestsTracker) {
                std::cout << entry.first << " | " << entry.second << "\n";
            }
            lock.release();
        }
        else if (option == 3){
            pid_t pid = getpid();
            kill(pid, SIGINT);
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