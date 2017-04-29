//
// Created by parallels on 3/25/17.
//

#ifndef VIDEOCAPTURE_VIDEORECORD_H
#define VIDEOCAPTURE_VIDEORECORD_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <pthread.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <X11/Xlib.h>
#include "v4l2uvc.h"
#include "avilib.h"
#include "Structure.h"
using namespace cv;
using namespace std;
class VideoRecorder {
private:
    int status=0;//0 for not recording,1 for recording,2 for recording stoped
    FILE *file = NULL;
    struct vdIn *vd;
    unsigned char *tmpbuffer;
    struct shared_package *shared_package_ptr;
    int camera_status=0;
    int video_status=0;

public:
    VideoRecorder();
    void CreateVideo1(char *path);
    void SaveOneFrame1();

    void Recording1();
    void Release1();
    int GetStatus();
    int SetStutus(int data);

};
void* thread_video_record(void *arg);
extern VideoRecorder videoRecorder;
#endif //VIDEOCAPTURE_VIDEORECORD_H
