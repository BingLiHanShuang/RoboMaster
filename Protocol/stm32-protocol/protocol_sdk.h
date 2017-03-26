
#ifndef STM32_PROTOCOL_SDK_H
#define STM32_PROTOCOL_SDK_H

#include <stdint.h>
void serial_send(void *payload, uint32_t size);
void callback_ScanResult();
void callback_PadPass();
void callback_VideoRecordStatus();

#endif //STM32_PROTOCOL_SDK_H
