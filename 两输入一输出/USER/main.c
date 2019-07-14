#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "timer.h"


int main(void)
 {
	 int i;
 	vu8 key=0;	
	delay_init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
 
 	LED_Init();			     
	KEY_Init(); 
  TIM3_Int_Init(9999 , 3599);	 //10Khz的计数频率，计数到10000为1s 
  for(i=0;i<5;i++)
				{
						delay_ms(1000);
				}	 	

	while(1)
	{
	//KEY0人体红GPIO_Pin_6 检查到人低电平  KEY1普通红外开关 P7检查到遮挡物低电平
	
		     if( KEY1==1)
				 {
					   i=30;
					   while(--i)
						 {
								delay_ms(1000);
						    if( KEY1!=1 )
								break;										
							
					    }
						 if(i==0)
						 {
								LED1=1;
								//TIM_Cmd(TIM3, ENABLE );  //使能TIMx
								for(i=0;i<10;i++)
								{
									 delay_ms(1000);
								}	
								LED1=0;
						 }
         }	
		
		}
}

