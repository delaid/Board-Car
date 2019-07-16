#ifndef __DRAWPATH_H
#define __DRAWPATH_H
#include "sys.h"


/*********************************************************************************************************
** Function name:       DrawPath_Mode
** Descriptions:        当前处于画路径模式，对画路径协议数据进行处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void DrawPath_Mode(void);

/*********************************************************************************************************
** Function name:       Set_DrawPath
** Descriptions:        处理并设置路径数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_DrawPath(void);

/*********************************************************************************************************
** Function name:       Read_DrawPath
** Descriptions:        读取路径状态
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Read_DrawPath(void);

/*********************************************************************************************************
** Function name:       DrawPath_DP
** Descriptions:        当前处于画路径模式，获取uart3的接收缓存数据并进行转换
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
u8 DrawPath_DP(void);

/*********************************************************************************************************
** Function name:       DrawPath_UpsideDown
** Descriptions:        路径数据颠倒
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void DrawPath_UpsideDown(void);
#endif
