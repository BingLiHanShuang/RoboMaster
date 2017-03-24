//
// Created by wzq on 23/02/2017.
//

#include "protocol.h"
#include "Serial.h"
#include <cstdlib>
#include <queue>
#include <stdint.h>
using namespace std;
uint8_t uart_buffer_1[512];
size_t uart_buffer_index_1;
/*
 * FE //start
 * 00 00 00 00//packet size
 * 00 00 00 00//CRC32
 * ...........//payload
 * FF //end
 */
namespace PROTOCOL{
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
    }
    void DispatchMessage() {
        uint8_t raw_data[256];
        memset(raw_data, 0, sizeof(raw_data));

        int raw_data_size_real = ExtractRaw(uart_buffer_1 + 1, raw_data);//to ommit package start 0xff

        memset(uart_buffer_1, 0, sizeof(uart_buffer_1));
        uart_buffer_index_1 = 0;

        int raw_data_size_from_protocol = DeserializeInt(raw_data);
        int raw_data_crc32_from_protocol = DeserializeInt(raw_data + 4);
        if (raw_data_size_from_protocol != raw_data_size_real - 8)return;//Drop this package due to package loss
        int raw_data_crc32_real = CRC32(raw_data + 8, raw_data_size_from_protocol);
        if (raw_data_crc32_real != raw_data_crc32_from_protocol)return;//Drop this package due to CRC32 check failure


        uint8_t *temp_ptr = raw_data + 8;

        //print(temp_ptr, raw_data_size_from_protocol);
    }

    void print(uint8_t * data,int len){
        printf("{");
        for (int i = 0; i < len; ++i) {
            printf("0x%02x,",data[i]);
        }
        printf("}\n");
    }
    uint32_t CRC32(uint8_t *buf, uint32_t size)
    {
        uint32_t i, crc;
        crc = 0xFFFFFFFF;
        for (i = 0; i < size; i++)
            crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
        return crc^0xFFFFFFFF;
    }


    void enque_safe(queue<uint8_t > & res,uint8_t data){
        if(data==0xff){
            res.push(0xfe);
            res.push(0xfd);
            return;
        }
        if(data==0xfe){
            res.push(0xfe);
            res.push(0xfc);
            return;
        }
        res.push(data);
        return;
    }
    void enque_int(queue<uint8_t > & res,uint32_t data){
        for (int i = 0; i < 4; ++i) {
            enque_safe(res,data&0xff);
            data>>=8;
        }
        return;
    }
    void presendtoserial(void *payload, uint32_t size) {
        queue<uint8_t> result_temp;

        uint32_t crc=CRC32((uint8_t *) payload, size);

        result_temp.push(0xfe);
        enque_int(result_temp,size);
        enque_int(result_temp,crc);

        uint8_t *payload_ptr=(uint8_t*)payload;
        for (int i = 0; i < size; ++i) {
            enque_safe(result_temp,payload_ptr[i]);
        }
        result_temp.push(0xff);

        int result_len=result_temp.size();
        uint8_t *result=new uint8_t[result_len];
        //int index=0;
        for(int i=0;i<result_len;i++){result[i]=result_temp.front();result_temp.pop();}

        sendtoserial(result,result_len);

    }
    void sendtoserial(void *payload, uint32_t size){
        //writetofile(payload,size);
        print((uint8_t *) payload, size);
        serial->send((uint8_t *) payload, size);
    }
    void writetofile(void *payload, uint32_t size){
        FILE *fp;
        fp=fopen("/home/parallels/log", "w");
        for (int i = 0; i < size; ++i) {
            uint8_t temp=((uint8_t*)payload)[i];
            fputc(temp,fp);
        }
        fclose(fp);

    }
}
