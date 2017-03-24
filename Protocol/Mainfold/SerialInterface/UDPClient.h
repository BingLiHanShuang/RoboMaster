//
// Created by parallels on 3/23/17.
//

#ifndef SERIALINTERFACE_UDPCLIENT_H
#define SERIALINTERFACE_UDPCLIENT_H

#include <iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>

using namespace std;
class UDPClient {
private:
    struct sockaddr_in server_addr;
    int client_socket_fd;
public:
    UDPClient();
    UDPClient(string address);
    void send(uint8_t* data,size_t size,uint16_t port);
    ~UDPClient();


};


#endif //SERIALINTERFACE_UDPCLIENT_H
