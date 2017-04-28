//
// Created by parallels on 4/27/17.
//

#include "Structure.h"
FILE *flog;
struct shared_package *shared_package_ptr=NULL;
using namespace std;
using namespace cv;
struct shared_package * get_shared_package(){
    int shmid;
    shmid = shmget(960826, sizeof(struct shared_package), 0666 | IPC_CREAT);
    struct shared_package * s = (struct shared_package *) shmat(shmid, NULL, 0);
    printf("shmid:%d, p:%lx\n", shmid, s);
    return s;
}
uint8_t getImageFromMemory(Mat &image){
    if(shared_package_ptr==NULL)shared_package_ptr=get_shared_package();
    pthread_mutex_lock(&shared_package_ptr->image_lock);
    int nowsize = shared_package_ptr->image_size;
    vector<char> img_data(shared_package_ptr->image_data, shared_package_ptr->image_data + nowsize);
    pthread_mutex_unlock(&shared_package_ptr->image_lock);
    if(nowsize==0)return -1;
    try{
        image = imdecode(Mat(img_data), CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_COLOR);

    }catch (exception& e){
        return -1;
    }
    return 0;


}
void printlog(const char *format,...)
{
    va_list args;

    va_start(args ,format);
    vprintf(format ,args);
    va_end(args);

    va_start(args, format);
    vfprintf(flog, format, args);
    va_end(args);
    fflush(flog);
}