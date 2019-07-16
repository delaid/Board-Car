#ifndef __USART3_H
#define __USART3_H
#include "sys.h"
#include <stdarg.h>
#include <string.h>

#define USART3_MAX_SEND_LEN		20 					//����ͻ����ֽ���
#define USART3_MAX_RECV_LEN		26  				//�����ջ����ֽ���
//extern volatile  u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         					//����״̬���
extern bool USART3_Time_falg;
extern uint32_t laster_forword;
extern void laster_forward_control_set(void);

/*********************************************************************************************************
** Function name:       usart3_init
** Descriptions:        Ŀ��壨STM32F103ZET6����ʼ��USART3
** input parameters:    bound:���ڲ�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usart3_init(u32 bound);
/*********************************************************************************************************
** Function name:       u3_printf
** Descriptions:        USART3�������ʽ���printf�����ʽ����ȷ��һ�η������ݲ�����USART_MAX_SEND_LEN�ֽ�
** input parameters:    fmt���ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void u3_printf(char* fmt,...);
/*********************************************************************************************************
** Function name:       crc16_build
** Descriptions:        ����crcУ��������
** input parameters:    data_ptr����ҪУ�������������׵�ַ
						data_length: ��ҪУ������ݳ���
** output parameters:   ��
** Returned value:      CRCУ����
*********************************************************************************************************/
u16 crc16_build(u8 *data_ptr, u8 data_length);
#endif
