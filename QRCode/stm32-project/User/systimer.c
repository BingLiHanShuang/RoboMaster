#include "systimer.h"
typedef struct _timeslice{
	void (*function)();
	int clock;
	int period;
	
} timeslice;
typedef struct _timeslicenode{
	struct _timeslicenode  *next;
	timeslice *val;
} timeslicenode;
timeslicenode *head;
void init_scheduler(){
	head=(timeslicenode*)malloc(sizeof(timeslicenode));
	
	
	
	

}
void RCC_cfg()
{
      
       //定义错误状态变量
       ErrorStatus HSEStartUpStatus;
      
       //将RCC寄存器重新设置为默认值
       RCC_DeInit();
 
       //打开外部高速时钟晶振
       RCC_HSEConfig(RCC_HSE_ON);
 
       //等待外部高速时钟晶振工作
       HSEStartUpStatus = RCC_WaitForHSEStartUp();
       if(HSEStartUpStatus == SUCCESS)
       {
              //设置AHB时钟(HCLK)为系统时钟
              RCC_HCLKConfig(RCC_SYSCLK_Div1);
 
              //设置高速AHB时钟(APB2)为HCLK时钟
              RCC_PCLK2Config(RCC_HCLK_Div1);
 
              //设置低速AHB时钟(APB1)为HCLK的2分频
              RCC_PCLK1Config(RCC_HCLK_Div2);
             
              //设置FLASH代码延时
              FLASH_SetLatency(FLASH_Latency_2);
 
              //使能预取指缓存
              FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
 
              //设置PLL时钟，为HSE的9倍频 8MHz * 9 = 72MHz
              RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
 
              //使能PLL
              RCC_PLLCmd(ENABLE);
 
              //等待PLL准备就绪
              while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
 
              //设置PLL为系统时钟源
              RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
              //判断PLL是否是系统时钟
              while(RCC_GetSYSCLKSource() != 0x08);
       }
 
       //允许TIM2的时钟
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
       //允许GPIO的时钟
       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 
}
void TIM2_IRQHandler(void);
void TIMER_cfg()

{

       TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

 

       //重新将Timer设置为缺省值

       TIM_DeInit(TIM2);

       //采用内部时钟给TIM2提供时钟源

       TIM_InternalClockConfig(TIM2);

       //预分频系数为36000-1，这样计数器时钟为72MHz/36000 = 2kHz

       TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;

       //设置时钟分割

       TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

       //设置计数器模式为向上计数模式

       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

       //设置计数溢出大小，每计2000个数就产生一个更新事件

       TIM_TimeBaseStructure.TIM_Period = 2000 - 1;

       //将配置应用到TIM2中

       TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

 

       //清除溢出中断标志

       TIM_ClearFlag(TIM2, TIM_FLAG_Update);

       //禁止ARR预装载缓冲器

       TIM_ARRPreloadConfig(TIM2, DISABLE);

       //开启TIM2的中断

       TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

}
void NVIC_cfg()

{

       NVIC_InitTypeDef NVIC_InitStructure;

        //选择中断分组1

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        

        

        //选择TIM2的中断通道

        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      

        //抢占式中断优先级设置为0

        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

       //响应式中断优先级设置为0

        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

        //使能中断

        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

        NVIC_Init(&NVIC_InitStructure);

}
void TIM2_IRQHandler(void)

{

       u8 ReadValue;

       //检测是否发生溢出更新事件

       if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)

       {
				 
				 

              //清除TIM2的中断待处理位

              TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);

      

       }

 

}