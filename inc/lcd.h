/*********************
 *     LCD驱动
 * 
 * @Author:DT9025A
 *********************/

#ifndef __LCD_H__
#define __LCD_H__

#include <STC8H.H>
#include "defines.h"
#include "delay.h"
#include "smp640.h"

//小字符列表
enum SMALL_CHAR {
    CH_PRINTING2 = 0,
    CH_PAPERNOTEXISTS,
    CH_PAPEREXISTS,
    CH_IDLE,
    CH_PRINTING,
    CH_CESIUS,
    CH_OVERHEAT,
    CH_TEMPOK
};

//打印状态
enum STATUS
{
    TRANSFERING,
    PRINTING,
    IDLE,
    STOP
};

//液晶初始化
void LCD_Init(void);
//液晶清屏
void LCD_Clear(void);
//液晶定位
void LCD_SetPosition(unsigned char x, unsigned char y);
//字符定位输出
void LCD_PrintChar(unsigned char x, unsigned char y, unsigned char c);
//字符串定位输出
void LCD_PrintString(unsigned char x, unsigned char y, unsigned char *s);
// 在LCD第一行显示工作状态
void LCD_ShowStatus(uint8_t s);

#endif