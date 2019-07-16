#include "usart1.h"

//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

static void USART1_Free(u8 Res_char);
static void USART1_Start(u8 Res_char);
static void USART1_Res_data(u8 Res_char);
static void USART1_End(u8 Res_char);
//static void USART1_Data_Check(void);
static void clean_rebuff_USART1(void);
static void UASRT1_Send_Buf(u8 *buf, u16 start_number, u16 length);
static u8 continuous_tmp_buff[4]={0x80,0x06,0x03,0x77};	//连续发送数组
/*********************************************************************************************************
** Function name:       USART1_init
** Descriptions:        目标板（STM32F103ZET6）初始化USART1
** input parameters:    bound:串口波特率
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void usart1_init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 							//发送缓冲,最大USART1_MAX_SEND_LEN字节

/*********************************************************************************************************
** Function name:       UASRT2_Send_Buf
** Descriptions:        USAR2??????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
static void UASRT1_Send_Buf(u8 *buf, u16 start_number, u16 length)
{
	u16 j;
	for(j=start_number;j<length;j++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);		
		USART_SendData(USART1,buf[j]);
	}
}

/*********************************************************************************************************
** Function name:       u3_printf
** Descriptions:        USART1的输出格式变成printf输出格式，并确保一次发送数据不超过USART_MAX_SEND_LEN字节
** input parameters:    fmt：字符串指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void u1_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);						//此次发送数据的长度
	for(j=0;j<i;j++)											//循环发送数据
	{
	  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 	//循环发送,直到发送完毕   
		USART_SendData(USART1,USART1_TX_BUF[j]); 
	} 
}

volatile  u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 
u16 USART1_RX_STA=0;  

void USART1_IRQHandler(void)
{
	 char Res;
   u8 status;
#if SYSTEM_SUPPORT_OS 														// 如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  					// 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		  Res =USART_ReceiveData(USART1);										// 读取接收到的数据
		  status = USART1_RX_STA >> 8;
		  switch (status)
			{

				case 0x00:USART1_Free(Res);		break;
				case 0x80:USART1_Start(Res);	break;
				case 0xC0:USART1_Res_data(Res);	break;
				case 0xE0:USART1_End(Res);		break;
				default:
				break;
			}
	}
	
#if SYSTEM_SUPPORT_OS 														// 如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}



static void USART1_Free(u8 Res_char)
{
	u8 star_sign = Res_char;
	if(star_sign ==  0x80)													
	{
		USART1_RX_BUF[USART1_RX_STA&0XFF]=Res_char;					
		USART1_RX_STA++;
		USART1_RX_STA |= 0x8000;											
	}	
}




static void USART1_Start(u8 Res_char)
{
	USART1_RX_BUF[USART1_RX_STA&0XFF]=Res_char;							
	USART1_RX_STA++;
	USART1_RX_STA |= 0x4000;												
}


static void USART1_Res_data(u8 Res_char)
{
	USART1_RX_BUF[USART1_RX_STA&0XFF]=Res_char;								
	USART1_RX_STA++;
	if((USART1_RX_STA&0XFF)==11)							
	{
		USART1_RX_STA |= 0x2000;											
	}
}


uint32_t  laster_backword=0;
static void USART1_End(u8 Res_char)
{
	    uint8_t  laster[7]={0};
			if(USART1_RX_BUF[3]==0x45)
			{
					clean_rebuff_USART1();
			}
			else 
			{
					laster[0] = USART1_RX_BUF[3] - 0x30;
				  laster[1] = USART1_RX_BUF[4] - 0x30;
				  laster[2] = USART1_RX_BUF[5] - 0x30;
				  laster[3] = USART1_RX_BUF[7] - 0x30;
				 	laster[4] = USART1_RX_BUF[8] - 0x30;
				  laster[5] = USART1_RX_BUF[9] - 0x30;
				  laster[6] = USART1_RX_BUF[10] - 0x30;
				   
				  laster_backword = (laster[0]*1000000 + laster[1]*100000 + laster[2]*10000 \
														+laster[3]*1000+ laster[4]*100+ laster[5]*10+laster[6])-50;
				  clean_rebuff_USART1();
				   
			}
			
}



/**
  * 函数功能: 清空缓冲区
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void clean_rebuff_USART1(void)
{
  uint16_t i=USART1_MAX_RECV_LEN+1;
	while(i)
	USART1_RX_BUF[--i]=0;
	USART1_RX_STA=0;
}



void laster_backward_control_set(void)
{	
	   for(int i=0;i<3;i++)
		 {
			UASRT1_Send_Buf(continuous_tmp_buff,0,4);//设置为连续发送模式
			delay_ms(10);
		 }	
}


