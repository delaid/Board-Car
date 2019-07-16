#include "ultra_task.h"
//超声波测距任务
#define ULTRA_TASK_PRIO				4					//设置任务优先级
#define ULTRA_STK_SIZE				128				//设置任务堆栈大小
	
OS_STK ULTRA_TASK_STK[ULTRA_STK_SIZE]; 	//任务堆栈
void ultra_task(void *pdata);							//任务函数


void Ultra_task_Create(void)
{
	OSTaskCreate(ultra_task,(void *)0,(OS_STK*)&ULTRA_TASK_STK[ULTRA_STK_SIZE-1],ULTRA_TASK_PRIO);	//创建APDS9960测试任务
}

void ultra_task(void *pdata)
{
	while(1)
	{
		Read_Distance();
		delay_ms(50); 
	}
}




