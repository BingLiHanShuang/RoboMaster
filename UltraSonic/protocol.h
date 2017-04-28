//
// Created by parallels on 3/25/17.
//

#ifndef VIDEOCAPTURE_PROTOCOL_H
#define VIDEOCAPTURE_PROTOCOL_H

#include "protocol.pb-c.h"

#ifdef __cplusplus

extern "C" {
    
#endif
    
    void MessageSend(enum _Message__MessageType type,ProtobufCBinaryData data);
    void presendtoserial(uint8_t *payload, size_t size);
    void VideoRecord_Status(size_t status);
    void TarmacSend(double x, double y);
    void UltraSonicSend(double height);
    void PadPassSend(uint8_t *digit_handwrite,uint8_t *digit_led);
    void print(uint8_t * data,int len);
#ifdef __cplusplus
    
}

#endif

#endif //VIDEOCAPTURE_PROTOCOL_H
