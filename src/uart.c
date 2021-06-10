#include "uart.h"

uint8_t xdata UART_BUFFER[UART_BUFFER_SIZE]; //���λ������
uint8_t UART_RECV_PTR;                       //������н���ָ��
uint8_t UART_RECV_READ_PTR;                  //��������Ѷ�ȡָ��

void Uart_Init(void) //460800bps@33.1776MHz
{
    SCON = 0x50;  //8λ����,�ɱ䲨����
    AUXR |= 0x40; //��ʱ��1ʱ��ΪFosc,��1T
    AUXR &= 0xFE; //����1ѡ��ʱ��1Ϊ�����ʷ�����
    TMOD &= 0x0F; //�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
    TL1 = 0xEE;   //�趨��ʱ��ֵ
    TH1 = 0xFF;   //�趨��ʱ��ֵ
    ET1 = 0;      //��ֹ��ʱ��1�ж�
    TR1 = 1;      //������ʱ��1

    UART_RECV_PTR = 0;
    UART_RECV_READ_PTR = 0;
}

//���ڷ����ַ���
void UART_SendString(char *s)
{
    while (*s)
    {
        UART_SendData(*s++);
    }
}

//���ڷ���һ�ֽ�
void UART_SendData(unsigned char dat)
{
    SBUF = dat;
    while (TI == 0)
        ;
    TI = 0;
}

//�������Ƿ��д���ȡ����
bit Uart_Available()
{
    //�������ָ����ڶ�ȡָ��, ����һ����δ��ȡ����
    if (UART_RECV_PTR > UART_RECV_READ_PTR)
        return 1;
    //�������ָ����ڶ�ȡָ��, �������ݶ�ȡ���
    else if (UART_RECV_PTR == UART_RECV_READ_PTR)
        return 0;
    //�������ָ��С�ڶ�ȡָ��, �������ζ�������, ��������δ��ȡ����
    else
        return 1;
}

//�ӻ�������ȡһ�ֽ�, ������������������
unsigned char Uart_Getch()
{
    while (!Uart_Available()) //�ȴ���Ч����
        ;
    if (UART_RECV_READ_PTR == UART_BUFFER_SIZE) //��ȡ������β��
        UART_RECV_READ_PTR = 0;                 //���ö�ȡָ��
    return UART_BUFFER[UART_RECV_READ_PTR++];   //��ȡ����
}

//UART�жϷ�����
void UART_ISR() interrupt 4
{
    if (RI)
    {
        RI = 0;
        if (UART_RECV_PTR == UART_BUFFER_SIZE) //������
            UART_RECV_PTR = 0;                 //���û��ζ���
        UART_BUFFER[UART_RECV_PTR++] = SBUF;   //��������
    }
}