//
// Created by wzq on 23/02/2017.
//
#include <string.h>
#include "protocol.h"
#include "protocol.pb-c.h"

/*
 * FE //start
 * 00 00 00 00//packet size
 * 00 00 00 00//CRC32
 * ...........//payload
 * FF //end
 */

int CRC32(const unsigned char *buf, unsigned int size) {
    unsigned int i, crc;
    crc = 0xFFFFFFFF;
    for (i = 0; i < size; i++)
        crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFF;
}

void GetMessage(char data) {
    uart_buffer_1[uart_buffer_index_1++] = data;
    if (data == 0xff) {
        DispatchMessage();
    }
}

int DeserializeInt(char *data) {
    int result = 0;
    result = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
    return result;
}

int ExtractRaw(char *original, char *output) {

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

void SaveScanResult(ScanResult *scanResult) {
    for (int i = 0; i < scanResult->n_position; ++i) {
        scanData.vertex[i].x=scanResult->position[i]->x;
        scanData.vertex[i].y=scanResult->position[i]->y;
    }
    scanData.angle=scanResult->angle;
    scanData.flag_read=0;
    scanData.size.x=scanResult->picrutesize->x;
    scanData.size.y=scanResult->picrutesize->y;
    strcpy(scanData.text,scanResult->result);


};

void DispatchMessage() {
    char raw_data[256];
    memset(raw_data, 0, sizeof(raw_data));

    int raw_data_size_real = ExtractRaw(uart_buffer_1 + 1, raw_data);//to ommit package start 0xff

    memset(uart_buffer_1, 0, sizeof(uart_buffer_1));
    uart_buffer_index_1 = 0;

    int raw_data_size_from_protocol = DeserializeInt(raw_data);
    int raw_data_crc32_from_protocol = DeserializeInt(raw_data + 4);
    if (raw_data_size_from_protocol != raw_data_size_real)return;//Drop this package due to package loss
    int raw_data_crc32_real = CRC32(raw_data + 9, raw_data_size_real);
    if (raw_data_crc32_real != raw_data_crc32_from_protocol)return;//Drop this package due to CRC32 check failure

    Message *message_temp = message__unpack(NULL, raw_data_size_real, raw_data + 9);
    if (message_temp->messagetype == MESSAGE__MESSAGE_TYPE__ScanResult) {
        ScanResult *scanResult = scan_result__unpack(NULL, (message_temp->data.len), (message_temp->data.data));
        SaveScanResult(scanResult);
        scan_result__free_unpacked(scanResult,NULL);
    }


    message__free_unpacked(message_temp, NULL);

}