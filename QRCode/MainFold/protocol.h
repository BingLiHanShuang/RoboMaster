//
// Created by wzq on 23/02/2017.
//
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using  namespace cv;
#ifndef QRCODE_PROTOCOL_H
#define QRCODE_PROTOCOL_H
static unsigned int crc_table[256];
static void init_crc_table(void);
void protocol_send_result(Point2f* pts,double angle,string data);
void prototol_send(Message_MessageType type,void * payload, int size);
void protocol_send_result(Point2f* pts,double angle,string data,double width,double height);
void protocol_sendtoserial(void *payload, int size);
#endif //QRCODE_PROTOCOL_H
