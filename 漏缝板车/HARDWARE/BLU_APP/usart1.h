#ifndef __USART1_H
#define __USART1_H
#include "sys.h"
#include <stdarg.h>
#include <string.h>

#define USART1_MAX_SEND_LEN		25 					//����ͻ����ֽ���
#define USART1_MAX_RECV_LEN		25 				//�����ջ����ֽ���
extern uint32_t laster_backword;
extern  void laster_backward_control_set(void);
/*********************************************************************************************************
** Function name:       usart3_init
** Descriptions:        Ŀ��壨STM32F103ZET6����ʼ��USART3
** input parameters:    bound:���ڲ�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usart1_init(u32 bound);
/*********************************************************************************************************
** Function name:       u3_printf
** Descriptions:        USART3�������ʽ���printf�����ʽ����ȷ��һ�η������ݲ�����USART_MAX_SEND_LEN�ֽ�
** input parameters:    fmt���ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void u1_printf(char* fmt,...);
/*********************************************************************************************************
** Function name:       crc16_build
** Descriptions:        ����crcУ��������
** input parameters:    data_ptr����ҪУ�������������׵�ַ
						data_length: ��ҪУ������ݳ���
** output parameters:   ��
** Returned value:      CRCУ����
*********************************************************************************************************/
#endif
