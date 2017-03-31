//
// Created by parallels on 3/25/17.
//

#include "VideoRecorder.h"
VideoRecorder videoRecorder;
VideoRecorder::VideoRecorder(){}
VideoRecorder::VideoRecorder(int deviceid) {
    vcap.open(deviceid);
    if(!vcap.isOpened()){
        cout<<"Error opening camera id"<<deviceid<<endl;
        exit(0);
    }
    frame_width=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    frame_height=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
}
void VideoRecorder::OpenCamera(int deviceid){

    vcap.open(deviceid);
    if(!vcap.isOpened()){
        cout<<"Error opening camera id"<<deviceid<<endl;
        //exit(0);
    }
    frame_width=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    frame_height=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
}
void VideoRecorder::CloseCamera() {
    if(!vcap.isOpened())vcap.release();
}

void VideoRecorder::CreateVideo(string path){
    if(!vcap.isOpened()){
        cout<<"Camera is not opened in VideoRecorder.cpp CreateVideo"<<endl;
        exit(0);
    }
    ReleaseVideo();
    video.open(path,CV_FOURCC('M','P','4','2'),10,Size(frame_width,frame_height),true);
    frame_count=0;
    status=0;

}
Mat VideoRecorder::ReadMat(){
    Mat frame;
    vcap>>frame;
    return frame;
}
void VideoRecorder::SaveMat(Mat mat) {
    video.write(mat);
    frame_count++;
}
void VideoRecorder::ReleaseVideo() {
    if(video.isOpened())video.release();
}
void VideoRecorder::SaveOneFrame(){
    Mat frame;
    vcap>>frame;
    video.write(frame);
    frame_count++;
    waitKey(1);
}
void VideoRecorder::Recording(){
    while (status==1){
        assert(video.isOpened());
        SaveOneFrame();
    }
    if(status==2)
        video.release();
}
VideoRecorder::~VideoRecorder(){
    vcap.release();
}
int VideoRecorder::GetStatus() { return status;}
int VideoRecorder::SetStutus(int data) {
    status=data;
    return status;
}

pthread_mutex_t thread_video_record_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_video_record_cond=PTHREAD_COND_INITIALIZER;
void* thread_video_record(void *arg){
    /*

    videoRecorder.OpenCamera(1);

    while(1){
        pthread_mutex_lock(&thread_video_record_mutex);
        if(videoRecorder.GetStatus()==1){
            videoRecorder.Recording();
        }
        pthread_cond_wait(&thread_video_record_cond,&thread_video_record_mutex);
        pthread_mutex_unlock(&thread_video_record_mutex);


    }
    videoRecorder.Recording();

     */




}