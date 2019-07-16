#include "led.h"

static void Left_LED_Follow(u32 time,u8 num);
static void Right_LED_Follow(u32 time,u8 num);
static void Down_LED_Follow(u32 time,u8 num);
static void Up_LED_Follow(u32 time,u8 num);
static void LED_Close(void);
//static void OK_LED_Follow(void);
static void Flash_LED(u32 time,u8 num);
static void Dance_fast_led(void);
static void Dance_slow_led(void);

volatile u8 Led_mode=LED_CLOSE;
/*********************************************************************************************************
** Function name:       LED_Brightness
** Descriptions:        LED�����ȵ��ں���
** input parameters:    brightness: 0-100
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LED_Brightness(u8 brightness)
{
    if(control_sleep== true)TIM4->CCR3 =0;//���߻��Ӳ���
	else if(brightness<=100)
	{
		TIM4->CCR3 = 36*brightness;
	}
}

/*********************************************************************************************************
** Function name:       Led_Init
** Descriptions:        ����Led�ƿ��س�ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Night_Led_Init(void)
{
		// ����GPIO�ṹ��
	GPIO_InitTypeDef  			GPIO_InitStructure;
	
	// ���嶨ʱ���ṹ��
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	
	// ���岶��ȽϽṹ��
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	
	// ʹ��GPIO�˿�Dʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	// ʹ�ܸ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// ��ӳ��TIM4����
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
	
	// ʹ��TIM4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// ѡ��IO�ܽ�14
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
	
	// ���ùܽ�Ϊ�����������
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
	
	// ���ùܽ��ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// �����趨������ʼ��GPIO
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// ��䶨ʱ���ṹ��
	TIM_TimeBaseStructure.TIM_Period 		= 3600;
	TIM_TimeBaseStructure.TIM_Prescaler 	= 0;
	TIM_TimeBaseStructure.TIM_ClockDivision	= 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	
	// ��ʼ��TIM4ʱ�����
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// ���ȽϽṹ��
	TIM_OCInitStructure.TIM_OCMode		 	= TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse 			= 0;
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
	
	// TIM4ͨ��3�����ʼ��
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
	// PWM���ʹ��
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	
	// ͨ��3Ԥװ��ʹ��
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	// ʹ��TIM4��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	// ʹ�ܶ�ʱ��4
	TIM_Cmd(TIM4, ENABLE);
	
	// ʹ��IO��ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
	
	// ʹ��GPIO�˿�Aʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// ʹ��SWD ����JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	m_nightlight.mode=CLOSE_LIGHT;//Ĭ��Ϊҹ��ģʽ�¹ص�ģʽ
	
}

/*********************************************************************************************************
** Function name:       Mod_Led_Init
** Descriptions:        ģʽLed�ƿ��س�ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Mod_Led_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// ʹ��GPIOC�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;					// IO�ܽ�8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// �����趨������ʼ��GPIO

    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// ʹ��GPIOC�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;					// IO�ܽ�8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// �����趨������ʼ��GPIO
	Mod_Led_White = 1;
	Control_Led_Blue = 1;	
	Drawpath_Led_Green = 1;	
	Dance_Led_Purple = 1;
	Night_Led_Yellow = 1;
}


/*********************************************************************************************************
** Function name:       Mod_Led_Init
** Descriptions:        ģʽLed�ƿ��س�ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void RGB_Led_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// ʹ��GPIOC�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;					// IO�ܽ�8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// �����趨������ʼ��GPIO
    RGB_LIGHT_BLUE;

}


#define LED_TASK_PRIO				8					//�����������ȼ�
#define LED_STK_SIZE				128				//���������ջ��С
	
OS_STK LED_TASK_STK[LED_STK_SIZE]; 	//�����ջ
void led_task(void *pdata);							//������





void Led_task_Create(void)
{
	OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);	//����APDS9960��������
}

void led_task(void *pdata)
{

	while(1)
	{  
		
		switch (Led_mode)
		{	
			case	LED_CLOSE:LED_Close();
				break;
			case	LEFT_LED_MODE:Left_LED_Follow(250,1);Led_mode=LED_CLOSE;
				break;
			case	RIGHT_LED_MODE:Right_LED_Follow(250,2);Led_mode=LED_CLOSE;
				break;
			case	DOWN_LED_MODE:Down_LED_Follow(250,2);Led_mode=LED_CLOSE;
				break;
			case	UP_LED_MODE:Up_LED_Follow(250,2);Led_mode=LED_CLOSE;
				break;
			case	FLASH_LED_MODE:Flash_LED(250,1);Led_mode=LED_CLOSE;
				break;
			case	OK_LED_MODE:Flash_LED(250,3);Led_mode=LED_CLOSE;
				break;
			case CONTROL_LED_MODE:CONTROL_LED_BLUE;
				break;
			case NIGHT_LIGHT_LED_MODE:NIGHT_LED_YELLOW;
				break;
			case DANCE_LIGHT_LED_MODE:DANCE_LED_PURPLE;
				break;
			case DRAWPATH_LED_MODE:DRAWPATH_LED_GREEN;
				break;
			case DANCE_LED_SLOW:Dance_slow_led();Led_mode=LED_CLOSE;
				break;
			case DANCE_LED_FAST:Dance_fast_led();Led_mode=LED_CLOSE;
				break;
			case DANCE_LED_SHAKE:Dance_fast_led();Led_mode=LED_CLOSE;
				break;
			default:
				break;
		}
		
	}
}


static void Dance_fast_led(void)
{	
	Flash_LED(125,3);
	Left_LED_Follow(125,2);
	Right_LED_Follow(125,2);	
	Flash_LED(125,3);
	Down_LED_Follow(125,2);
	Up_LED_Follow(125,2);
	
}


static void Dance_slow_led(void)
{	
	Flash_LED(250,3);
	Left_LED_Follow(250,2);
	Right_LED_Follow(250,2);	
	Flash_LED(250,3);
	Down_LED_Follow(250,2);
	Up_LED_Follow(250,2);
	
}


static void Left_LED_Follow(u32 time,u8 num)
{
	u8 i=0;
	for(i=0;i<num;i++)
	{
	Mod_Led_White = 1;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_LEFT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_LEFT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_LEFT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_LEFT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 1;
	delay_ms(LED_LEFT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 0;
	delay_ms(LED_LEFT_TIME);
	}

}

static void Right_LED_Follow(u32 time,u8 num)
{
	u8 i=0;
	for(i=0;i<num;i++)
	{
	Mod_Led_White = 1;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 0;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 0;Control_Led_Blue = 0;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =1;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 0;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 0;
	delay_ms(LED_RIGHT_TIME);
	}
}

static void Down_LED_Follow(u32 time,u8 num)
{
	u8 i=0;
	for(i=0;i<num;i++)
	{
	Mod_Led_White = 1;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 0;
	delay_ms(LED_RIGHT_TIME);
	Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 0;
	delay_ms(LED_RIGHT_TIME);
	}
}

static void Up_LED_Follow(u32 time,u8 num)
{
	u8 i=0;
	for(i=0;i<num;i++)
	{
		Mod_Led_White = 1;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
		delay_ms(time);
		Mod_Led_White = 1;Drawpath_Led_Green =1;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 1;
		delay_ms(time);
		Mod_Led_White = 1;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 0;
		delay_ms(time);
		Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 0;
		delay_ms(time);
	}
}


static void LED_Close(void)
{
	Mod_Led_White = 1;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
}

static void Flash_LED(u32 time,u8 num)
{
	u8 i=0;
	for(i=0;i<num;i++)
	{
		Mod_Led_White = 1;Drawpath_Led_Green =1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;Control_Led_Blue = 1;
		delay_ms(time);
		Mod_Led_White = 0;Drawpath_Led_Green =0;Dance_Led_Purple = 0;Night_Led_Yellow = 0;Control_Led_Blue = 0;
		delay_ms(time);
	}
}





