//
// Created by wzq on 23/02/2017.
//
#include <string.h>
#include <stdlib.h>
#include <printf.h>
#include "protocol.h"
#include "DataType.h"
#include <stdint.h>
#include <stdlib.h>
#include "protocol.pb-c.h"

/*
 * FE //start
 * 00 00 00 00//packet size
 * 00 00 00 00//CRC32
 * ...........//payload
 * FF //end
 */
struct ScanData scanData;
struct PadPass padPass;

void enque_safe(BufferData* res,uint8_t data){
    if(data==0xff){
        res->data[res->len++]=0xfe;
        res->data[res->len++]=0xfd;
        return;
    }
    if(data==0xfe){
        res->data[res->len++]=0xfe;
        res->data[res->len++]=0xfc;
        return;
    }
    res->data[res->len++]=data;
    return;
}
void enque_int(BufferData* res,uint32_t data){
    for (int i = 0; i < 4; ++i) {
    enque_safe(res,data&0xff);
    data>>=8;
    }
return;
}
void enque_uint8(BufferData* res,uint8_t data){
    res->data[res->len++]=data;
    return;
}
void presendtoserial(void *payload, uint32_t size) {
    BufferData result_temp;
    result_temp.len=0;
    result_temp.data=(uint8_t*)malloc(sizeof(uint8_t)*512);//malloc 1
    uint32_t crc=CRC32((uint8_t *) payload, size);

    enque_uint8(&result_temp,0xfe);
    enque_int(&result_temp,size);
    enque_int(&result_temp,crc);

    uint8_t *payload_ptr=(uint8_t*)payload;
    for (int i = 0; i < size; ++i) {
        enque_safe(&result_temp,payload_ptr[i]);
    }
    enque_uint8(&result_temp,0xff);

    int result_len=result_temp.len;
    uint8_t *result=(uint8_t*)malloc(sizeof(uint8_t)*result_len);//malloc 2
    //int index=0;
    for(int i=0;i<result_len;i++){result[i]=result_temp.data[i];}


    sendtoserial(result,result_len);
    free(result_temp.data);//free 1
    free(result);//free 2

}
void sendtoserial(void *payload, uint32_t size){//abstruct for serial
    //writetofile(payload,size);
    print((uint8_t *) payload, size);
    //serial->send((uint8_t *) payload, size);
}
int CRC32(uint8_t *buf, uint8_t size) {//calculate CRC32 code in case of data loss
    unsigned int i, crc;
    crc = 0xFFFFFFFF;
    for (i = 0; i < size; i++)
        crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFF;
}

void GetMessage(uint8_t data) {//receive the data from serial stream
    uart_buffer_1[uart_buffer_index_1++] = data;
    if (data == 0xff) {
        DispatchMessage();
    }
}

int DeserializeInt(uint8_t *data) {//deserialize int from memory
    int result = 0;
    result = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
    return result;
}

void SerializeInt(uint8_t *data,int val) {

    for (int i = 0; i < 4; ++i) {
        data[i]|=0b11111111&(val>>(i*8));
    }




    //data[0]=
    //    int result = 0;
    //    result = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;

    //return result;
}
int ExtractRaw(uint8_t *original,uint8_t *output) {//process escape character to convert data frame to original data

    int i = 0, j = 0;
    while (original[i] != 0xff) {
        if (original[i] == 0xfe) {//escape char
            i++;
            switch (original[i]) {
                case 0xfd:
                    output[j] = 0xff;
                    break;
                case 0xfc:
                    output[j] = 0xfe;
                    break;
            }
            j++;
            i++;
            continue;
        }
        output[j] = original[i];
        i++;
        j++;
    }
    return j;
    //if(size!=(j))return;

}

