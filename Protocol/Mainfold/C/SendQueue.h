//
// Created by parallels on 3/11/17.
//
#include <queue>
#include <pthread.h>
#include "protocol.h"

using namespace std;

#ifndef C_SENDQUEUE_H
#define C_SENDQUEUE_H
struct BufferData{
    char * data;
    int len;

};
extern pthread_cond_t cond_thread;
extern pthread_mutex_t mtx_thread;
extern pthread_mutex_t mtx_queue;
extern queue<BufferData> data_queue;
void* thread_SendQueue(void *arg);
#endif //C_SENDQUEUE_H
