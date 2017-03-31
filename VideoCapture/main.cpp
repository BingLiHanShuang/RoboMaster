#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <queue>
#include <pthread.h>
#include "ReceiveHandler.h"
using namespace std;
using namespace cv;
queue<Mat> image;
extern pthread_mutex_t thread_video_record_mutex;
extern pthread_cond_t thread_video_record_cond;
pthread_cond_t thread_video_capture_cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t thread_video_capture_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_image=PTHREAD_MUTEX_INITIALIZER;
void* thread_video_capture(void *arg){
    while(1) {
        pthread_mutex_lock(&thread_video_capture_mutex);
        while (videoRecorder.GetStatus() == 1) {
            while(!image.empty()){
                pthread_mutex_lock(&mutex_image);
                Mat temp=image.front();
                image.pop();
                pthread_mutex_unlock(&mutex_image);
                videoRecorder.SaveMat(temp);

            }
        }

        pthread_cond_wait(&thread_video_capture_cond, &thread_video_capture_mutex);
        pthread_mutex_unlock(&thread_video_capture_mutex);
    }



}
int main() {


    pthread_t ntid_message_handler;
    pthread_t ntid_server_udp;
    pthread_t ntid_thread_video_record;


    pthread_create(&ntid_message_handler,NULL,thread_server_udp,NULL);
    pthread_create(&ntid_server_udp,NULL,thread_message_handler,NULL);
    pthread_create(&ntid_thread_video_record,NULL,thread_video_capture,NULL);



    videoRecorder.OpenCamera(0);

    while(1) {
        pthread_mutex_lock(&thread_video_record_mutex);
        while (videoRecorder.GetStatus() == 1){
            Mat temp=videoRecorder.ReadMat();
            pthread_mutex_lock(&mutex_image);
            image.push(temp);
            pthread_mutex_unlock(&mutex_image);

        }
        pthread_cond_wait(&thread_video_record_cond, &thread_video_record_mutex);
        pthread_mutex_unlock(&thread_video_record_mutex);

    }
    pthread_join(ntid_message_handler, NULL);
    pthread_join(ntid_server_udp, NULL);



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