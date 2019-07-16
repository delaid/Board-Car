#include "usart1.h"

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

static void USART1_Free(u8 Res_char);
static void USART1_Start(u8 Res_char);
static void USART1_Res_data(u8 Res_char);
static void USART1_End(u8 Res_char);
//static void USART1_Data_Check(void);
static void clean_rebuff_USART1(void);
static void UASRT1_Send_Buf(u8 *buf, u16 start_number, u16 length);
static u8 continuous_tmp_buff[4]={0x80,0x06,0x03,0x77};	//������������
/*********************************************************************************************************
** Function name:       USART1_init
** Descriptions:        Ŀ��壨STM32F103ZET6����ʼ��USART1
** input parameters:    bound:���ڲ�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usart1_init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 							//���ͻ���,���USART1_MAX_SEND_LEN�ֽ�

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
** Descriptions:        USART1�������ʽ���printf�����ʽ����ȷ��һ�η������ݲ�����USART_MAX_SEND_LEN�ֽ�
** input parameters:    fmt���ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void u1_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);						//�˴η������ݵĳ���
	for(j=0;j<i;j++)											//ѭ����������
	{
	  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 	//ѭ������,ֱ���������   
		USART_SendData(USART1,USART1_TX_BUF[j]); 
	} 
}

volatile  u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 
u16 USART1_RX_STA=0;  

void USART1_IRQHandler(void)
{
	 char Res;
   u8 status;
#if SYSTEM_SUPPORT_OS 														// ���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  					// �����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		  Res =USART_ReceiveData(USART1);										// ��ȡ���յ�������
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
	
#if SYSTEM_SUPPORT_OS 														// ���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
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
  * ��������: ��ջ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
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
			UASRT1_Send_Buf(continuous_tmp_buff,0,4);//����Ϊ��������ģʽ
			delay_ms(10);
		 }	
}


