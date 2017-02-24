/*********************************************************************************************************
*
* File                : usart.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#include "usart.h"
#include "stm32f10x.h"
#include "User_systick.h"
void _ttywrch(int ch) { }

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

	void uart1_init(){
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

		NVIC_InitTypeDef NVIC_InitStructure;    
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//∆Ù”√ ±÷”
	
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//÷ÿ”≥…‰∂Àø⁄
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//????
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//????
		GPIO_Init(GPIOB,&GPIO_InitStructure);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		USART_InitTypeDef USART_InitStructure;
	
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		
		USART_ITConfig(USART1, USART_IT_PE, ENABLE);
		USART_ITConfig(USART1, USART_IT_ERR, ENABLE);
		USART_Cmd(USART1, ENABLE);

}
void uart4_init(){
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

		NVIC_InitTypeDef NVIC_InitStructure;    
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);//∆Ù”√ ±÷”
	
		//GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//÷ÿ”≥…‰∂Àø⁄
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//????
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOC,&GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//????
		GPIO_Init(GPIOC,&GPIO_InitStructure);
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
		USART_InitTypeDef USART_InitStructure;
	
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(UART4, &USART_InitStructure);
		
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);		
		USART_Cmd(UART4, ENABLE);

}

void UsartGPIO_CTRT_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void delay_us(u16 time)
{
u16 i=0;  
while(time--)
	{
i=10;  //Ëá™Â∑±ÂÆö‰πâ
while(i--) ;    

	}

}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		uint8_t mchar=USART_ReceiveData(USART1);
		GetMessage(mchar);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 

}
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
{//Âêå  @arg USART_IT_ORE_ER : OverRun Error interrupt if the EIE bit is set  

USART_ReceiveData(USART1); //ÂèñÂá∫Êù•ÊâîÊéâ
USART_ClearFlag(USART1, USART_FLAG_ORE);
}

if(USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET)
{//Âêå  @arg USART_IT_NE     : Noise Error interrupt
USART_ClearFlag(USART1, USART_FLAG_NE);
}


if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
{//Âêå   @arg USART_IT_FE     : Framing Error interrupt
USART_ClearFlag(USART1, USART_FLAG_FE);
}

if(USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
{//Âêå  @arg USART_IT_PE     : Parity Error interrupt
USART_ClearFlag(USART1, USART_FLAG_PE);
}
}
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		//getmessage(UART4);
		USART_ClearITPendingBit(UART4,USART_IT_RXNE); 

	}
}
void USART_CTRT_Configuartion(void)
{
	USART_InitTypeDef USART_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);

	UsartGPIO_CTRT_Configuration();
}


/* Use no semihosting */
#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{  
	int handle;
};
FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}
#endif

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);
	//USART_SendData(USART1,(u8)ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	//while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}
