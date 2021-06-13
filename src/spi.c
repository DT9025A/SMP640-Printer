#include "spi.h"

void SPI_Init()
{
    SPCTL = 0xD0 | SPI_CLK_DIV; //使能SPI主机模式 SSIG+SPEN+MSTR
    SPCTL &= ~CPHA;
    SPCTL &= ~CPOL; //mode 0
    SPSTAT = 0xc0;  //清中断标志
}

unsigned char SPI_SendByte(unsigned char dat)
{
    SPDAT = dat;   //发送数据
    SPI_WAIT_BUSY; //等待数据传送完成
    SPSTAT = 0xc0; //清除标志位
    return SPDAT;  //返回从机传输数据
}

void SPI_GenerateOneClock()
{
    SCLK = 0;
    delay_us(5);
    SCLK = 1;
    delay_us(5);
    SCLK = 0;
}