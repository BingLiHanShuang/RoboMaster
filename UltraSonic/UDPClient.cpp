//
// Created by parallels on 3/23/17.
//

#include "UDPClient.h"
UDPClient::UDPClient() {
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_socket_fd < 0)
    {
        perror("Create Socket Failed:");
        exit(1);
    }
}
UDPClient::UDPClient(string address){
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address.c_str());
    client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_socket_fd < 0)
    {
        perror("Create Socket Failed:");
        exit(1);
    }
}
void UDPClient::send(uint8_t* data,size_t size,uint16_t port){
    server_addr.sin_port = htons(port);
    sendto(client_socket_fd, data, size,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
}
UDPClient::~UDPClient() {
    close(client_socket_fd);
}