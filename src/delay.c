#include "delay.h"

void delay_ms(uint16_t ms)
{
    while (ms--)
    {
        delay_1ms();
    }
}

void delay_1ms()
{
    unsigned char i, j;

    _nop_();
    _nop_();
    i = 44;
    j = 19;
    do
    {
        while (--j)
            ;
    } while (--i);
}

void delay_us(uint16_t us)
{
    while (us--)
    {
        delay_1us();
    }
}

void delay_1us()
{
    unsigned char i;

    i = 9;
    while (--i)
        ;
}
