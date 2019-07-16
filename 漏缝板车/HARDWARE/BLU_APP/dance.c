#include "dance.h"

#define  DANCE_LOVELY 	 0x01
#define  DANCE_PRIDE	 0x02
#define  DANCE_SURPRISED 0x03
#define  DANCE_NEVERS1	 0x04
#define  DANCE_HAPPPY1	 0x05
#define  DANCE_ANGRY1	 0x06
#define  DANCE_NEVERS2	 0x07
#define  DANCE_HAPPPY2	 0x08
#define  DANCE_ANGRY2	 0x09
#define  DANCE_GARGLE	 0x10

volatile bool flag = 0;
volatile uint8_t  shakeparam = SHAKEMAXE;//ҡ�η��ȱ�������
volatile uint8_t  Dance_Flag = 0x04;	 //�����־�����������赸����
volatile uint16_t Dance_BufLength=0;	 //��¼�Զ����赸���ݳ���
volatile uint16_t fast_step=0;
volatile uint16_t slow_step=0;
volatile uint16_t custom_step=0;
volatile uint16_t time_count=0;

const short    FastDanceData_angle[10]={  30, -60,  60, -30, 360,-360,  30, -60,  60, -30};
const short    FastDanceData_Move[10] ={   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};
const uint16_t FastDanceData_Time[10] ={   1,   1,   1,   1,   1,   1,   1,   1,   1,   1};
const uint8_t  FastDanceData_TurnV[10]={ 80, 80, 80, 80, 75, 75, 80, 80, 80, 80};
const uint8_t  FastDanceData_MoveV[10]={  25,  25,  25,  25,  25,  25,  25,  25,  25,  25};
const uint8_t  FastDanceData_Sound[10]={   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}; 

const short    SlowDanceData_angle[4] ={360,-360,-360,360};
const short    SlowDanceData_Move[4]  ={  0,   0,   0,  0};
const uint16_t SlowDanceData_Time[4]  ={  5,   5,   5,  5};
const uint8_t  SlowDanceData_TurnV[4] ={ 35,  35,  35, 35};
const uint8_t  SlowDanceData_MoveV[4] ={ 25,  25,  25, 25};
const uint8_t  SlowDanceData_Sound[4] ={  0,   0,   0,  0};
const uint8_t  SlowDanceData_Mode[4]  ={  1,   0,   1,  0};

static short    Dance_Angle[100] = {0};//�Զ����赸�����Ƕ�
static short    Dance_Move[100]  = {0};//�Զ����赸��������
static uint16_t Dance_Time[100]  = {0};//�Զ����赸����ʱ��
static uint8_t  Dance_TurnV[100] = {0};//�Զ����赸����ת��
static uint8_t  Dance_MoveV[100] = {0};//�Զ����赸�����ٶ�
static uint8_t  Dance_Sound[100] = {0};//�Զ����赸������Ч




