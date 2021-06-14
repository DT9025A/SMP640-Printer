/*********************
 *       ����
 * 460800bps@33.1776MHz
 * 
 * @Author:DT9025A
 *********************/

#ifndef __UART_H__
#define __UART_H__

#include "defines.h"
#include <STC8H.H>

//��������С
#define UART_BUFFER_SIZE 600
#define _19200BPS

//���λ�����
extern uint8_t xdata UART_BUFFER[UART_BUFFER_SIZE];

//��ʼ������
void UART_Init(void);
//���ڷ����ַ���
void UART_SendString(char *s);
//���ڷ���һ�ֽ�
void UART_SendData(unsigned char dat);
//�ӻ�������ȡһ�ֽ�, ������������������
unsigned char Uart_Getch();
//�������Ƿ��д���ȡ����
bit Uart_Available();

#endif