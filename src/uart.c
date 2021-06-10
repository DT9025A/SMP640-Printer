#include "uart.h"

uint8_t xdata UART_BUFFER[UART_BUFFER_SIZE]; //环形缓冲队列
uint8_t UART_RECV_PTR;                       //缓冲队列接收指针
uint8_t UART_RECV_READ_PTR;                  //缓冲队列已读取指针

void Uart_Init(void) //460800bps@33.1776MHz
{
    SCON = 0x50;  //8位数据,可变波特率
    AUXR |= 0x40; //定时器1时钟为Fosc,即1T
    AUXR &= 0xFE; //串口1选择定时器1为波特率发生器
    TMOD &= 0x0F; //设定定时器1为16位自动重装方式
    TL1 = 0xEE;   //设定定时初值
    TH1 = 0xFF;   //设定定时初值
    ET1 = 0;      //禁止定时器1中断
    TR1 = 1;      //启动定时器1

    UART_RECV_PTR = 0;
    UART_RECV_READ_PTR = 0;
}

//串口发送字符串
void UART_SendString(char *s)
{
    while (*s)
    {
        UART_SendData(*s++);
    }
}

//串口发送一字节
void UART_SendData(unsigned char dat)
{
    SBUF = dat;
    while (TI == 0)
        ;
    TI = 0;
}

//缓冲区是否有待读取内容
bit Uart_Available()
{
    //如果接收指针大于读取指针, 表明一定有未读取数据
    if (UART_RECV_PTR > UART_RECV_READ_PTR)
        return 1;
    //如果接收指针等于读取指针, 表明数据读取完成
    else if (UART_RECV_PTR == UART_RECV_READ_PTR)
        return 0;
    //如果接收指针小于读取指针, 表明环形队列重置, 队列首有未读取数据
    else
        return 1;
}

//从缓冲区读取一字节, 缓冲区无内容则阻塞
unsigned char Uart_Getch()
{
    while (!Uart_Available()) //等待有效数据
        ;
    if (UART_RECV_READ_PTR == UART_BUFFER_SIZE) //读取到队列尾部
        UART_RECV_READ_PTR = 0;                 //重置读取指针
    return UART_BUFFER[UART_RECV_READ_PTR++];   //读取数据
}

//UART中断服务函数
void UART_ISR() interrupt 4
{
    if (RI)
    {
        RI = 0;
        if (UART_RECV_PTR == UART_BUFFER_SIZE) //队列满
            UART_RECV_PTR = 0;                 //重置环形队列
        UART_BUFFER[UART_RECV_PTR++] = SBUF;   //放入数据
    }
}