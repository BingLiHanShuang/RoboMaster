#include "protocol_sdk.h"
#include "protocol.h"


/*
 * 串口操作接口
 *
 */
void serial_send(void *payload, uint32_t size){
    //串口发送代码,请把串口发送的部分放入这个函数
    print(payload,size);
    //USART2_send((uint8_t*)payload,size);


}
void serial_receive(uint8_t data){
    //串口中断接收到一个字节的数据的时候,将那一字节的数据作为参数调用这个函数
    GetMessage(data);
}
/*
 * 回调函数
 */
void callback_ScanResult(){
    //无人机扫描结果回调函数,接收到数据后会执行这个函数

}
void callback_PadPass(){
    //九宫格数据到达回调,接收到数据后会执行这个函数

}
void callback_VideoRecordStatus(){
	
    //录像结果接收回调,接收到数据后会执行这个函数
	/*
	if(videoRecordStatusData.status==0)//录像未开始

	if(videoRecordStatusData.status==1)//正在录像
	if(videoRecordStatusData.status==2)//录像已停止
	*/

}

/*
 * 发送接口,下面的函数已经实现,直接调用
 */
void VideoRecord_Start();//开始录像
void VideoRecord_Stop();//停止录像
void VideoRecord_Status();//完成录像


