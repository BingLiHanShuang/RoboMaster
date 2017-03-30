//
// Created by parallels on 3/25/17.
//
#include "protocol.h"
#include "UDPClient.h"
extern "C" void sendtoserial(uint8_t *payload, size_t size){
    UDPClient udpClient;
    udpClient.send(payload,size,6000);

};
