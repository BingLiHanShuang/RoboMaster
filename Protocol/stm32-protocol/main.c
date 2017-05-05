#include "protocol_sdk.h"

#include <stdio.h>

int main() {
    VideoRecord_Start();
    VideoRecord_Stop();
    VideoRecord_Status();
    uint8_t a[]={0xFE,0x16,0x00,0x00,0x00,0x74,0x35,0xAD,0x66,0x08,0x02,0x12,0x12,0x0A,0x09,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x12,0x05,0x01,0x02,0x03,0x04,0x05,0xFF}
    ;
    int len= sizeof(a)/sizeof(uint8_t);
    for(int i=0;i<len;i++){
        //uint8_t a=fgetc(fp);
        //printf("%02x ",a);
        serial_receive(a[i]);

    }
//    for(int i=0;i<len;i++){
//        //uint8_t a=fgetc(fp);
//        //printf("%02x ",a);
//        serial_receive(a[i]);
//
//    }

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