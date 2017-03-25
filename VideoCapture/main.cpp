#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
using namespace std;
using namespace cv;

int main() {

    VideoCapture vcap(0);
    if(!vcap.isOpened()){
        cout<<"Error opening camera"<<endl;
        return -1;
    }

    int frame_width=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
    int count=0;
    VideoWriter video("/Users/wzq/RoboMaster/VideoCapture/out.avi",CV_FOURCC('H','2','6','4'),10,Size(frame_width,frame_height),true);
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
}