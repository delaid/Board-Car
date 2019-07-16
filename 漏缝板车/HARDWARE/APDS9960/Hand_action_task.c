#include  "Hand_action_task.h"

//APDS9960检测任务
#define APDS9960_TASK_PRIO				3					//设置任务优先级
#define APDS9960_STK_SIZE				128					//设置任务堆栈大小
	
OS_STK APDS9960_TASK_STK[APDS9960_STK_SIZE]; 	//任务堆栈
void apds9960_task(void *pdata);							//任务函数


void Hand_action_task_Create(void)
{
	OSTaskCreate(apds9960_task,(void *)0,(OS_STK*)&APDS9960_TASK_STK[APDS9960_STK_SIZE-1],APDS9960_TASK_PRIO);	//创建APDS9960测试任务
}


/*********************************************************************************************************
** Function name:       void apds9960_task(void *pdata)
** Descriptions:        手势检测的任务，由于读取手势运行相应的算法较长，所以需要给手势设定一个独立任务
						通过手势传感器
						并设定对应的电压值，发送对应电量信号给灯和APP以及开关进行通讯。
** input parameters:    pdata: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void apds9960_task(void *pdata)
{
	while(1)
	{	
		APDS_SCAN();
		APDS_Mode();
		delay_ms(20);
	}
}





