#include "usart2.h"


////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

/*********************************************************************************************************
** Function name:       usart2_init
** Descriptions:        目标板（STM32F103VET6）初始化USAR2
** input parameters:    bound:串口波特率
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void usart2_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD| RCC_APB2Periph_AFIO, ENABLE);			//使能GPIOD时钟和重映射时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);								//使能USART3

	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);										//I/O口重映射开启,重映射到PD8和PD9上需要选择完全重映射

	//USART2_TX   GPIOD.5
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5; 										//PD.5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;									//复用推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//初始化GPIOD.5

	//USART2_RX	  GPIOD.6
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;											//PD.6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;								//浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//初始化GPIOD.6  

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;							//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;									//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;										//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);														//根据指定的参数初始化VIC寄存器

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate   = bound;										//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//字长为8位数据格式
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity     = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//收发模式

	USART_Init(USART2, &USART_InitStructure);											//初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);										//开启串口接受中断
	USART_Cmd(USART2, ENABLE);															//使能串口2

}


/*********************************************************************************************************
** Function name:       UASRT2_Send_Buf
** Descriptions:        USAR2发送缓存数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UASRT2_Send_Buf(u8 *buf, u16 start_number, u16 length)
{
	u16 j;
	for(j=start_number;j<length;j++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);			// 循环发送,直到发送完毕
		USART_SendData(USART2,buf[j]);
	}
}



//小鸡哔哔音效控制指令存储
unsigned char Sound[16][4] = {  
								{0xFF,0x01,0x01,0x0D},
								{0xFF,0x01,0x02,0x10},
								{0xFF,0x01,0x03,0x0D},
								{0xFF,0x01,0x04,0x0D},
								{0xFF,0x01,0x05,0x0D},
								{0xFF,0x01,0x06,0x0D},
								{0xFF,0x01,0x07,0x0D},
								{0xFF,0x01,0x08,0x0D},
								{0xFF,0x01,0x09,0x0D},
								{0xFF,0x01,0x0A,0x0D},
								{0xFF,0x01,0x0B,0x0D},
								{0xFF,0x01,0x0C,0x0D},
								{0xFF,0x01,0x0D,0x0D},
								{0xFF,0x01,0x0E,0x0D},
								{0xFF,0x01,0x0F,0x10},
								{0xFF,0x01,0x05,0x00},
							};		

	

/*********************************************************************************************************
** Function name:       Music_Sound
** Descriptions:        音效发送数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/							
u8 m_sound_type=0;
bool sound_update=0; 
							
void Music_Sound(void)
{
		    
		if(sound_update==true && power_off==false)
		{
			switch (m_sound_type)
			{
				case	CHEICKET_SOUND:		UASRT2_Send_Buf(Sound[0],0,4);
					break;
				case	PROGRAME_SOUND:		UASRT2_Send_Buf(Sound[1],0,4);
					break;
				case	BACK_LOVELY_SOUND:	UASRT2_Send_Buf(Sound[2],0,4);
					break;
				case	BACK_PRIDE_SOUND:	UASRT2_Send_Buf(Sound[3],0,4);
					break;
				case	FORN_SURPRISED_SOUND:UASRT2_Send_Buf(Sound[4],0,4);
					break;
				case	RIGHT_NEVERS_SOUND:	UASRT2_Send_Buf(Sound[5],0,4);
					break;
				case	RIGHT_HAPPY_SOUND:	UASRT2_Send_Buf(Sound[6],0,4);
					break;
				case	RIGHT_ANGRY_SOUND:	UASRT2_Send_Buf(Sound[7],0,4);
					break;
				case	LEFT_NEVERS_SOUND:	UASRT2_Send_Buf(Sound[8],0,4);
					break;
				case	LEFT_HAPPY_SOUND:	UASRT2_Send_Buf(Sound[9],0,4);
					break;
				case	LEFT_ANGERY_SOUND:	UASRT2_Send_Buf(Sound[10],0,4);
					break;
				case	END_SOUND:			UASRT2_Send_Buf(Sound[11],0,4);
					break;
				case	BLUE_RIVER_SOUND:	UASRT2_Send_Buf(Sound[12],0,4);	
					break;
				case	MODE_CHANGE_SOUND:	UASRT2_Send_Buf(Sound[13],0,4);
					break;
				case	NIGHT_SOUND:		UASRT2_Send_Buf(Sound[14],0,4);
					break;
				case	STOP_SOUND:			UASRT2_Send_Buf(Sound[15],0,4);
					break;
			}
			sound_update=false;

		 }
	
}








