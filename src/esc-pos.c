#include "esc-pos.h"

uint8_t onProcessingCommand = CMD_NUL; //当前正在处理的指令
uint8_t readyCommand = CMD_NUL;        //校验完成将要处理的指令

uint8_t RXBuf[SMP640_LINE_SIZE]; //'接收数据'指令的缓冲区
uint8_t rxPtr;                   //'接收数据'指令的缓冲区下标

uint8_t printedLine = 0; //GS v 0已打印的行数, 为兼容ESC J

uint8_t temp8;
uint16_t temp16;

//结束指令预处理
void FinCMD()
{
    readyCommand = onProcessingCommand;
    onProcessingCommand = EOF;
}

void UART_Process_Data()
{
    if (Uart_Available())
    {
        LED_TRANSFERING = 0;
        switch (onProcessingCommand)
        {

        //没有在处理的指令
        case CMD_NUL:
            temp8 = Uart_Getch();
            if (temp8 == ESC || temp8 == GS)
            {
                onProcessingCommand = Uart_Getch(); //设置在处理指令
                rxPtr = 0;
            }

            break;

        case '@':
            //ESC @
            printedLine = 0;
            FinCMD();
            break;

        case 'v':
            //GS v
            Uart_Getch();                //'0'
            Uart_Getch();                //m, '0'
            temp16 = Uart_Getch();       //xl
            temp16 += Uart_Getch() << 8; //xh
            temp8 = Uart_Getch();        //yl
            Uart_Getch();                //yh 本驱动就没出现过yh非零的情况
            temp16 *= temp8;             //(x)*(y)

            LCD_ShowStatus(PRINTING);
            while (temp16--)
            {
                RXBuf[rxPtr++] = Uart_Getch();
                if (rxPtr == SMP640_LINE_SIZE)
                {
                    SMP640_Print_Line_Without_PowerDown(RXBuf, 48, heatTime);
                    printedLine++;
                    rxPtr = 0;
                }
            }
            FinCMD();
            break;

        //打印走纸
        case 'J':
            temp8 = Uart_Getch();
            FinCMD();
            break;

        //无条件走纸
        case 'd':
            temp8 = Uart_Getch();
            temp8 *= 8;
            printedLine = 0;
            FinCMD();
            break;

        //结束处理
        default:
            switch (readyCommand)
            {
            case 'J':
            case 'd':
                temp8 -= printedLine;
                while (temp8--)
                {
                    SMP640_Motor_DotLine();
                }
                printedLine = 0;
                break;

            default:
                break;
            }
            readyCommand = onProcessingCommand = CMD_NUL; //复位

            SMP640_Motor_Step(MOTOR_STEP_STOP); //关电机
            LCD_ShowStatus(IDLE);
            LED_TRANSFERING = 1;
            break;
        }
    }
}
