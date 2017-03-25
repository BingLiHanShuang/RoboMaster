//
// Created by parallels on 3/24/17.
//

#include "ReceiveHandler.h"
#include "UDPServer.h"
#include "protocol.pb-c.h"
#include <pthread.h>
queue<Message*> data_queue;
pthread_mutex_t data_mutex=PTHREAD_MUTEX_INITIALIZER;
void callback_receive(BufferData &data){

    Message* temp=message__unpack(NULL,data.len,data.data);
    pthread_mutex_lock(&data_mutex);
    data_queue.push(temp);
    pthread_mutex_unlock(&data_mutex);

}
void callback_finish(){


}
void* thread_server_udp(void *arg){
    UDPServer udpServer(6001);
    udpServer.setReceiveCallback(callback_receive);
    udpServer.setFinishCasllback(callback_finish);
    udpServer.listen();

}
void* thread_message_handler(void *arg){
    while (1){
        while (!data_queue.empty()){
            pthread_mutex_lock(&data_mutex);
            Message* message=data_queue.front();
            data_queue.pop();
            pthread_mutex_unlock(&data_mutex);
            switch(message->messagetype){
                case MESSAGE__MESSAGE_TYPE__VideoRecord:
                    VideoRecord* videoRecord=video_record__unpack(NULL,message->data.len,message->data.data);
                    switch (videoRecord->control){
                        case VIDEO_RECORD__CONTROL_TYPE__Start://start video recording

                            break;
                        case VIDEO_RECORD__CONTROL_TYPE__Stop://stop video recording
                            break;
                        case VIDEO_RECORD__CONTROL_TYPE__Status://show the recording status
                            break;
                        default:
                            break;
                    }
                    video_record__free_unpacked(videoRecord,NULL);
                    message__free_unpacked(message,NULL);

                    break;
                default:
                    //not handle it and print a error;
                    cout<<"error message int ReceiveHandler.cpp in thread_message_handler"<<endl;
                    break;
            }
        }
    }


}