#include "piddebug.h"

//PID参数调节
float PID_Buffer[6]={0};
u16	PID_int_Flag=0;
/*********************************************************************************************************
** Function name:       PID_Change
** Descriptions:        转换成float型数据函数（专用于Set_PID函数）
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
union data
{
	char   data[4];
	float  number;
}NUM;

static float PID_Change(void)
{
	NUM.data[0] = USART3_RX_BUF[5];
	NUM.data[1] = USART3_RX_BUF[6];
	NUM.data[2] = USART3_RX_BUF[7];
	NUM.data[3] = USART3_RX_BUF[8];
	return NUM.number;
}



/*********************************************************************************************************
** Function name:       Set_PID
** Descriptions:        设置平衡小车PID参数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_PID(void)
{
	if((USART3_RX_STA&0xFF)==12)
	{
		if(0<USART3_RX_BUF[4]&&USART3_RX_BUF[4]<8)
		{
			switch(USART3_RX_BUF[4])
			{
				case 0x01:PID_Buffer[0]=PID_Change();Send_OK();break;//直立环P
				case 0x02:PID_Buffer[1]=PID_Change();Send_OK();break;//直立环D
				case 0x03:PID_Buffer[2]=PID_Change();Send_OK();break;//速度环P
				case 0x04:PID_Buffer[3]=PID_Change();Send_OK();break;//速度环I
				case 0x05:PID_Buffer[4]=PID_Change();Send_OK();break;//转向环P
				case 0x06:PID_Buffer[5]=PID_Change();Send_OK();break;//转向环D
				case 0x07:STMFLASH_Write(0X08070000,(u16*)PID_Buffer,sizeof(PID_Buffer));
						 PID_int_Flag=0xC0;STMFLASH_Write(0X08078000,&PID_int_Flag,sizeof(PID_int_Flag));
						 NVIC_SystemReset();break;//重启系统
			}
		}
		else{
			//接收到错误信息
			Send_ERROR();
		}
	}
	else{
		//接收到错误信息
		Send_ERROR();
	}
}

