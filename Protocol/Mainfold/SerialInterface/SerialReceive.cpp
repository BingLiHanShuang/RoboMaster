//
// Created by parallels on 3/23/17.
//

#include "SerialReceive.h"
void* thread_Receive(void *arg){
    while(1){
        BufferData* bufferData=serial->Read();
        if(bufferData->len>0){
            for (int i = 0; i < bufferData->len; ++i) {
                PROTOCOL::GetMessage(bufferData->data[i]);
            }
        }
        delete bufferData->data;
        delete bufferData;

    }

}