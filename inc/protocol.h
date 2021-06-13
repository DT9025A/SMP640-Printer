/*********************
 *   UART通讯协议
 * 
 * 包头 控制字 数据长度 数据 校验 包尾
 * 数据长度为 数据+校验+包尾
 * 校验为    控制字+数据长度+数据
 * 
 * @Author:DT9025A
 *********************/

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "defines.h"
#include "lcd.h"
#include "smp640.h" //打印
#include "uart.h"   //缓冲区

#define PROTOCOL_VERSION "SP V2.3 B210310"
#define PROTOCOL_MAJOR_VERSION 2
#define PROTOCOL_MINOR_VERSION 3

enum COMMAND
{
    CMD_NUL,
    CMD_STOP,
    CMD_START,
    CMD_TEMP,
    CMD_TRANSFER,
    CMD_HEAT_TIME,
    CMD_DOTLINE,
    CMD_CLEAR_BUFFER,
    CMD_VERSION_STRING,
    CMD_VERSION_CODE,
    CMD_STROBE_MODE,
    CMD_FIN = 0xff
};

enum REPLY
{
    ERR_OK,
    ERR_PACKAGE_NOT_ACCEPTED,
    ERR_PACKAGE_END_ERROR,
    ERR_CHECKSUM_ERROR,
    ERR_LINE_FILLED,
    ERR_LINE_UNFILLED,
    ERR_OVERHEAT,
    ERR_NOPAPER,
    ERR_LOWPOWER
};

#ifndef _USE_ESC_POS_

extern bit enablePrinting;
extern bit isOverHeating;
extern bit stateChanged;
extern uint8_t RXBuf[SMP640_LINE_SIZE];
extern uint16_t heatTime;
extern uint8_t strobeMode;

#define PACKAGE_HEAD 0x33
#define PACKAGE_END 0xAA

void UART_Process_Data();
void LCD_ShowData();

#endif

#endif