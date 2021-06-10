#ifndef __SPI_H__
#define __SPI_H__

#include <STC8H.H>
#include "defines.h"
#include "delay.h"

#define CPOL 0X08
#define CPHA 0X04
#define SPI_CLK_DIV 1                          //SPIʱ��4��Ƶ
#define SPI_WAIT_BUSY while (!(SPSTAT & 0x80)) //�ȴ��������

//��ʼ��SPI
void SPI_Init();
//SPI�����ֽ�
unsigned char SPI_SendByte(unsigned char dat); 
//SPI����һ��ʱ��
void SPI_GenerateOneClock();

#endif