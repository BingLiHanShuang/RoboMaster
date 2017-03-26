#include "protocol.h"

#include <stdio.h>

int main() {
    VideoRecord_Start();
    VideoRecord_Stop();
    VideoRecord_Status();
    uart_buffer_index_1=0;
    uint8_t a[]={0xfe,0x0b,0x00,0x00,0x00,0xc2,0xd9,0x1d,0x47,0x08,0x03,0x12,0x07,0x08,0x01,0x1a,0x03,0x57,0x5a,0x51,0xff}
    ;
    int len= sizeof(a)/sizeof(uint8_t);
    for(int i=0;i<len;i++){
        //uint8_t a=fgetc(fp);
        //printf("%02x ",a);
        //GetMessage(a[i]);

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