#include "protocol.h"

#include <stdio.h>

int main() {
    uart_buffer_index_1=0;
    FILE *fp;
    fp=fopen("/Users/wzq/RoboMaster/QRCode/test/test1.dat", "r");
    //int size=ftell(fp);
    for(int i=0;i<90;i++){
        uint8_t a=fgetc(fp);
        //printf("%02x ",a);
        GetMessage(a);
    }
    //fgetc(fp);
    return 0;
}