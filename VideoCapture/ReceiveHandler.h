//
// Created by parallels on 3/24/17.
//

#ifndef VIDEOCAPTURE_RECEIVEQUEUE_H
#define VIDEOCAPTURE_RECEIVEQUEUE_H

#include <cstdint>
#include <cstdlib>
#include <queue>
#include "VideoRecorder.h"
#include "protocol.pb-c.h"
#include "DataType.h"

using namespace std;
extern VideoRecord videoRecord;

extern queue<Message*> data_queue;


void* thread_server_udp(void *arg);
void* thread_message_handler(void *arg);

#endif //VIDEOCAPTURE_RECEIVEQUEUE_H
