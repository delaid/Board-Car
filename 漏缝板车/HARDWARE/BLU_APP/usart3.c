#include "usart3.h"

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//static u8 adress_buff[5]={0xFA,0x04,0x01,0x80,0x81};	
//static u8 rang_buff[5]={0XFA,0X04,0X09,0X05,0xF4};	
//static u8 fre_buff[5] = {0xFA,0x04,0x0A,0x00,0xF8};
//static u8 ratio_buff[5] = {0xFA,0x04,0x0C,0x02,0xF4};
//static u8 open_buff[5] ={0xFA,0x04,0x0D,0x01,0xF4};
//static u8 continuous_top_buff[5]={0XFA,0X04,0X08,0X01,0xF9};	//������������	

static u8 continuous_tmp_buff[4]={0x80,0x06,0x03,0x77};	//������������

//static u8 stop_tmp_buff[4]={0XFA,0X06,0X02,0XFE};				//ֹͣ��������

static void USART3_Free(u8 Res_char);
static void USART3_Start(u8 Res_char);
static void USART3_Res_data(u8 Res_char);
static void USART3_End(u8 Res_char);
//static void USART3_Data_Check(void);
static void clean_rebuff_usart3(void);
static void UASRT3_Send_Buf(u8 *buf, u16 start_number, u16 length);
/*********************************************************************************************************
** Function name:       usart3_init
** Descriptions:        Ŀ��壨STM32F103ZET6����ʼ��USART3
** input parameters:    bound:���ڲ�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usart3_init(u32 bound)
{
	
	
	//GPIO�˿�����
	GPIO_InitTypeDef 	GPIO_InitStructure;						//GPIO�ṹ��
	USART_InitTypeDef 	USART_InitStructure;					//USART�ṹ��
	NVIC_InitTypeDef 	NVIC_InitStructure;						//NVIC�ṹ��
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//??USART3,GPIOB??
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//??USART3,GPIOB??
    USART_DeInit(USART3);  
    //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    //USART3_RX	  PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
 
    //Usart1 NVIC ??
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//?????3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
    NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
 
    //USART ?????
    USART_InitStructure.USART_BaudRate = bound;//?????115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
    USART_InitStructure.USART_Parity = USART_Parity_No;//??????
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????
    USART_Init(USART3, &USART_InitStructure); //?????
 
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//??USART3????
    USART_Cmd(USART3, ENABLE);                    //????
}

u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 							//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
/*********************************************************************************************************
** Function name:       u3_printf
** Descriptions:        USART3�������ʽ���printf�����ʽ����ȷ��һ�η������ݲ�����USART_MAX_SEND_LEN�ֽ�
** input parameters:    fmt���ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);						//�˴η������ݵĳ���
	for(j=0;j<i;j++)											//ѭ����������
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 	//ѭ������,ֱ���������   
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}


/*********************************************************************************************************
** Function name:       UASRT2_Send_Buf
** Descriptions:        USAR2??????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
static void UASRT3_Send_Buf(u8 *buf, u16 start_number, u16 length)
{
	u16 j;
	for(j=start_number;j<length;j++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);		
		USART_SendData(USART3,buf[j]);
	}
}


volatile  u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 
u16 USART3_RX_STA=0;     

void USART3_IRQHandler(void)
{
	
u8 Res;	
#if SYSTEM_SUPPORT_OS 														// ���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  					// �����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		
		  Res =USART_ReceiveData(USART3);										// ��ȡ���յ�������
		  u8 status = USART3_RX_STA >> 8;
		  switch (status)
			{

				case 0x00:USART3_Free(Res);		break;
				case 0x80:USART3_Start(Res);	break;
				case 0xC0:USART3_Res_data(Res);	break;
				case 0xE0:USART3_End(Res);		break;
				default:
				break;
			}

	}
	
#if SYSTEM_SUPPORT_OS 														// ���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}



static void USART3_Free(u8 Res_char)
{
	u8 star_sign = Res_char;
	if(star_sign ==  0x80)													
	{
		USART3_RX_BUF[USART3_RX_STA&0XFF]=Res_char;					
		USART3_RX_STA++;
		USART3_RX_STA |= 0x8000;											
	}	
}




static void USART3_Start(u8 Res_char)
{
	USART3_RX_BUF[USART3_RX_STA&0XFF]=Res_char;							
	USART3_RX_STA++;
	USART3_RX_STA |= 0x4000;												
}


static void USART3_Res_data(u8 Res_char)
{
	USART3_RX_BUF[USART3_RX_STA&0XFF]=Res_char;								
	USART3_RX_STA++;
	if((USART3_RX_STA&0XFF)==11)							
	{
		USART3_RX_STA |= 0x2000;											
	}
}


uint32_t  laster_forword=0;
static void USART3_End(u8 Res_char)
{
	    uint8_t  laster[7]={0};
			if(USART3_RX_BUF[3]==0x45)
			{
					clean_rebuff_usart3();
			}
			else 
			{
					laster[0] = USART3_RX_BUF[3] - 0x30;
				  laster[1] = USART3_RX_BUF[4] - 0x30;
				  laster[2] = USART3_RX_BUF[5] - 0x30;
				  laster[3] = USART3_RX_BUF[7] - 0x30;
				 	laster[4] = USART3_RX_BUF[8] - 0x30;
				  laster[5] = USART3_RX_BUF[9] - 0x30;
				  laster[6] = USART3_RX_BUF[10] - 0x30;
				   
				  laster_forword = laster[0]*1000000 + laster[1]*100000 + laster[2]*10000 \
														+laster[3]*1000+ laster[4]*100+ laster[5]*10+laster[6];
				  clean_rebuff_usart3();				   
			}
			
}



/**
  * ��������: ��ջ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void clean_rebuff_usart3(void)
{
  uint16_t i=USART3_MAX_RECV_LEN+1;
	while(i)
		USART3_RX_BUF[--i]=0;
	USART3_RX_STA=0;
}



void laster_forward_control_set(void)
{		
	   for(int i=0;i<3;i++)
		 {
			UASRT3_Send_Buf(continuous_tmp_buff,0,4);//����Ϊ��������ģʽ
			delay_ms(50);
		 }	

}

