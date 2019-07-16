#include "uitra_time1.h"

static void Uitra_Trig_Init(void);
static void TIM1_Cap_Init(u16 arr,u16 psc);





//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(���� 32 λ��ʱ����˵,1us �������� 1,���ʱ��:4294 ��)
u8 	TIM1CH1_CAPTURE_STA = 0;		//���벶��״̬
u16 TIM1CH1_CAPTURE_VAL;			//���벶��ֵ

volatile u32 Distance;						// ����

/*********************************************************************************************************
** Function name:       Uitra_Init
** Descriptions:        Ŀ��壨STM32F103VET6����������ʼ��
** input parameters:    arr���Զ���װ��ֵ.
                        psc��ʱ��Ԥ��Ƶ��.
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uitra_Init(u16 arr,u16 psc)
{
	Uitra_Trig_Init();
	TIM1_Cap_Init(arr,psc);
}


/*********************************************************************************************************
** Function name:       Read_Distance
** Descriptions:        Ŀ��壨STM32F103VET6�����������ջز�����.
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Read_Distance(void)
{
	TRIG=0;
	delay_us(15);
	TRIG=1;
	delay_us(15);
	TRIG=0;
	
	if(TIM1CH1_CAPTURE_STA&0X80)								// �ɹ�������һ�θߵ�ƽ
	{
		Distance 	= TIM1CH1_CAPTURE_STA&0X3F;
		Distance   *= 65536;					        		// ���ʱ���ܺ�
		Distance   += TIM1CH1_CAPTURE_VAL;						// �õ��ܵĸߵ�ƽʱ��
		Distance 	= Distance*170/1000;
		TIM1CH1_CAPTURE_STA=0;									// ������һ�β���
	}				
}


/*********************************************************************************************************
** Function name:       Uitra_Trig_Init
** Descriptions:        Ŀ��壨STM32F103VET6�������������ź����ų�ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void Uitra_Trig_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				 //����IOPC9 �˿�����
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.9
	GPIO_SetBits(GPIOC,GPIO_Pin_9);							 //PC.9�����
}



/*********************************************************************************************************
** Function name:       TIM1_Cap_Init
** Descriptions:        Ŀ��壨STM32F103VET6���߼���ʱ��TIM1�����ʼ��.
** input parameters:    arr���Զ���װ��ֵ.
                        psc��ʱ��Ԥ��Ƶ��.
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void TIM1_Cap_Init(u16 arr,u16 psc)	
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  			TIM1_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);								// ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);								// ʹ��TIM1ʱ�ӣ�72M��

	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_8;  										// PA8 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;									// IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPD;									// PA8 ��������  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);													// PA8 ����

	//��ʼ����ʱ��1 TIM1	 
	TIM_TimeBaseStructure.TIM_Period 		= arr; 										// �趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler 	= psc; 										// Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 							// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  						// TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 									// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM1���벶�����
	TIM1_ICInitStructure.TIM_Channel 		= TIM_Channel_1; 							// CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM1_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;					// �����ز���
  	TIM1_ICInitStructure.TIM_ICSelection 	= TIM_ICSelection_DirectTI; 				// ӳ�䵽TI1��
  	TIM1_ICInitStructure.TIM_ICPrescaler	= TIM_ICPSC_DIV1;	 						// ���������Ƶ,����Ƶ 
  	TIM1_ICInitStructure.TIM_ICFilter 		= 0x00;										// IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM1, &TIM1_ICInitStructure);

	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM1_CC_IRQn; 			// TIM1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;  						// ��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 2;  						// �����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 					// IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  													// ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM1_UP_IRQn; 			// TIM1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;  						// ��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 3;  						// �����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 					// IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  													// ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);									// ��������ж� ,����CC1IE�����ж�	
	TIM_Cmd(TIM1,ENABLE );																// ʹ��TIM1
	delay_us(100);
}


/*********************************************************************************************************
** Function name:       TIM1_CC_IRQHandler
** Descriptions:        Ŀ��壨STM32F103VET6���߼���ʱ��TIM1�����жϣ��������ز������ȡ�ж�.
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void TIM1_CC_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)										//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)						//TIM1����1�Ƿ��������¼�
		{
			if(TIM1CH1_CAPTURE_STA&0X40)									//����һ���½��� 
			{
				TIM1CH1_CAPTURE_STA|=0X80;									//��ǳɹ�����һ�θߵ�ƽ����
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);					//��ȡ��ǰ�Ĳ���ֵ.
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); 			//���������ز���
			}
			else															//����һ��������
			{
				TIM1CH1_CAPTURE_STA      = 0;								//���
				TIM1CH1_CAPTURE_VAL      = 0;
				TIM1CH1_CAPTURE_STA		|= 0X40;							//��ǲ�����������
				TIM_Cmd(TIM1,ENABLE ); 										//ʹ�ܶ�ʱ��1
				TIM_SetCounter(TIM1,0); 									//���������
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);			//�����½��ز���
				TIM_Cmd(TIM1,ENABLE ); 										//ʹ�ܶ�ʱ��1
			}
		}
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);								//����жϱ�־λ 
}


/*********************************************************************************************************
** Function name:       TIM1_UP_IRQHandler
** Descriptions:        Ŀ��壨STM32F103VET6���߼���ʱ��TIM1�����жϣ��������ز������ȡ������ʱ��.
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)								// ��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			// ���
		{	    
			if(TIM1CH1_CAPTURE_STA&0X40)							// �Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM1CH1_CAPTURE_STA&0X3F)==0X3F)				// �ߵ�ƽ̫����
				{
					TIM1CH1_CAPTURE_STA|=0X80;						// ��ǳɹ�������һ��
					TIM1CH1_CAPTURE_VAL=0XFFFF;
				}
				else 
					TIM1CH1_CAPTURE_STA++;
			}	 
		}	     	    					   
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 					// ����жϱ�־λ 	  
}
