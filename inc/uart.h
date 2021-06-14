/*********************
 *       串口
 * 460800bps@33.1776MHz
 * 
 * @Author:DT9025A
 *********************/

#ifndef __UART_H__
#define __UART_H__

#include "defines.h"
#include <STC8H.H>

//缓冲区大小
#define UART_BUFFER_SIZE 600
#define _19200BPS

//环形缓冲区
extern uint8_t xdata UART_BUFFER[UART_BUFFER_SIZE];

//初始化串口
void UART_Init(void);
//串口发送字符串
void UART_SendString(char *s);
//串口发送一字节
void UART_SendData(unsigned char dat);
//从缓冲区读取一字节, 缓冲区无内容则阻塞
unsigned char Uart_Getch();
//缓冲区是否有待读取内容
bit Uart_Available();

#endif