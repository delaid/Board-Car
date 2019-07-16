#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"	

/******电机轮子的驱动开关和相应的定时器引脚***********/


#define PWM_LEFT_SPEED      	TIM8->CCR2  // 左轮的PWM
#define PWM_RIGHT_SPEED      	TIM8->CCR3	//右轮反向转的PWM
#define MOTOR4_PWM__SPEED   	TIM8->CCR4  

#define MOTOR4_HIGH         MOTOR4_PWM__SPEED = 9000
#define MOTOR4_LOW          MOTOR4_PWM__SPEED = 0

#define MOTOR4_DR_HIGH				GPIO_SetBits(GPIOG,GPIO_Pin_10)
#define MOTOR4_DR_LOW 	      GPIO_ResetBits(GPIOG,GPIO_Pin_10)


#define MOTOR4_EN_HIGH				GPIO_SetBits(GPIOG,GPIO_Pin_11)
#define MOTOR4_EN_LOW 	      GPIO_ResetBits(GPIOG,GPIO_Pin_11)


#define MOTOR4_BK_HIGH				GPIO_SetBits(GPIOG,GPIO_Pin_12)
#define MOTOR4_BK_LOW 	      GPIO_ResetBits(GPIOG,GPIO_Pin_12)



#define MOTOR_LEFT_DR_FOWARD					GPIO_SetBits(GPIOD,GPIO_Pin_3)
#define MOTOR_LEFT_DR_BACKWARD 	      GPIO_ResetBits(GPIOD,GPIO_Pin_3)

#define MOTOR_LEFT_EN_EN 							GPIO_SetBits(GPIOD,GPIO_Pin_4)
#define MOTOR_LEFT_EN_DISEN 					GPIO_ResetBits(GPIOD,GPIO_Pin_4)

#define MOTOR_LEFT_BK_EN 							GPIO_ResetBits(GPIOD,GPIO_Pin_5)
#define MOTOR_LEFT_BK_DISEN 					GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define MOTOR_RIGHT_DR_FOWARD					GPIO_SetBits(GPIOD,GPIO_Pin_6)
#define MOTOR_RIGHT_DR_BACKWARD 	    GPIO_ResetBits(GPIOD,GPIO_Pin_6)


#define MOTOR_RIGHT_EN_EN 						GPIO_SetBits(GPIOD,GPIO_Pin_7)
#define MOTOR_RIGHT_EN_DISEN 					GPIO_ResetBits(GPIOD,GPIO_Pin_7)


#define MOTOR_RIGHT_BK_EN 					  GPIO_ResetBits(GPIOG,GPIO_Pin_9)
#define MOTOR_RIGHT_BK_DISEN 					GPIO_SetBits(GPIOG,GPIO_Pin_9)

#define MOTOR_PUSH_DISEN 					    GPIO_SetBits(GPIOF,GPIO_Pin_3)
#define MOTOR_PUSH_EN 					      GPIO_ResetBits(GPIOF,GPIO_Pin_3)
 
#define MOTOR_PULL_DISEN 					    GPIO_SetBits(GPIOF,GPIO_Pin_4)
#define MOTOR_PULL_EN 					      GPIO_ResetBits(GPIOF,GPIO_Pin_4)

#define Limiter1                      PCin(5)           
#define Limiter2                      PBin(0)           
#define Limiter3                      PFin(12)           

/*********************************************************************************************************
** Function name:       MiniBalance_PWM_Init
** Descriptions:        目标板（STM32F103VET6）电机的PWM初始化函数
** input parameters:    arr：自动重装载值.
                        psc：时钟预分频数.
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Motor_PWM_Init(u16 arr,u16 psc);
#endif