/*********************************************************************************************************
** Function name:       Dance_Mode
** Descriptions:        ����ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void Dance_Mode(void)
{
	if(DANCE == Car_Mode)
	{
		switch(Dance_Flag)
		{
			case DANCE_FAST:	Fast_Dance();break;
			case DANCE_SLOW:	Slow_Dance();break;
			case DANCE_CUSTOM:	Custom_Dance();break;
			case DANCE_STOP:	Control_flag.Qian =0;Control_flag.Hou =0;Control_flag.Left =0;Control_flag.Right =0;break;
			case DANCE_SHAKE:	FrontBack_Shake();break;
			default:
				break;
		}
	}
}

/*********************************************************************************************************
** Function name:       Dance_Clear
** Descriptions:        ���õ�ǰ����ģʽ״̬��������Զ������裬��ôͬʱ�����ݽ��д���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Dance_Clear(void)
{
	sound_update= true;
	m_sound_type=STOP_SOUND;
	slow_dance_time=0;
	fast_dance_time=0;
	fast_step=0;
	slow_step=0;
	custom_step=0;
	
	//�������ģʽ����
	Control_flag.Qian   = 0;	// ǰ����־
	Control_flag.Hou    = 0;	// ���˱�־
	Control_flag.Left   = 0;	// ��ת��־
	Control_flag.Right  = 0;	// ��ת��־
	Control_flag.Stop   = 0;	// ֹͣ��־
	Control_flag.TurnV  = 0;	// ת�ٱ�־
	Control_flag.Acce   = 0;	// ���ٱ�־
	//���㻥��ģʽ����
	ActionControl_TurnAngle = 0;	//�ڵ�������ִ������ת�Ƕ�
	ActionControl_MoveDistance = 0;	//�ڵ�������ִ�����ƶ�����
	ActionControl_StayTime = 0;		//�ڵ�������ִ����ͣ��ʱ��
	flag=0;

}
/*********************************************************************************************************
** Function name:       Set_Dance
** Descriptions:        ���õ�ǰ����ģʽ״̬��������Զ������裬��ôͬʱ�����ݽ��д���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Set_Dance(void)
{
	//�жϵ�ǰ�Ƿ�������ģʽ
	if(DANCE == Car_Mode)
	{	
		Dance_Clear();
		switch(USART3_RX_BUF[4])
		{
			case DANCE_FAST:  Dance_Flag = 0x01;Send_OK();break;//������赸
			case DANCE_SLOW:  Dance_Flag = 0x02;Send_OK();break;//�������赸
			case DANCE_CUSTOM:Dance_BufLength=Dance_DP();Dance_Flag = 0x03;Send_OK();break;//�Զ����赸��ͬʱ�����ݽ��д���
			case DANCE_STOP:  Dance_Flag = 0x04;break;
			default:
				Send_ERROR();//���յ�������Ϣ
				break;
		}
	}
}


/*********************************************************************************************************
** Function name:       Dance_DP
** Descriptions:        �Զ����������ݴ���  
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
u8 Dance_DP(void)
{
	u8 Dance_numb = 0;
	u8 n = 0;
	u8 i = 5;
	
	//���֮ǰ�Ļ�������
	short_clear_buf(Dance_Angle,0,Dance_BufLength);
	short_clear_buf(Dance_Move, 0,Dance_BufLength);
	short_clear_buf((short*)Dance_Time, 0,Dance_BufLength);
	u8_clear_buf(Dance_TurnV, 0,Dance_BufLength);
	u8_clear_buf(Dance_MoveV, 0,Dance_BufLength);
	u8_clear_buf(Dance_Sound, 0,Dance_BufLength);
	
	
	for(Dance_numb = ((USART3_RX_STA&0x00FF)-8);Dance_numb>0;)
	{
		Dance_Angle[n] = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];//��������
		i += 2;
		Dance_Move[n]  = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];//��������
		i += 2;
		Dance_Time[n]  = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];//ʱ������
		i += 2;
		if(USART3_RX_BUF[i]<20)USART3_RX_BUF[i]=15;//���������ٶȣ���ֹת����
		Dance_TurnV[n] =  USART3_RX_BUF[i];//ת������	
		i++;
		Dance_MoveV[n] =  USART3_RX_BUF[i];//�ٶ�����
		i++;
		Dance_Sound[n] =  USART3_RX_BUF[i];//��Ч����
		i++;
		n++;
		Dance_numb -= 9;
	}
	
	return n;
}


/*********************************************************************************************************
** Function name:       Custom_Dance
** Descriptions:        �������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Custom_Dance(void)
{
	if(custom_step >= Dance_BufLength)
	{
		custom_step = 0;
		flag=0;
	}
	
	if(!flag)
	{
	
		//����С��������Ч
	    switch (Dance_Sound[custom_step])
		{
			case	DANCE_LOVELY: sound_update= true; m_sound_type=BACK_LOVELY_SOUND;
				break;
			case	DANCE_PRIDE:sound_update= true; m_sound_type=BACK_PRIDE_SOUND;
				break;
			case    DANCE_SURPRISED:sound_update= true; m_sound_type=FORN_SURPRISED_SOUND;
				break;
			case	DANCE_NEVERS1:sound_update= true; m_sound_type=RIGHT_NEVERS_SOUND;
				break;
			case	DANCE_HAPPPY1:sound_update= true; m_sound_type=RIGHT_HAPPY_SOUND;
				break;
			case    DANCE_ANGRY1:sound_update= true; m_sound_type=RIGHT_ANGRY_SOUND;
				break;
			case	DANCE_NEVERS2:sound_update= true; m_sound_type=LEFT_NEVERS_SOUND;
				break;
			case	DANCE_HAPPPY2:sound_update= true; m_sound_type=LEFT_HAPPY_SOUND;
				break;
			case    DANCE_ANGRY2:sound_update= true; m_sound_type=LEFT_ANGERY_SOUND;
				break;
			case	DANCE_GARGLE:sound_update= true; m_sound_type=BACK_LOVELY_SOUND;
				break;
			default:
				break;
		}
		
		
		//����ת���ٶ�
		Control_flag.TurnV = Dance_TurnV[custom_step];
		//�����ƶ��ٶ�
		Control_flag.Acce  = Dance_MoveV[custom_step];
		//ÿ������ִֻ��һ��
		flag = 1;
	}
	
	//ִ�е�������
	Single_Action(Dance_Angle[custom_step],Dance_Move[custom_step],Dance_Time[custom_step]);//��������
	
	//�жϵ��������Ƿ����
	if(Control_flag.SingleAction)
	{
		//�ƶ�����һ������ִ��
		custom_step++;
		//����������ɱ�־����
		Control_flag.SingleAction = 0;
		flag = 0;
	}
	
}

void Fast_Dance(void)
{
	
	if(fast_step >= 10)
	{
		fast_step = 0;
		flag = 0;
	}	
	
	if(fast_dance_time == 0)
	{
		sound_update	= 	true;
		m_sound_type	=	CHEICKET_SOUND;
		fast_dance_time=6500;
	}
	
	
		 static u8 Flag_Dance_fast_avoid=0;
		 if(Distance<90)
		 {
			 //�������ģʽ����
			Control_flag.Qian   = 0;	// ǰ����־
			Control_flag.Hou    = 0;	// ���˱�־
			Control_flag.Left   = 0;	// ��ת��־
			Control_flag.Right  = 0;	// ��ת��־
			Flag_Dance_fast_avoid=1;
		 }	
		if(Flag_Dance_fast_avoid == 1)
		 {
			Single_Action(0,-50,0);if(Control_flag.SingleAction){Flag_Dance_fast_avoid=2;Control_flag.SingleAction = 0;}
		 } 
		 else if(Flag_Dance_fast_avoid == 2)
		 {
			Single_Action(170,0,0);if(Control_flag.SingleAction){Flag_Dance_fast_avoid=0;Control_flag.SingleAction = 0;}
		 }
		 else 
		 {	
			Flag_Dance_fast_avoid =0;
		 }
	
	if(Flag_Dance_fast_avoid==0)
	{
		if(!flag)
		{
			//����С��������Ч
			//����ת���ٶ�
			Control_flag.TurnV = FastDanceData_TurnV[fast_step];
			//�����ƶ��ٶ�
			Control_flag.Acce  = FastDanceData_MoveV[fast_step];
			//ÿ������ִֻ��һ��
			flag = 1;
		}
		
		//ִ�е�������
		Single_Action(FastDanceData_angle[fast_step],FastDanceData_Move[fast_step],FastDanceData_Time[fast_step]);//��������
		
		//�жϵ��������Ƿ����
		if(Control_flag.SingleAction)
		{
			//�ƶ�����һ������ִ��
			fast_step++;
			//����������ɱ�־����
			Control_flag.SingleAction = 0;
			flag = 0;
		}
	}
	
}


void Slow_Dance(void)
{	
	if(slow_step >= 4)
	{
		slow_step = 0;
		flag = 0;
	}
	
	 if(slow_dance_time == 0)
	{
		sound_update	= 	true;
		m_sound_type	=	BLUE_RIVER_SOUND;
		slow_dance_time=6500;
	}
	
	 static u8 Flag_Dance_slow_avoid=0;
		 if(Distance<90)
		 {
			 //�������ģʽ����
			Control_flag.Qian   = 0;	// ǰ����־
			Control_flag.Hou    = 0;	// ���˱�־
			Control_flag.Left   = 0;	// ��ת��־
			Control_flag.Right  = 0;	// ��ת��־
			Flag_Dance_slow_avoid=1;
		 }	
		if(Flag_Dance_slow_avoid == 1)
		 {
			Single_Action(0,-50,0);if(Control_flag.SingleAction){Flag_Dance_slow_avoid=2;Control_flag.SingleAction = 0;}
		 } 
		 else if(Flag_Dance_slow_avoid == 2)
		 {
			Single_Action(170,0,0);if(Control_flag.SingleAction){Flag_Dance_slow_avoid=0;Control_flag.SingleAction = 0;}
		 }
		 else 
		 {	
			Flag_Dance_slow_avoid =0;
		 }
	
	if(Flag_Dance_slow_avoid==0)
	{
		if(!flag)
		{
			//����С��������Ч
			
			//����ת���ٶ�
			Control_flag.TurnV = SlowDanceData_TurnV[slow_step];
			//�����ƶ��ٶ�
			Control_flag.Acce  = SlowDanceData_MoveV[slow_step];
			//ÿ������ִֻ��һ��
			flag = 1;
		}
		
		//ִ�е�������
		Single_Action_mode(SlowDanceData_angle[slow_step],SlowDanceData_Move[slow_step],SlowDanceData_Time[slow_step],SlowDanceData_Mode[slow_step]);//��������
		
		//�жϵ��������Ƿ����
		if(Control_flag.SingleAction)
		{
			//�ƶ�����һ������ִ��
			slow_step++;
			//����������ɱ�־����
			Control_flag.SingleAction = 0;
			flag = 0;
		}
	}	
}

/*���������ֵ0��5*/
uint8_t Random_Numb(void)
{
	uint16_t numb;
	
	numb = rand();
	
	srand(numb);
	
	return numb%6;
}

