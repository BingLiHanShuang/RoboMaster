//
// Created by wzq on 23/02/2017.
//
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "protocol.pb.h"
#include "protocol.h"
#include <cstdlib>
#include <queue>
using namespace cv;
using namespace std;
static void init_crc_table(void)
{
    unsigned int c;
    unsigned int i, j;

    for (i = 0; i < 256; i++)
    {
        c = (unsigned int)i;

        for (j = 0; j < 8; j++)
        {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }

        crc_table[i] = c;
    }
}

void prototol_send(Message_MessageType type, void *payload, int size) {
    Message *message = new Message();
    message->set_data(payload, size);
    message->set_messagetype(type);

    int buffer_size = message->ByteSize();
    void *buffer = malloc(buffer_size);

    protocol_sendtoserial(buffer,buffer_size);

}

void protocol_send_result(Point2f *pts, double angle, string data, double width, double height) {
    ScanResult *scanResult = new ScanResult();
    PosPoint *point;
    for (int i = 0; i < 4; i++) {
        point = scanResult->add_position();
        point->set_x(pts[i].x);
        point->set_y(pts[i].y);
    }
    scanResult->set_angle(angle);
    scanResult->set_result(data);
    PosPoint *picture_size = (PosPoint *) (&scanResult->picrutesize());
    picture_size->set_x(width);
    picture_size->set_y(height);


    int buffer_size = scanResult->ByteSize();
    void *buffer = malloc(buffer_size);
    scanResult->SerializeToArray(buffer, buffer_size);
    prototol_send(Message_MessageType_ScanResult, buffer, buffer_size);

    scanResult->Clear();
    delete scanResult;
    free(buffer);

}

void protocol_sendtoserial(void *payload, int size) {
    queue<>
//    char head[3] ={0xff,size|0xff,(size<<8)|0xff};

}