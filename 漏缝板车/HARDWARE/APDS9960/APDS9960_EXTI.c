#include "APDS9960_EXTI.h"

int isr_flag = 0;

//外部中断0服务程序
void APDS9960_EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;				//中断端口PB7
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 			//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化GPIOB7


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//使能复用功能时钟

    //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;				//PB8中断线为Line0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断模式	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断使能
  	EXTI_Init(&EXTI_InitStructure);	 						//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级0， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
	
	
}

//外部中断0服务程序 
void  EXTI9_5_IRQHandler(void)
{
	//OSIntEnter(); 
	if((EXTI_GetITStatus(EXTI_Line7) != RESET))
	{
		isr_flag = 1;
		EXTI_ClearFlag(EXTI_Line7);
		EXTI_ClearITPendingBit(EXTI_Line7); //清除LINE0上的中断标志位 
	}			
	//OSIntExit();  
}
 

