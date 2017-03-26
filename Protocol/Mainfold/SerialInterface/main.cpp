
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include "UDPServer.h"
#include "SerialSendQueue.h"
#include "SerialReceive.h"
#include "protocol.h"
#define MAXLINE 80
#define SERV_PORT 8888
using namespace std;
int main(void)
{
    serial=new Serial("/dev/ttyUSB1");
    udpClient=new UDPClient();
    pthread_t ntid_server;
    pthread_t ntid_queue;
    pthread_t ntid_receive;
    pthread_create(&ntid_queue,NULL,thread_SendQueue,NULL);
    pthread_create(&ntid_server,NULL,thread_Server,NULL);
    pthread_create(&ntid_receive,NULL,thread_Receive,NULL);

    pthread_join (ntid_server, NULL);
    pthread_join (ntid_queue, NULL);
    pthread_join (ntid_receive, NULL);

    //UDPServer *udpServer=new UDPServer(6000);
    //udpServer->listen();



    return 0;
}