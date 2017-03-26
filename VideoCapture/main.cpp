#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "ReceiveHandler.h"
using namespace std;
using namespace cv;

int main() {


    pthread_t ntid_message_handler;
    pthread_t ntid_server_udp;

    pthread_create(&ntid_message_handler,NULL,thread_server_udp,NULL);
    pthread_create(&ntid_server_udp,NULL,thread_message_handler,NULL);

    pthread_join (ntid_message_handler, NULL);
    pthread_join (ntid_server_udp, NULL);
    /*

    VideoCapture vcap(1);
    if(!vcap.isOpened()){
        cout<<"Error opening camera"<<endl;
        return -1;
    }

    int frame_width=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
    int count=0;
    VideoWriter video("/home/parallels/RoboMaster/VideoCapture/out.avi",CV_FOURCC('M','P','4','2'),10,Size(frame_width,frame_height),true);
    while (1){
        Mat frame;
        vcap>>frame;
        video.write(frame);
        imshow("frame",frame);
        waitKey(33);
        count++;
        if(count>100){
            video.release();
            return 0;
        }

    }
     */

}