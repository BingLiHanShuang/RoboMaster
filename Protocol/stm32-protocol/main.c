#include "protocol.h"

#include <stdio.h>

int main() {
    uart_buffer_index_1=0;
    uint8_t a[]={0xfe,0x16,0x00,0x00,0x00,0xba,0xb7,0x0f,0x84,0x08,0x02,0x12,0x12,0x0a,0x09,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x12,0x05,0x31,0x32,0x33,0x34,0x35,0xff}

    ;
    int len= sizeof(a)/sizeof(uint8_t);
    for(int i=0;i<len;i++){
        //uint8_t a=fgetc(fp);
        //printf("%02x ",a);
        GetMessage(a[i]);
    }
//    FILE *fp;
//    fp=fopen("/Users/wzq/RoboMaster/QRCode/test/test1.dat", "r");
//    //int size=ftell(fp);
//    for(int i=0;i<90;i++){
//        uint8_t a=fgetc(fp);
//        //printf("%02x ",a);
//        GetMessage(a);
//    }
    //fgetc(fp);
    return 0;
}