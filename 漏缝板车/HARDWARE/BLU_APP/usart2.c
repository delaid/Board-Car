#include "usart2.h"


////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

/*********************************************************************************************************
** Function name:       usart2_init
** Descriptions:        Ŀ��壨STM32F103VET6����ʼ��USAR2
** input parameters:    bound:���ڲ�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void usart2_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD| RCC_APB2Periph_AFIO, ENABLE);			//ʹ��GPIODʱ�Ӻ���ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);								//ʹ��USART3

	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);										//I/O����ӳ�俪��,��ӳ�䵽PD8��PD9����Ҫѡ����ȫ��ӳ��

	//USART2_TX   GPIOD.5
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5; 										//PD.5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;									//�����������
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//��ʼ��GPIOD.5

	//USART2_RX	  GPIOD.6
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;											//PD.6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;								//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//��ʼ��GPIOD.6  

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;							//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;									//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;										//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate   = bound;										//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity     = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure);											//��ʼ������2
	USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);										//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);															//ʹ�ܴ���2

}


/*********************************************************************************************************
** Function name:       UASRT2_Send_Buf
** Descriptions:        USAR2���ͻ�������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UASRT2_Send_Buf(u8 *buf, u16 start_number, u16 length)
{
	u16 j;
	for(j=start_number;j<length;j++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);			// ѭ������,ֱ���������
		USART_SendData(USART2,buf[j]);
	}
}



//С��������Ч����ָ��洢
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
** Descriptions:        ��Ч��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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








