//
// Created by parallels on 3/10/17.
//

#include "UDPServer.h"
#include "ReceiveHandler.h"
UDPServer::UDPServer(int port) {
    ReceiveCasllback=NULL;
    FinishCasllback=NULL;
    struct epoll_event event;   // 告诉内核要监听什么事件
    epfd=epoll_create(1024);

    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind error");

    }
    event.data.fd = sockfd;     //监听套接字
    event.events = EPOLLIN; // 表示对应的文件描述符可以读
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
    if(-1 == ret){
        perror("epoll_ctl");
        exit(1);
    }
}
void UDPServer::listen() {
    while (1){
        struct epoll_event    events[_MAX_SOCKFD_COUNT];
        int num=epoll_wait(epfd,events,_MAX_SOCKFD_COUNT,-1);
        for (int i = 0; i < num; ++i) {
            int client_socket = events[i].data.fd;

            if (events[i].events & EPOLLIN){
                uint8_t *buffer=new uint8_t[1024];//每次收发的字节数小于1024字节
                memset(buffer, 0, 1024);
                int rev_size = recv(events[i].data.fd,buffer, 1024,0);
                if( rev_size > 0 )
                {
                    BufferData data;
                    data.data=buffer;
                    data.len=rev_size;
                    if(ReceiveCasllback!=NULL){
                        ReceiveCasllback(data);
                    }
                    /*
                    pthread_mutex_lock(&mtx_queue);
                    data_queue.push(data);
                    pthread_mutex_unlock(&mtx_queue);
                    */



                }
                delete buffer;
                struct epoll_event event_del;
                event_del.data.fd = client_socket;
                event_del.events = EPOLLIN | EPOLLERR | EPOLLHUP;;
                epoll_ctl(epfd, EPOLL_CTL_MOD, event_del.data.fd, &event_del);
            }
        }
        if(num>0){
            if(FinishCasllback!=NULL){
                FinishCasllback();
            }
            /*
            if(pthread_mutex_trylock(&mtx_thread)==0){//lock successfully and signal the SendQueueThread
                pthread_cond_signal(&cond_thread);
                pthread_mutex_unlock(&mtx_thread);
            }
             */

        }

    }
}
void UDPServer::setReceiveCallback(void (*callback)(BufferData &data)){
    ReceiveCasllback=callback;
}
void UDPServer::setFinishCasllback(void (*callback)() ) {
    FinishCasllback=callback;
}
void* thread_Server_UDPToSerial(void *arg){

    UDPServer udpServer(6001);
    udpServer.listen();
}