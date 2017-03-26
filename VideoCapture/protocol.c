//
// Created by parallels on 3/25/17.
//

#include "protocol.h"
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
