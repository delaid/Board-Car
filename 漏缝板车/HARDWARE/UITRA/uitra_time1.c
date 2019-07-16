#include "uitra_time1.h"

static void Uitra_Trig_Init(void);
static void TIM1_Cap_Init(u16 arr,u16 psc);





//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于 32 位定时器来说,1us 计数器加 1,溢出时间:4294 秒)
u8 	TIM1CH1_CAPTURE_STA = 0;		//输入捕获状态
u16 TIM1CH1_CAPTURE_VAL;			//输入捕获值

volatile u32 Distance;						// 距离

/*********************************************************************************************************
** Function name:       Uitra_Init
** Descriptions:        目标板（STM32F103VET6）超声波初始化
** input parameters:    arr：自动重装载值.
                        psc：时钟预分频数.
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Uitra_Init(u16 arr,u16 psc)
{
	Uitra_Trig_Init();
	TIM1_Cap_Init(arr,psc);
}


/*********************************************************************************************************
** Function name:       Read_Distance
** Descriptions:        目标板（STM32F103VET6）超声波接收回波函数.
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Read_Distance(void)
{
	TRIG=0;
	delay_us(15);
	TRIG=1;
	delay_us(15);
	TRIG=0;
	
	if(TIM1CH1_CAPTURE_STA&0X80)								// 成功捕获到了一次高电平
	{
		Distance 	= TIM1CH1_CAPTURE_STA&0X3F;
		Distance   *= 65536;					        		// 溢出时间总和
		Distance   += TIM1CH1_CAPTURE_VAL;						// 得到总的高电平时间
		Distance 	= Distance*170/1000;
		TIM1CH1_CAPTURE_STA=0;									// 开启下一次捕获
	}				
}


/*********************************************************************************************************
** Function name:       Uitra_Trig_Init
** Descriptions:        目标板（STM32F103VET6）超声波触发信号引脚初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void Uitra_Trig_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				 //触发IOPC9 端口配置
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.9
	GPIO_SetBits(GPIOC,GPIO_Pin_9);							 //PC.9输出高
}



/*********************************************************************************************************
** Function name:       TIM1_Cap_Init
** Descriptions:        目标板（STM32F103VET6）高级定时器TIM1捕获初始化.
** input parameters:    arr：自动重装载值.
                        psc：时钟预分频数.
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void TIM1_Cap_Init(u16 arr,u16 psc)	
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  			TIM1_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);								// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);								// 使能TIM1时钟（72M）

	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_8;  										// PA8 清除之前设置  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;									// IO口速度为50MHz
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPD;									// PA8 下拉输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);													// PA8 下拉

	//初始化定时器1 TIM1	 
	TIM_TimeBaseStructure.TIM_Period 		= arr; 										// 设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler 	= psc; 										// 预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 							// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  						// TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 									// 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM1输入捕获参数
	TIM1_ICInitStructure.TIM_Channel 		= TIM_Channel_1; 							// CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM1_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;					// 上升沿捕获
  	TIM1_ICInitStructure.TIM_ICSelection 	= TIM_ICSelection_DirectTI; 				// 映射到TI1上
  	TIM1_ICInitStructure.TIM_ICPrescaler	= TIM_ICPSC_DIV1;	 						// 配置输入分频,不分频 
  	TIM1_ICInitStructure.TIM_ICFilter 		= 0x00;										// IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM1, &TIM1_ICInitStructure);

	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM1_CC_IRQn; 			// TIM1捕获中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;  						// 先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 2;  						// 从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 					// IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  													// 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM1_UP_IRQn; 			// TIM1更新中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;  						// 先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 3;  						// 从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 					// IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  													// 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);									// 允许更新中断 ,允许CC1IE捕获中断	
	TIM_Cmd(TIM1,ENABLE );																// 使能TIM1
	delay_us(100);
}


/*********************************************************************************************************
** Function name:       TIM1_CC_IRQHandler
** Descriptions:        目标板（STM32F103VET6）高级定时器TIM1捕获中断，超声波回波脉宽读取中断.
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void TIM1_CC_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)										//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)						//TIM1捕获1是否发生捕获事件
		{
			if(TIM1CH1_CAPTURE_STA&0X40)									//捕获到一个下降沿 
			{
				TIM1CH1_CAPTURE_STA|=0X80;									//标记成功捕获到一次高电平脉宽
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);					//获取当前的捕获值.
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); 			//设置上升沿捕获
			}
			else															//捕获到一个上升沿
			{
				TIM1CH1_CAPTURE_STA      = 0;								//清空
				TIM1CH1_CAPTURE_VAL      = 0;
				TIM1CH1_CAPTURE_STA		|= 0X40;							//标记捕获到了上升沿
				TIM_Cmd(TIM1,ENABLE ); 										//使能定时器1
				TIM_SetCounter(TIM1,0); 									//计数器清空
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);			//设置下降沿捕获
				TIM_Cmd(TIM1,ENABLE ); 										//使能定时器1
			}
		}
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);								//清除中断标志位 
}


/*********************************************************************************************************
** Function name:       TIM1_UP_IRQHandler
** Descriptions:        目标板（STM32F103VET6）高级定时器TIM1更新中断，超声波回波脉宽读取的周期时间.
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)								// 还未成功捕获	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			// 溢出
		{	    
			if(TIM1CH1_CAPTURE_STA&0X40)							// 已经捕获到高电平了
			{
				if((TIM1CH1_CAPTURE_STA&0X3F)==0X3F)				// 高电平太长了
				{
					TIM1CH1_CAPTURE_STA|=0X80;						// 标记成功捕获了一次
					TIM1CH1_CAPTURE_VAL=0XFFFF;
				}
				else 
					TIM1CH1_CAPTURE_STA++;
			}	 
		}	     	    					   
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 					// 清除中断标志位 	  
}
