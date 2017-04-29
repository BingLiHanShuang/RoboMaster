//
// Created by parallels on 3/25/17.
//

#include "VideoRecorder.h"
VideoRecorder videoRecorder;
VideoRecorder::VideoRecorder(){
    shared_package_ptr=get_shared_package();
    vd = (struct vdIn *) calloc(1, sizeof(struct vdIn));

}



void VideoRecorder::CreateVideo1(char *path)
{
    int width = 640 ;
    int height = 480 ;
    int fps=30;

    Release1();
    vd->avifile = AVI_open_output_file(path);
    if (vd->avifile == NULL )
    {
        printf("Error opening avifile %s\n",vd->avifilename);
        exit(1);
    }

    AVI_set_video(vd->avifile, width, height, fps, "MJPG");
    printf("recording to %s\n",path);
    //frame_count=0;
    status=0;
    video_status=1;

}

void VideoRecorder::SaveOneFrame1(){
    int ret;

    if(getImageFromMemory()!=0)
        return;



    //memcpy(vd->tmpbuffer, image_buffer,image_buffer_len);

    AVI_write_frame(vd->avifile,(char *) image_buffer,image_buffer_len, vd->framecount);

    vd->framecount++;

}

void VideoRecorder::Release1() {
    if(video_status==0||file==NULL)
        return;

    //fclose(file);
    //file==NULL;
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