
#ifndef STM32_PROTOCOL_SDK_H
#define STM32_PROTOCOL_SDK_H

#include <stdint.h>
void serial_send(void *payload, uint32_t size);
void serial_receive(uint8_t data);
void callback_ScanResult();
void callback_PadPass();
void callback_VideoRecordStatus();
void VideoRecord_Start();//开始录像
void VideoRecord_Stop();//停止录像
void VideoRecord_Status();//完成录像
#endif //STM32_PROTOCOL_SDK_H
