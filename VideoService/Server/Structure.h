//
// Created by parallels on 4/27/17.
//
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#ifndef VIDEOSERVER_STRUCTURE_H
#define VIDEOSERVER_STRUCTURE_H
extern FILE *flog;
void printlog(const char *format,...);
struct shared_package{
    pthread_mutex_t image_lock;
    int image_size;
    char image_data[307200];        //640*480图像数据
};
struct shared_package * get_shared_package();


#endif //VIDEOSERVER_STRUCTURE_H
