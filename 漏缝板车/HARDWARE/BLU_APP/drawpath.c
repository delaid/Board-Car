#include "drawpath.h"



u8 Path_BufLength;						// ��·��ģʽ�½��յ����ݳ���
u8 Path_Data_Update;					// ·�����ݸ��±�־
u8 usddflag = 0;						// ·�����ݵߵ���־
volatile u8 pdwtflag = 0;				// ·�����ݵȴ���־

short Angle[150]={0};					// �洢·���Ƕ�
short Move[150]={0}; 					// �洢·������
/*********************************************************************************************************
** Function name:       DrawPath_Mode
** Descriptions:        ��ǰ���ڻ�·��ģʽ���Ի�·��Э�����ݽ��д���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DrawPath_Mode(void)
{
	static u8 i = 0;
	static u8 time  = 0;
	static u8 times = 0;
	
	if(DRAWPATH == Car_Mode)
	{
		if(Path_Data_Update)//�ж�·�������Ƿ����
		{
			i=0;
			Control_flag.Qian	 =0;
			Control_flag.Hou	 =0;
			Control_flag.Left	 =0;
			Control_flag.Right	 =0;
			Control_flag.Angle	 =0;
			Control_flag.Stop_Time =0;
			Control_flag.Distance=0;
			ActionControl_TurnAngle = 0;
			ActionControl_MoveDistance = 0;
			ActionControl_StayTime = 0;
			Control_flag.TurnV= 55;
			Control_flag.Acce = 55;
			Path_Data_Update = 0;
			
		}
		
		static u8 Flag_Draw_avoid=0;
		static u8 Draw_Path_count=0;
	    static u16 Go_bostaclecount=0;

		time++;
		
		 if(Distance<80)
		 {
			 //�������ģʽ����
			Control_flag.Qian   = 0;	// ǰ����־
			Control_flag.Hou    = 0;	// ���˱�־
			Control_flag.Left   = 0;	// ��ת��־
			Control_flag.Right  = 0;	// ��ת��־
			Control_flag.Stop   = 0;	// ֹͣ��־
			times++;
			//Flag_Draw_avoid=1;
			Draw_Path_count++;
		 }	
		 
		 if(time<20)
		 {
			 if(times>10)
				 Flag_Draw_avoid=1;
		 }
		 else{
			 time  = 0;
			 times = 0;
		 }

		if(Flag_Draw_avoid == 1)
		 {
			Single_Action(0,-10,0);if(Control_flag.SingleAction){Flag_Draw_avoid=2;Control_flag.SingleAction = 0;}
		 } 
		 else if(Flag_Draw_avoid == 2)
		 {
			if(Draw_Path_count%3==0){
			Single_Action(45,0,0);if(Control_flag.SingleAction){Flag_Draw_avoid=0;Control_flag.SingleAction = 0;}
		    }
			else{
			Single_Action(170,0,0);if(Control_flag.SingleAction){Flag_Draw_avoid=0;Control_flag.SingleAction = 0;}
		    }
		 }
		 else 
		 {	
			Flag_Draw_avoid=0;
		 }
		
		if(Flag_Draw_avoid==0)
		{
			
			
			if(Path_BufLength!=0 && (Encoder_Left==0 && Encoder_Right==0)){
			
				if( (Encoder_Left==0 && Encoder_Right==0) )
				{
					Go_bostaclecount++;
					if(Go_bostaclecount>100)
					{
					   Go_bostaclecount=0;
						Flag_Draw_avoid=1;
					}
				}
				else
				{
					  Go_bostaclecount=0;
				}
					
			}
#if 0			
			//�ж�·�������Ƿ������
			if(pdwtflag)
			{
				if(1 == usddflag)
				{
					if(0==i)
					{
						Single_Action(180,Move[i],0);		//��������	
					}
					else
					{
						Single_Action(Angle[i],Move[i],0);	//��������	
					}
					
					if(Control_flag.SingleAction)//�жϵ��������Ƿ����
					{
						i++;
						Control_flag.SingleAction = 0;
					}
				}
				else
				{
					Single_Action(Angle[i],Move[i],0);	//��������	
					if(Control_flag.SingleAction)		//�жϵ��������Ƿ����
					{
						i++;
						Control_flag.SingleAction = 0;
					}
				}
				
				if(i>=(Path_BufLength/2))//�ж��Ƿ������·��
				{
#if 0				
					if(Path_BufLength!=0)
					{
						m_sound_type=END_SOUND;sound_update=true;
					}
					//���֮ǰ�Ļ���
					memset(Angle,0,200);
					memset(Move,0,200);
					//���ݳ��ȹ���
					Path_BufLength = 0;
					i=0;
#endif
					//·�����ݵߵ�
					DrawPath_UpsideDown();
					i=0;
					usddflag = 1;
				}
			}
#endif
			
#if 1
			Single_Action(Angle[i],Move[i],0);	//��������	
			if(Control_flag.SingleAction)		//�жϵ��������Ƿ����
			{
				i++;
				Control_flag.SingleAction = 0;
			}
			
			if(i>=(Path_BufLength/2))			//�ж��Ƿ������·��
			{
				
				if(Path_BufLength!=0)
				{
					m_sound_type=END_SOUND;sound_update=true;
				}
				//���֮ǰ�Ļ���
				memset(Angle,0,200);
				memset(Move,0,200);
				//���ݳ��ȹ���
				Path_BufLength = 0;
				i=0;
			}
#endif		
	    }
	}
}

/*********************************************************************************************************
** Function name:       Set_DrawPath
** Descriptions:        ��������·������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Set_DrawPath(void)
{
	//�жϵ�ǰ�Ƿ��ڻ�·��ģʽ
	if(DRAWPATH == Car_Mode)
	{

		Path_Data_Update = 1;
		memset(Angle,0,200);
		memset(Move,0,200);
		Path_BufLength = DrawPath_DP();
		Send_OK();
	}
}


/*********************************************************************************************************
** Function name:       Read_DrawPath
** Descriptions:        ��ȡ·��״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Read_DrawPath(void)
{
	
}


/*********************************************************************************************************
** Function name:       DrawPath_DP
** Descriptions:        ��ǰ���ڻ�·��ģʽ����ȡuart3�Ľ��ջ������ݲ�����ת��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
u8 DrawPath_DP(void)
{
	u8 Path_numb = 0;
	u8 n = 0;
	u8 i = 4;
	u8 angle = 0;
	u8 move  = 0;
	short Path_data = 0;
	pdwtflag = 1;
	for(Path_numb = ((USART3_RX_STA&0x00FF)-7); Path_numb>0;)
	{
		Path_data = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];
		n++;
		i += 2;
		Path_numb -= 2;
		if(n%2)
		{
			Angle[angle] = -Path_data;
			angle++;
		}
		else{
			Move[move] = Path_data;
			move++;
		}
	}
	
	USART3_RX_STA = 0;
	return n;
}

//·���滮���ݵߵ�
void DrawPath_UpsideDown(void)
{
	uint8_t	n=0;
	uint8_t	m=(Path_BufLength/2)-1;
	uint8_t i=(Path_BufLength/2)/2;
	short buff[2];
	
	for(n=0;n<i;)
	{
		buff[0]  = -Angle[n];
		Angle[n] = -Angle[m];
		Angle[m] = buff[0];
		buff[1]  = Move[n];
		Move[n]  = Move[m];
		Move[m]  = buff[1];
		m--;
		n++;
	}
}
