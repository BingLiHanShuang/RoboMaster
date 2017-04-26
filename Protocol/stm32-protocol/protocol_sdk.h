
#ifndef STM32_PROTOCOL_SDK_H
#define STM32_PROTOCOL_SDK_H

#include <stdint.h>
#include "DataType.h"
#ifdef __cplusplus
extern "C" {
#endif
extern ScanData scanData;                  //无人机
extern PadPassData padPassData;             //大幅
extern VideoRecordStatusData videoRecordStatusData;  //luxiang
extern UltraSonicData ultraSonicData;//超声测距回调


void serial_send(void *payload, uint32_t size);
void serial_receive(uint8_t data);
void callback_ScanResult();
void callback_PadPass();
void callback_VideoRecordStatus();
void callback_UltraSonic();
void VideoRecord_Start();//开始录像
void VideoRecord_Stop();//停止录像
void VideoRecord_Status();//完成录像
#endif //STM32_PROTOCOL_SDK_H
#ifdef __cplusplus
}
#endif