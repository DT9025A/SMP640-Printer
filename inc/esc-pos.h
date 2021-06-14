/**
 * ESC/POS指令集
 * 
 * 指令支持列表如下:
 * ESC @ / 1B 40
 * ESC J / 1B 4A N  在此被处理为仅走纸未打印行数
 * ESC d / 1B 64 N  在此被处理为走纸n * 8点行
 * GS v 0 / 1D 76 30  在此被处理为直接打印
 * 
 */

#ifndef __ESC_POS_H__
#define __ESC_POS_H__

#include "defines.h"
#include "lcd.h"
#include "smp640.h" //打印
#include "uart.h"   //缓冲区

#define CMD_NUL 0x00
#define HT 0x09
#define LF 0x0A
#define CR 0x0D
#define ESC 0x1B
#define DLE 0x10
#define GS 0x1D
#define FS 0x1C
#define STX 0x02
#define US 0x1F
#define CAN 0x18
#define CLR 0x0C
#define EOT 0x04
#define EOF 0xFF

extern uint16_t heatTime;
extern uint16_t count;

void UART_Process_Data();

#endif
