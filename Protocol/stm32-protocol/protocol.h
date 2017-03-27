//
// Created by wzq on 23/02/2017.
//

#ifndef STM32_PROTOCOL_H
#define STM32_PROTOCOL_H

#include "protocol.pb-c.h"
#include <stdint.h>
#include <stdio.h>
#include "DataType.h"
#include "protocol_config.h"
#include "protocol_sdk.h"
#ifdef __cplusplus
extern "C" {
#endif

void DispatchMessage();
static const unsigned int crc32tab[];

int CRC32(uint8_t *buf, uint8_t size);
extern uint8_t uart_buffer_1[256];
extern int uart_buffer_index_1;
void GetMessage(uint8_t data);
void SerializeInt(uint8_t *data,int val);
int ExtractRaw(uint8_t *original,uint8_t *output);
int DeserializeInt(uint8_t *data);
void SaveScanResult(ScanResult *scanResult);
void SavePadPass(PadPass * mpadPass);
void SaveVideoRecordStatus(VideoRecord * mVideoRecord);
void VideoRecord_Start();
void MessageSend(enum _Message__MessageType type,ProtobufCBinaryData data);
void presendtoserial(void *payload, uint32_t size);
void enque_safe(BufferData* res,uint8_t data);
void enque_int(BufferData* res,uint32_t data);
void enque_uint8(BufferData* res,uint8_t data);
void serial_send(void *payload, uint32_t size);
void print(uint8_t * data,int len);
void VideoRecord_Stop();
void VideoRecord_Status();
extern ScanData scanData;
extern PadPassData padPassData;
extern VideoRecordStatusData videoRecordStatusData;
#ifdef __cplusplus
}
#endif

#endif //STM32_PROTOCOL_H
