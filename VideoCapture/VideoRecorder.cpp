//
// Created by parallels on 3/25/17.
//

#include "VideoRecorder.h"
VideoRecorder videoRecorder;
VideoRecorder::VideoRecorder(){}
VideoRecorder::VideoRecorder(int deviceid) {
    VideoRecorder::OpenCamera(deviceid);
}
VideoRecorder::VideoRecorder(char * devicefile){
    VideoRecorder::OpenCamera(devicefile);
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
void VideoRecorder::OpenCamera(char * devicefile){
    width = 640 ;
    height = 480 ;
    fps   = 30 ;
//    filename = "/dev/video0";
//    avifilename = "test.avi";

    frame_width=width;
    frame_height=height;
    int format = V4L2_PIX_FMT_MJPEG;
    int ret;
    int grabmethod = 1;

    file = fopen(devicefile, "wb");
    if(file == NULL)
    {
        printf("Unable to open file for raw frame capturing\n ");
        exit(1);
    }

    //v4l2 init
    vd = (struct vdIn *) calloc(1, sizeof(struct vdIn));
    if(init_videoIn(vd, (char *) devicefile, width, height,fps,format,grabmethod,NULL) < 0)
    {
        exit(1);
    }

    if (video_enable(vd))
    {
        exit(1);
    }
    camera_status=1;

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
void VideoRecorder::CreateVideo1(char *path)
{
    Release1();
    vd->avifile = AVI_open_output_file(path);
    if (vd->avifile == NULL )
    {
        printf("Error opening avifile %s\n",vd->avifilename);
        exit(1);
    }

    AVI_set_video(vd->avifile, vd->width, vd->height, fps, "MJPG");
    printf("recording to %s\n",path);
    frame_count=0;
    status=0;
    video_status=1;

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
void VideoRecorder::SaveOneFrame1(){
    int ret;

    memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
    vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(vd->fd, VIDIOC_DQBUF, &vd->buf);
    if (ret < 0)
    {
        printf("Unable to dequeue buffer");
        exit(1);
    }

    memcpy(vd->tmpbuffer, vd->mem[vd->buf.index],vd->buf.bytesused);

    AVI_write_frame(vd->avifile,(char *) vd->tmpbuffer,vd->buf.bytesused, vd->framecount);

    vd->framecount++;

    ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
    if (ret < 0)
    {
        printf("Unable to requeue buffer");
        exit(1);
    }

}
void VideoRecorder::Recording(){
    while (status==1){
        assert(video.isOpened());
        SaveOneFrame();
    }
    if(status==2)
        video.release();
}
void VideoRecorder::Release1() {
    if(video_status==0)
        return;

    fclose(file);
    video_status==0;
}
void VideoRecorder::Recording1(){
    while (status==1){
        assert(camera_status==1);
        assert(video_status==1);
        SaveOneFrame1();
    }
    if(status==2)
        Release1();
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