/*ҡ��*/
void FrontBack_Shake(void)
{
	static uint16_t delay;
	static uint8_t turn_flag;
	static uint8_t count_flag;
	static uint8_t shake_count;//ҡ�δ���
	static uint16_t count;
	
	
	/*��һ�ν�����ʼ���ڲ�����*/
	if(time_count==0)
	{
		turn_flag  = 0;
		count_flag = 1;
		count = 0;
		shake_count=0;
		delay=50;
	}
	
	Random_Numb();
	
	if(time_count<300)
	{
		time_count++;
	}
	else{
		count++;
		switch(count_flag)
		{
			case 0x01:	BalancePoint = ZHONGZHI+shakeparam;if(count==50){count_flag=2;count=0;shake_count++;}break;
			case 0x02:	BalancePoint = ZHONGZHI-shakeparam;if(count==50){count_flag=1;count=0;if(shake_count>3){shake_count=0;count_flag=3;}}break;
			case 0x03:	BalancePoint = ZHONGZHI;           if(count==delay){count_flag=4;count=0;turn_flag=Random_Numb();Control_flag.TurnV=50;}break;
			case 0x04:	{
							switch(turn_flag)
							{
								case 0x00:	Single_Action(  60,0,0);break;//��ת60��
								case 0x01:	Single_Action( -60,0,0);break;//��ת60��
								case 0x02:	Single_Action(  90,0,0);break;//��ת90��
								case 0x03:	Single_Action( -90,0,0);break;//��ת90��
								case 0x04:	Single_Action( 120,0,0);break;//��ת120��
								case 0x05:	Single_Action(-120,0,0);break;//��ת120��
								default:
									break;
							}
							/*�жϵ��������Ƿ����*/
							if(Control_flag.SingleAction)
							{
								Control_flag.SingleAction = 0;
								count_flag = 5;
								count=0;
								switch(Random_Numb()%3)
								{
									case 0x00:	delay =  20;break;//��
									case 0x01:	delay =  50;break;//��
									case 0x02:	delay = 100;break;//��
									default:
										break;
								}
							}
						}break;
			case 0x05:	BalancePoint = ZHONGZHI;if(count == delay){count_flag=1;count=0;}break;//ͣ��
			default:
				break;
		}
	}
}



/********************************************END_DANCE_C_FILE********************************************/


