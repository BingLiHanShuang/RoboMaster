#include "stm32f10x_iwdg.h"
void IWDG_Init(void)
{
    //1/????????
       //2/?????????????????
   //3/????????
     //4/?????
    
      IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
    
      IWDG_SetPrescaler(IWDG_Prescaler_4);          
    
      IWDG_SetReload(625);                           
      
      IWDG_ReloadCounter();
    
      IWDG_Enable();                                
}

void IWDG_Feed(void)
{
   IWDG_ReloadCounter();              //??????
}