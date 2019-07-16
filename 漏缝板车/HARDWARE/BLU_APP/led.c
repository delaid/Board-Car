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
** Descriptions:        LED的亮度调节函数
** input parameters:    brightness: 0-100
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LED_Brightness(u8 brightness)
{
    if(control_sleep== true)TIM4->CCR3 =0;//休眠机子不亮
	else if(brightness<=100)
	{
		TIM4->CCR3 = 36*brightness;
	}
}

/*********************************************************************************************************
** Function name:       Led_Init
** Descriptions:        左右Led灯开关初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Night_Led_Init(void)
{
		// 定义GPIO结构体
	GPIO_InitTypeDef  			GPIO_InitStructure;
	
	// 定义定时器结构体
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	
	// 定义捕获比较结构体
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	
	// 使能GPIO端口D时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	// 使能复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 重映射TIM4引脚
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
	
	// 使能TIM4时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// 选择IO管脚14
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
	
	// 设置管脚为复用推挽输出
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
	
	// 设置管脚速度为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 根据设定参数初始化GPIO
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// 填充定时器结构体
	TIM_TimeBaseStructure.TIM_Period 		= 3600;
	TIM_TimeBaseStructure.TIM_Prescaler 	= 0;
	TIM_TimeBaseStructure.TIM_ClockDivision	= 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	
	// 初始化TIM4时间基数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// 填充比较结构体
	TIM_OCInitStructure.TIM_OCMode		 	= TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse 			= 0;
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
	
	// TIM4通道3输出初始化
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
	// PWM输出使能
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	
	// 通道3预装载使能
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	// 使能TIM4在ARR上的预装载寄存器
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	// 使能定时器4
	TIM_Cmd(TIM4, ENABLE);
	
	// 使能IO重映射时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
	
	// 使能GPIO端口A时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 使能SWD 禁用JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	m_nightlight.mode=CLOSE_LIGHT;//默认为夜灯模式下关灯模式
	
}

/*********************************************************************************************************
** Function name:       Mod_Led_Init
** Descriptions:        模式Led灯开关初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Mod_Led_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// 使能GPIOC端口时钟
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;					// IO管脚8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// 根据设定参数初始化GPIO

    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// 使能GPIOC端口时钟
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;					// IO管脚8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// 根据设定参数初始化GPIO
	Mod_Led_White = 1;
	Control_Led_Blue = 1;	
	Drawpath_Led_Green = 1;	
	Dance_Led_Purple = 1;
	Night_Led_Yellow = 1;
}


/*********************************************************************************************************
** Function name:       Mod_Led_Init
** Descriptions:        模式Led灯开关初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void RGB_Led_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// 使能GPIOC端口时钟
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;					// IO管脚8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// 根据设定参数初始化GPIO
    RGB_LIGHT_BLUE;

}


#define LED_TASK_PRIO				8					//设置任务优先级
#define LED_STK_SIZE				128				//设置任务堆栈大小
	
OS_STK LED_TASK_STK[LED_STK_SIZE]; 	//任务堆栈
void led_task(void *pdata);							//任务函数





void Led_task_Create(void)
{
	OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);	//创建APDS9960测试任务
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





