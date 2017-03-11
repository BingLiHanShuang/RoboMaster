//
// Created by parallels on 3/11/17.
//
#include "SendQueue.h"
queue<BufferData> data_queue;
pthread_cond_t cond_thread =PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx_thread=PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mtx_queue=PTHREAD_MUTEX_INITIALIZER;
void* thread_SendQueue(void *arg){
    //serial=new Serial("/dev/ttyS0");


    while(1){
        pthread_mutex_lock(&mtx_thread);
        int size=data_queue.size();
        for (int i = 0; i < size; ++i) {
            pthread_mutex_lock(&mtx_queue);
            BufferData temp=data_queue.front();
            data_queue.pop();
            pthread_mutex_unlock(&mtx_queue);

            PROTOCOL::presendtoserial(temp.data,temp.len);
            delete temp.data;
        }
        pthread_mutex_unlock(&mtx_thread);
        pthread_cond_wait(&cond_thread,&mtx_thread);
        pthread_mutex_unlock(&mtx_thread);

    }

}