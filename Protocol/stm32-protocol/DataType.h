//
// Created by wzq on 26/03/2017.
//

#ifndef STM32_DATATYPE_H
#define STM32_DATATYPE_H
#include <stdlib.h>
#include <stdint.h>
typedef struct BufferData{
    uint8_t * data;
    size_t len;
} BufferData;
typedef struct Point{
    float x;
    float y;
} Point;
typedef struct ScanData{
    struct Point vertex[4];
    struct Point size;
    struct Point center;
    float angle;
    char text[50];
    uint8_t flag_read;//
} ScanData;

typedef struct  PadPassData{
    uint8_t Pad[9];//九宫格密码盘
    uint8_t Pass[5];//密码
    uint8_t flag_read;//
} PadPassData;
typedef struct  VideoRecordStatusData{
    uint8_t status;//0为从未录像,1为正在录像,2为录像已停止
    uint8_t flag_read;//

} VideoRecordStatusData;
typedef struct  UltraSonicData{
    float height;
} UltraSonicData;
#endif //STM32_DATATYPE_H
