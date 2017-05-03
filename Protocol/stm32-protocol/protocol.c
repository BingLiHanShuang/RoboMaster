//
// Created by wzq on 23/02/2017.
//
#include <string.h>
#include <stdlib.h>
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
#ifdef __cplusplus
extern "C" {
#endif
ScanData scanData;
PadPassData padPassData;
VideoRecordStatusData videoRecordStatusData;
UltraSonicData ultraSonicData;
uint8_t uart_buffer_1[256];
uint8_t flag_start=0;
const unsigned int crc32tab[] = {
        0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
        0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
        0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
        0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
        0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
        0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
        0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
        0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
        0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
        0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
        0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
        0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
        0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
        0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
        0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
        0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
        0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
        0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
        0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
        0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
        0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
        0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
        0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
        0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
        0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
        0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
        0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
        0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
        0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
        0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
        0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
        0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
        0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
        0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
        0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
        0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
        0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
        0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
        0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
        0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
        0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
        0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
        0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
        0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
        0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
        0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
        0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
        0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
        0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
        0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
        0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
        0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
        0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
        0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
        0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
        0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
        0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
        0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
        0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
        0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
        0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
        0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
        0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
        0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
};
int uart_buffer_index_1;
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
    print(payload,size);
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


    serial_send(result,result_len);
    free(result_temp.data);//free 1
    free(result);//free 2

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
    if(flag_start==1){
        if (data==0xfc||data==0xfd){
            flag_start=0;
        } else{
            flag_start=0;
            uart_buffer_index_1=0;
            uart_buffer_1[uart_buffer_index_1++] = 0xfe;
            uart_buffer_1[uart_buffer_index_1++]=data;
        }
    }
    if (data == 0xff) {
        DispatchMessage();
    }
    if(data==0xfe){
        flag_start=1;
    }
    if(uart_buffer_index_1>=255){
        uart_buffer_index_1=0;
    }
}

int DeserializeInt(uint8_t *data) {//deserialize int from memory
    int result = 0;
    result = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
    return result;
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
    scanData.size.x=mscanResult->picrutesize->x;
    scanData.size.y=mscanResult->picrutesize->y;
    scanData.center.x=mscanResult->center->x;
    scanData.center.y=mscanResult->center->y;
    strcpy(scanData.text,mscanResult->result);
    scanData.flag_read=0;
    callback_ScanResult();

};
void SavePadPass(PadPass * mpadPass){
//    assert(mpadPass->has_password==1&&mpadPass->password.len==5);//密码长度为5位
//    assert(mpadPass->has_pad==1&&mpadPass->pad.len==9);//九宫格中的数字为9个
    memcpy(padPassData.Pad,mpadPass->pad.data,mpadPass->pad.len* sizeof(uint8_t));//将九宫格拷贝至全局变量
    memcpy(padPassData.Pass,mpadPass->password.data,mpadPass->password.len* sizeof(uint8_t));//将密码拷贝至全局变量
    padPassData.flag_read=0;
    callback_PadPass();

}
void SaveVideoRecordStatus(VideoRecord * mVideoRecord){
    videoRecordStatusData.status=mVideoRecord->status;
    videoRecordStatusData.flag_read=0;
    callback_VideoRecordStatus();
}
void SaveUltraSonic(UltraSonic *mUltraSonic){
    ultraSonicData.height=mUltraSonic->height;
    ultraSonicData.flag_read=0;
    callback_UltraSonic();
}
void print(uint8_t * data,int len){//for debug
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
    if(message_temp->messagetype==MESSAGE__MESSAGE_TYPE__VideoRecord){//读取录像状态
        VideoRecord* videoRecord_temp=video_record__unpack(NULL, (message_temp->data.len), (message_temp->data.data));
        SaveVideoRecordStatus(videoRecord_temp);
        video_record__free_unpacked(videoRecord_temp,NULL);
    }
    if(message_temp->messagetype==MESSAGE__MESSAGE_TYPE__UltraSonic){//超声波测距
        UltraSonic * ultraSonic_temp=ultra_sonic__unpack(NULL, (message_temp->data.len), (message_temp->data.data));
        SaveUltraSonic(ultraSonic_temp);
        ultra_sonic__free_unpacked(ultraSonic_temp,NULL);
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
    print(buffer,buffer_len);
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

void VideoRecord_Status(){


    VideoRecord videoRecord=VIDEO_RECORD__INIT;
    videoRecord.has_control=1;
    videoRecord.control=VIDEO_RECORD__CONTROL_TYPE__Status;
//    videoRecord.has_status=1;
//    videoRecord.status=type;

    ProtobufCBinaryData data;
    uint8_t buffer[100];
    data.len=video_record__pack(&videoRecord,buffer);
    data.data=buffer;

    MessageSend(MESSAGE__MESSAGE_TYPE__VideoRecord,data);

}
#ifdef __cplusplus
}
#endif