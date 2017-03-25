//
// Created by parallels on 3/25/17.
//

#include "VideoRecord.h"
VideoRecord::VideoRecord(int deviceid) {
    vcap.open(deviceid);
    if(vcap.isOpened()){
        cout<<"Error opening camera id"<<deviceid<<endl;
        exit(0);
    }
    frame_width=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    frame_height=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
}
void VideoRecord::CreateVideo(string path){
    video.open(path,CV_FOURCC('H','2','6','4'),10,Size(frame_width,frame_height),true);
    frame_count=0;
    status=0;

}
void VideoRecord::SaveOneFrame(){
    Mat frame;
    vcap>>frame;
    video.write(frame);
    frame_count++;
}
void VideoRecord::Recording(){
    while (status==1){
        assert(video.isOpened());
        SaveOneFrame();
    }
    if(status==2)
        video.release();
}
VideoRecord::~VideoRecord(){
    vcap.release();
}
void* thread_video_record(void *arg){
    VideoCapture vcap(0);

    if(!vcap.isOpened()){
        cout<<"Error opening camera"<<endl;
        exit(0);
    }


}