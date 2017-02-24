/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usart.h"
#include "protocol.pb-c.h"
#include "string.h"




void delay_nms(u16 time) {
    u16 i = 0;
    while (time--) {
        i = 12000;  //????
        while (i--);
    }
}



int main(void) {
		SystemInit();
		uart1_init();




    RCC_cfg();
    while (1) {
        delay_nms(500);
    }

    return 0;


}


