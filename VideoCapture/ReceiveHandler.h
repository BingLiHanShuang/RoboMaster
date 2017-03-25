//
// Created by parallels on 3/24/17.
//

#ifndef VIDEOCAPTURE_RECEIVEQUEUE_H
#define VIDEOCAPTURE_RECEIVEQUEUE_H

#include <cstdint>
#include <cstdlib>
#include <queue>
#include "protocol.pb-c.h"

using namespace std;
extern queue<Message*> data_queue;
struct BufferData{
    uint8_t * data;
    size_t len;
};

void* thread_message_handler(void *arg);

#endif //VIDEOCAPTURE_RECEIVEQUEUE_H
