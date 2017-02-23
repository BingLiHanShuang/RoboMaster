//
// Created by wzq on 23/02/2017.
//
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "protocol.pb.h"
#include "protocol.h"
#include "Serial.h"
#include <cstdlib>
#include <queue>
using namespace cv;
using namespace std;
extern Serial *serial;
/*
 * FE //start
 * 00 00 00 00//packet size
 * 00 00 00 00//CRC32
 * ...........//payload
 * FF //end
 */
namespace PROTOCOL{
    void print(uint8_t * data,int len){
        for (int i = 0; i < len; ++i) {
            printf("%02x ",data[i]);
        }
    }
    uint32_t CRC32(uint8_t *buf, uint32_t size)
    {
        uint32_t i, crc;
        crc = 0xFFFFFFFF;
        for (i = 0; i < size; i++)
            crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
        return crc^0xFFFFFFFF;
    }

    void send(Message_MessageType type, void *payload, uint32_t size) {
        Message *message = new Message();
        message->set_data(payload, size);
        message->set_messagetype(type);

        int buffer_size = message->ByteSize();
        void *buffer = malloc(buffer_size);

        message->SerializeToArray(buffer,buffer_size);
//        uint8_t * buffer_ptr= (uint8_t *) buffer;
//
//        Message * test=new Message();
//        test->ParseFromArray(buffer_ptr,buffer_size);

//        print(buffer_ptr,buffer_size);



        presendtoserial(buffer,buffer_size);
        delete message;
        free(buffer);

    }

    void send_result(Point2f *pts, double angle, string data, double width, double height) {
        ScanResult *scanResult = new ScanResult();
        PosPoint *point;
        for (int i = 0; i < 4; i++) {
            point = scanResult->add_position();
            point->set_x(pts[i].x);
            point->set_y(pts[i].y);
        }
        scanResult->set_angle(angle);
        scanResult->set_result(data);

        PosPoint *picture_size = scanResult->mutable_picrutesize();

        picture_size->set_x(width);
        picture_size->set_y(height);


        int buffer_size = scanResult->ByteSize();
        void *buffer = malloc(buffer_size);
        scanResult->SerializeToArray(buffer, buffer_size);
        send(Message_MessageType_ScanResult, buffer, buffer_size);

        scanResult->Clear();
        delete scanResult;
        free(buffer);

    }
    void enque_safe(queue<uint8_t > & res,uint8_t data){
        if(data==0xff){
            res.push(0xfe);
            res.push(0xfd);
            return;
        }
        if(data==0xfe){
            res.push(0xfe);
            res.push(0xfc);
            return;
        }
        res.push(data);
        return;
    }
    void enque_int(queue<uint8_t > & res,uint32_t data){
        for (int i = 0; i < 4; ++i) {
            enque_safe(res,data&0xff);
            data>>=8;
        }
        return;
    }
    void presendtoserial(void *payload, uint32_t size) {
        queue<uint8_t> result_temp;

        uint32_t crc=CRC32((uint8_t *) payload, size);

        result_temp.push(0xfe);
        enque_int(result_temp,size);
        enque_int(result_temp,crc);

        uint8_t *payload_ptr=(uint8_t*)payload;
        for (int i = 0; i < size; ++i) {
            enque_safe(result_temp,payload_ptr[i]);
        }
        result_temp.push(0xff);

        int result_len=result_temp.size();
        uint8_t *result=new uint8_t[result_len];
        //int index=0;
        for(int i=0;i<result_len;i++){result[i]=result_temp.front();result_temp.pop();}

        sendtoserial(result,result_len);

    }
    void sendtoserial(void *payload, uint32_t size){
        writetofile(payload,size);
        //serial->send((char *) payload, size);
    }
    void writetofile(void *payload, uint32_t size){
        FILE *fp;
        fp=fopen("/Users/wzq/RoboMaster/QRCode/test/test1.dat", "w");
        for (int i = 0; i < size; ++i) {
            uint8_t temp=((uint8_t*)payload)[i];
            fputc(temp,fp);
        }
        fclose(fp);

    }
}
