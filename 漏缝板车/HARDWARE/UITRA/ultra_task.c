#include "ultra_task.h"
//�������������
#define ULTRA_TASK_PRIO				4					//�����������ȼ�
#define ULTRA_STK_SIZE				128				//���������ջ��С
	
OS_STK ULTRA_TASK_STK[ULTRA_STK_SIZE]; 	//�����ջ
void ultra_task(void *pdata);							//������


void Ultra_task_Create(void)
{
	OSTaskCreate(ultra_task,(void *)0,(OS_STK*)&ULTRA_TASK_STK[ULTRA_STK_SIZE-1],ULTRA_TASK_PRIO);	//����APDS9960��������
}

void ultra_task(void *pdata)
{
	while(1)
	{
		Read_Distance();
		delay_ms(50); 
	}
}




