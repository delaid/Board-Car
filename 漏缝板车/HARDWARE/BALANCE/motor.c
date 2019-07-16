#include "motor.h"


void Motor_PWM_Init(u16 arr,u16 psc)
{		 		  	 		
  	
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	/***************************************************************************
//	左轮引脚采用：         C6 C7 C8 C9
//	对应定时器分别为TIM8的CH1、CH2、CH3、CH4
//	****************************************************************************/	
			 
			//此部分需手动修改IO口设置
				RCC->APB2ENR|=1<<13;   //TIM8时钟使能     
				RCC->APB2ENR|=1<<4;   //使能PORTC口时钟  
				
				GPIOC->CRL&=0X00FFFFFF;	//PC6 7输出
				GPIOC->CRL|=0XBB000000;	//复用功能输出 	  	 
					 
				GPIOC->CRH&=0XFFFFFF00;	//PC 8 9输出
				GPIOC->CRH|=0X000000BB;	//复用功能输出
				
				GPIOC->ODR|=15<<6;	   	//PA0 上拉  

				TIM8->ARR=arr;//设定计数器自动重装值  
				TIM8->PSC=psc;//预分频器不分频 

				TIM8->CCMR1|=7<<4;   //CH1 PWM2模式	   
				TIM8->CCMR1|=1<<3;   //CH1预装载使能 
				
				TIM8->CCMR1|=7<<12;  //CH2 PWM2模式	   
				TIM8->CCMR1|=1<<11;  //CH2预装载使能
				
				TIM8->CCMR2|=7<<4;   //CH3 PWM2模式	   
				TIM8->CCMR2|=1<<3;   //CH3预装载使能 
				
				TIM8->CCMR2|=7<<12;  //CH4 PWM2模式	   
				TIM8->CCMR2|=1<<11;  //CH4预装载使能	

				TIM8->CCER|=1<<0;   //OC1 输出使能 
				TIM8->CCER|=1<<1;   //OC1 输出极性 
				
				TIM8->CCER|=1<<4;   //OC2 输出使能 
				TIM8->CCER|=1<<5;   //OC2 输出极性 
				
				TIM8->CCER|=1<<8;   //OC3 输出使能 
				TIM8->CCER|=1<<9;   //OC3 输出极性 
				
				TIM8->CCER|=1<<12;   //OC4 输出使能 
				TIM8->CCER|=1<<13;   //OC4 输出极性 

				TIM8->BDTR=0X8000;	 //设置PMW主输出 
				
				TIM8->CR1|=0x01;    //使能定时器8



				GPIO_InitTypeDef GPIO_InitStructure;
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD ,ENABLE); 
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_Init(GPIOD, &GPIO_InitStructure);

     
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG ,ENABLE);
				GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9| GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
				GPIO_Init(GPIOG, &GPIO_InitStructure);
				


				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF ,ENABLE);
				GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
				GPIO_Init(GPIOF, &GPIO_InitStructure);//
				GPIO_ResetBits(GPIOF,GPIO_Pin_3|GPIO_Pin_4);
		     
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC ,ENABLE);
				GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉输入
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
				GPIO_Init(GPIOC, &GPIO_InitStructure);//
		
				
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
				GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //推挽输出
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
				GPIO_Init(GPIOC, &GPIO_InitStructure);//
				
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF ,ENABLE);
				GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //推挽输出
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
				GPIO_Init(GPIOC, &GPIO_InitStructure);//
				
				
				
		
}


