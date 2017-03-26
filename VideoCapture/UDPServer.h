//
// Created by parallels on 3/10/17.
//
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <iostream>
#include <cstdio>
#include <pthread.h>
#include "DataType.h"
#ifndef C_UDPSERVER_H
#define C_UDPSERVER_H
#define _MAX_SOCKFD_COUNT 1024

using namespace std;

class UDPServer {
private:
    void (*ReceiveCasllback)(BufferData &data);
    void (*FinishCasllback)();
    int sockfd;
    int epfd;

public:
    UDPServer(int port=6000);
    void setReceiveCallback(void (*arg1)(BufferData &data));
    void setFinishCasllback(void (*arg1)());
    void listen();
};


#endif //C_UDPSERVER_H
