#ifndef __SPI_H__
#define __SPI_H__

#include <STC8H.H>
#include "defines.h"
#include "delay.h"

#define CPOL 0X08
#define CPHA 0X04
#define SPI_CLK_DIV 1                          //SPI时钟4分频
#define SPI_WAIT_BUSY while (!(SPSTAT & 0x80)) //等待发送完成

//初始化SPI
void SPI_Init();
//SPI发送字节
unsigned char SPI_SendByte(unsigned char dat); 
//SPI产生一个时钟
void SPI_GenerateOneClock();

#endif