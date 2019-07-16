#ifndef __USART1_H
#define __USART1_H
#include "sys.h"
#include <stdarg.h>
#include <string.h>

#define USART1_MAX_SEND_LEN		25 					//最大发送缓存字节数
#define USART1_MAX_RECV_LEN		25 				//最大接收缓存字节数
extern uint32_t laster_backword;
extern  void laster_backward_control_set(void);
/*********************************************************************************************************
** Function name:       usart3_init
** Descriptions:        目标板（STM32F103ZET6）初始化USART3
** input parameters:    bound:串口波特率
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void usart1_init(u32 bound);
/*********************************************************************************************************
** Function name:       u3_printf
** Descriptions:        USART3的输出格式变成printf输出格式，并确保一次发送数据不超过USART_MAX_SEND_LEN字节
** input parameters:    fmt：字符串指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void u1_printf(char* fmt,...);
/*********************************************************************************************************
** Function name:       crc16_build
** Descriptions:        数据crc校验码生成
** input parameters:    data_ptr：需要校验的数据数组的首地址
						data_length: 需要校验的数据长度
** output parameters:   无
** Returned value:      CRC校验码
*********************************************************************************************************/
#endif