void SaveScanResult(ScanResult *mscanResult) {
    for (int i = 0; i < mscanResult->n_position; ++i) {
        scanData.vertex[i].x=mscanResult->position[i]->x;
        scanData.vertex[i].y=mscanResult->position[i]->y;
    }
    scanData.angle=mscanResult->angle;
    scanData.flag_read=0;
    scanData.size.x=mscanResult->picrutesize->x;
    scanData.size.y=mscanResult->picrutesize->y;
    strcpy(scanData.text,mscanResult->result);
};
void SavePadPass(PadPass * mpadPass){
    assert(mpadPass->has_password==1&&mpadPass->password.len==5);//密码长度为5位
    assert(mpadPass->has_pad==1&&mpadPass->pad.len==9);//九宫格中的数字为9个
    memcpy(padPass.Pad,mpadPass->pad.data,mpadPass->pad.len* sizeof(uint8_t));//将九宫格拷贝至全局变量
    memcpy(padPass.Pass,mpadPass->password.data,mpadPass->password.len* sizeof(uint8_t));//将密码拷贝至全局变量
    padPass.flag_read=1;



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
}
void DispatchMessage() {//process the received buffer
    uint8_t raw_data[256];
    memset(raw_data, 0, sizeof(raw_data));

    int raw_data_size_real = ExtractRaw(uart_buffer_1 + 1, raw_data);//to ommit package start 0xff

    memset(uart_buffer_1, 0, sizeof(uart_buffer_1));
    uart_buffer_index_1 = 0;

    int raw_data_size_from_protocol = DeserializeInt(raw_data);
    int raw_data_crc32_from_protocol = DeserializeInt(raw_data + 4);
    if (raw_data_size_from_protocol != raw_data_size_real-8)return;//Drop this package due to package loss
    int raw_data_crc32_real = CRC32(raw_data + 8, raw_data_size_from_protocol);
    if (raw_data_crc32_real != raw_data_crc32_from_protocol)return;//Drop this package due to CRC32 check failure


    uint8_t *temp_ptr=raw_data+8;
    //print(temp_ptr,raw_data_size_from_protocol);

    Message *message_temp = message__unpack(NULL, (size_t)raw_data_size_from_protocol, temp_ptr);

    if (message_temp->messagetype == MESSAGE__MESSAGE_TYPE__ScanResult) {//读取扫描二维码
        ScanResult *scanResult_temp = scan_result__unpack(NULL, (message_temp->data.len), (message_temp->data.data));
        SaveScanResult(scanResult_temp);
        scan_result__free_unpacked(scanResult_temp,NULL);
    }
    if(message_temp->messagetype==MESSAGE__MESSAGE_TYPE__PadPass){//读取九宫格和密码信息
        PadPass * padPass_temp =pad_pass__unpack(NULL, (message_temp->data.len), (message_temp->data.data));
        SavePadPass(padPass_temp);
        pad_pass__free_unpacked(padPass_temp,NULL);

    }
    if(message_temp->messagetype==MESSAGE__MESSAGE_TYPE__VideoRecord){
        VideoRecord* videoRecord_temp=video_record__unpack(NULL, (message_temp->data.len), (message_temp->data.data));
        video_record__free_unpacked(videoRecord_temp,NULL);

    }


    message__free_unpacked(message_temp, NULL);

}
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

void VideoRecord_Start(){
    char *devicename_temp=config_devicename;
    char *deviceid_temp=config_deviceid;

    VideoRecord videoRecord=VIDEO_RECORD__INIT;
    videoRecord.has_control=1;
    videoRecord.control=VIDEO_RECORD__CONTROL_TYPE__Start;
    videoRecord.devicename=devicename_temp;
    videoRecord.deviceid=deviceid_temp;


    ProtobufCBinaryData data;
    uint8_t buffer[100];
    data.len=video_record__pack(&videoRecord,buffer);
    data.data=buffer;

    MessageSend(MESSAGE__MESSAGE_TYPE__VideoRecord,data);

}
void VideoRecord_Stop(){


    VideoRecord videoRecord=VIDEO_RECORD__INIT;
    videoRecord.has_control=1;
    videoRecord.control=VIDEO_RECORD__CONTROL_TYPE__Stop;


    ProtobufCBinaryData data;
    uint8_t buffer[100];
    data.len=video_record__pack(&videoRecord,buffer);
    data.data=buffer;

    MessageSend(MESSAGE__MESSAGE_TYPE__VideoRecord,data);

}
void VideoRecord_Status(VideoRecord__StatusType type){


    VideoRecord videoRecord=VIDEO_RECORD__INIT;
    videoRecord.has_control=1;
    videoRecord.control=VIDEO_RECORD__CONTROL_TYPE__Status;
    videoRecord.has_status=1;
    videoRecord.status=type;

    ProtobufCBinaryData data;
    uint8_t buffer[100];
    data.len=video_record__pack(&videoRecord,buffer);
    data.data=buffer;

    MessageSend(MESSAGE__MESSAGE_TYPE__VideoRecord,data);

}