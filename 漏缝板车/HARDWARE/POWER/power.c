#include "power.h"
#define POWER_TASK_PRIO					6					//设置触摸任务优先级
#define POWER_STK_SIZE					64					//设置触摸任务堆栈大小

OS_STK POWER_TASK_STK[POWER_STK_SIZE]; 			  		//触摸任务堆栈
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
		if(m_socket_volt>5000)//大于5V表示有插座插入
		{
			Control_flag.Stop=1;
			socket_swtich=true;
		}
		else
		{
			socket_swtich = false;
		}
		
		if(m_bat_volt>7000)//大于7V表示取消低压提示
		{
			power_low=false;
			if(m_bat_volt>7500)//大于7.5V表示取消关机标志
			power_off=false;
			count=0;
			count_off=0;
		}
		else if(m_bat_volt<6500 && m_bat_volt>6300 && power_off!=true )//低于6.5V，高于6.3V，低电压提示power_low = true
		{
			count++;
			if(count>10)
			power_low = true;
			count_off=0;
		}
		else if(m_bat_volt<6300)//低于6.3V，关机条件成立 power_off=true
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




