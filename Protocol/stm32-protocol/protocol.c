//
// Created by wzq on 23/02/2017.
//
#include <string.h>
#include <stdlib.h>
#include <printf.h>
#include "protocol.h"
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
int CRC32(uint8_t *buf, uint8_t size) {
    unsigned int i, crc;
    crc = 0xFFFFFFFF;
    for (i = 0; i < size; i++)
        crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFF;
}

void GetMessage(uint8_t data) {
    uart_buffer_1[uart_buffer_index_1++] = data;
    if (data == 0xff) {
        DispatchMessage();
    }
}

int DeserializeInt(uint8_t *data) {
    int result = 0;
    result = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
    return result;
}

int ExtractRaw(uint8_t *original,uint8_t *output) {

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
void print(uint8_t * data,int len){
    for (int i = 0; i < len; ++i) {
        printf("%02x ",data[i]);
    }
}
void DispatchMessage() {
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
    print(temp_ptr,raw_data_size_from_protocol);

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


    message__free_unpacked(message_temp, NULL);

}