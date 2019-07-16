#include "APDS9960_EXTI.h"

int isr_flag = 0;

//�ⲿ�ж�0�������
void APDS9960_EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;				//�ж϶˿�PB7
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 			//���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��GPIOB7


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//ʹ�ܸ��ù���ʱ��

    //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;				//PB8�ж���ΪLine0
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж�ģʽ	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж�ʹ��
  	EXTI_Init(&EXTI_InitStructure);	 						//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�0�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//�����ȼ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
	
	
}

//�ⲿ�ж�0������� 
void  EXTI9_5_IRQHandler(void)
{
	//OSIntEnter(); 
	if((EXTI_GetITStatus(EXTI_Line7) != RESET))
	{
		isr_flag = 1;
		EXTI_ClearFlag(EXTI_Line7);
		EXTI_ClearITPendingBit(EXTI_Line7); //���LINE0�ϵ��жϱ�־λ 
	}			
	//OSIntExit();  
}
 

