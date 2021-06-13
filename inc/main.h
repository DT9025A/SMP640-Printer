/*********************
 *     主头文件
 * 
 * @Author:DT9025A
 *********************/

#ifndef __MAIN_H__
#define __MAIN_H__

#define _USE_ESC_POS_

#include "adc.h"
#include "defines.h"
#include "ioconfig.h"
#include "lcd.h"
#include "pic.h"
#include "power.h"
#include "protocol.h"
#include "esc-pos.h"
#include "smp640.h"
#include "spi.h"
#include "uart.h"
#include "flash.h"
#include <STC8H.H>
#include <stdio.h>

//过热保护
#define TEMP_ALERT 50
//低于此温度取消过热保护
#define TEMP_DEALERT 35
//当
#define POWERDOWN_LOOP_COUNT 10

//系统温度
extern float SysTempearature;
//LCD显示缓冲区
extern uint8_t LCD_Buffer[8];
//是否过热
extern bit isOverHeating;
//允许打印, 置零为停止打印 (在uart.c声明)
extern bit enablePrinting;
//电源供电信息 (在power.c声明)
extern PowerStatus SysPowerStatus;
//加热模式 (在smp640.c声明)
extern uint8_t strobeMode;

//初始化IO
void IO_Init();
//初始化中断
void Interrupt_Init();
//获取并在屏幕上显示当前状态
void LCD_ShowData();

#endif