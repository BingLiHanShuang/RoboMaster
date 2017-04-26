//
// Created by parallels on 3/25/17.
//

#include "protocol.h"
#include <stdio.h>
//#include "UDPClient.h"
void sendtoserial(uint8_t *payload, size_t size);
void MessageSend(enum _Message__MessageType type,ProtobufCBinaryData data){
    Message message=MESSAGE__INIT;
    message.has_data=1;
    message.has_messagetype=1;


    message.messagetype=type;
    message.data=data;

    uint8_t buffer[100];
    size_t buffer_len=message__pack(&message,buffer);
    presendtoserial(buffer,buffer_len);
}
void TarmacSend(double x, double y){
    ScanResult scanResult=SCAN_RESULT__INIT;
    PosPoint center=POS_POINT__INIT;
//    posPoint.x=x;
//    posPoint.y=y;

    PosPoint picrutesize=POS_POINT__INIT;


    scanResult.picrutesize=&picrutesize;
    scanResult.picrutesize->has_x=1;
    scanResult.picrutesize->has_y=1;

    scanResult.picrutesize->x=640;
    scanResult.picrutesize->y=480;

    scanResult.center=&center;

    scanResult.center->has_x=1;
    scanResult.center->has_y=1;
    scanResult.center->x=x;
    scanResult.center->y=y;




    ProtobufCBinaryData data;
    uint8_t buffer[100];
    data.len=scan_result__pack(&scanResult,buffer);
    data.data=buffer;

    MessageSend(MESSAGE__MESSAGE_TYPE__ScanResult,data);

}
void UltraSonicSend(double height){
    UltraSonic ultraSonic=ULTRA_SONIC__INIT;
    ultraSonic.height=height;
    ProtobufCBinaryData data;
    uint8_t buffer[100];
    data.len=ultra_sonic__pack(&ultraSonic,buffer);
    data.data=buffer;
    MessageSend(MESSAGE__MESSAGE_TYPE__UltraSonic,data);

}
void print(uint8_t * data,int len){//for debug
#define general
#ifdef hex
    for (int i = 0; i < len; ++i) {
        printf("0x%02x,",data[i]);
    }
    printf("\n");

#endif
#ifdef general
    for (int i = 0; i < len; ++i) {
        printf("%02x ",data[i]);
    }
    printf("\n");

#endif
#ifdef hex_nospace
    for (int i = 0; i < len; ++i) {
        printf("%02x",data[i]);
    }
    printf("\n");

#endif
}
void presendtoserial(uint8_t *payload, size_t size){

    sendtoserial(payload,size);
}

void VideoRecord_Status(size_t status){


    VideoRecord videoRecord=VIDEO_RECORD__INIT;
    videoRecord.has_control=1;
    videoRecord.control=VIDEO_RECORD__CONTROL_TYPE__Status;
    videoRecord.has_status=1;
    switch (status){
        case 0:
            videoRecord.status=VIDEO_RECORD__STATUS_TYPE__Off;
            break;
        case 1:
            videoRecord.status=VIDEO_RECORD__STATUS_TYPE__Recording;
            break;
        case 2:
            videoRecord.status=VIDEO_RECORD__STATUS_TYPE__Off;
            break;
    }
    //videoRecord.status=type;

    ProtobufCBinaryData data;
    uint8_t buffer[100];
    data.len=video_record__pack(&videoRecord,buffer);
    data.data=buffer;

    MessageSend(MESSAGE__MESSAGE_TYPE__VideoRecord,data);

}
