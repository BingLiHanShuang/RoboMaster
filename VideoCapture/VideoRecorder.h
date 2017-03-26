//
// Created by parallels on 3/25/17.
//

#ifndef VIDEOCAPTURE_VIDEORECORD_H
#define VIDEOCAPTURE_VIDEORECORD_H
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class VideoRecorder {
private:
    int frame_width,frame_height;
    int status=0;//0 for not recording,1 for recording,2 for recording stoped
    int frame_count=0;
    VideoCapture vcap;
    VideoWriter video;

public:
    VideoRecorder();
    VideoRecorder(int deviceid);
    void OpenCamera(int deviceid);
    void CloseCamera();
    void CreateVideo(string path);
    void ReleaseVideo();
    void SaveOneFrame();
    void Recording();
    int GetStatus();
    int SetStutus(int data);
    ~VideoRecorder();

};
void* thread_video_record(void *arg);
extern VideoRecorder videoRecorder;
#endif //VIDEOCAPTURE_VIDEORECORD_H
