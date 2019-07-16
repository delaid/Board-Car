#include "power.h"
#define POWER_TASK_PRIO					6					//���ô����������ȼ�
#define POWER_STK_SIZE					64					//���ô��������ջ��С

OS_STK POWER_TASK_STK[POWER_STK_SIZE]; 			  		//���������ջ
void power_task(void *pdata);

void Power_task_Create(void)
{   
	OSTaskCreate(power_task,(void*)0,(OS_STK*)&POWER_TASK_STK[POWER_STK_SIZE-1],POWER_TASK_PRIO);
}


volatile bool socket_swtich = false;
volatile bool power_low	=	false;
volatile bool power_off	=	false;

volatile uint16_t m_bat_volt=8000;

void power_task(void *pdata)
{
	static int m_socket_volt;
	static int count;
	static int count_off;
	while(1)
	{	
		m_bat_volt = Get_battery_volt();
		m_socket_volt = Get_socket_volt();
		if(m_socket_volt>5000)//����5V��ʾ�в�������
		{
			Control_flag.Stop=1;
			socket_swtich=true;
		}
		else
		{
			socket_swtich = false;
		}
		
		if(m_bat_volt>7000)//����7V��ʾȡ����ѹ��ʾ
		{
			power_low=false;
			if(m_bat_volt>7500)//����7.5V��ʾȡ���ػ���־
			power_off=false;
			count=0;
			count_off=0;
		}
		else if(m_bat_volt<6500 && m_bat_volt>6300 && power_off!=true )//����6.5V������6.3V���͵�ѹ��ʾpower_low = true
		{
			count++;
			if(count>10)
			power_low = true;
			count_off=0;
		}
		else if(m_bat_volt<6300)//����6.3V���ػ��������� power_off=true
		{
			count_off++;
			if(count_off>10)
			{
			  power_off=true;
			}
		}
		delay_ms(30);
	}	
}




