//
// Created by wzq on 23/02/2017.
//

#include "protocol.h"
#include "Serial.h"
#include <cstdlib>
#include <queue>
using namespace std;
/*
 * FE //start
 * 00 00 00 00//packet size
 * 00 00 00 00//CRC32
 * ...........//payload
 * FF //end
 */
namespace PROTOCOL{
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
        //serial->send((uint8_t *) payload, size);
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
