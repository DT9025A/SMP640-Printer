#include "esc-pos.h"

uint8_t onProcessingCommand = CMD_NUL; //当前正在处理的指令
uint8_t readyCommand = CMD_NUL;        //校验完成将要处理的指令

uint8_t RXBuf[SMP640_LINE_SIZE]; //'接收数据'指令的缓冲区
uint8_t rxPtr;                   //'接收数据'指令的缓冲区下标

uint8_t temp8;
uint16_t temp16;

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
            //SMP640_BUFFER_INIT;
            //memset(RXBuf, 0, 48);
            readyCommand = onProcessingCommand;
            onProcessingCommand = EOF;
            break;

        case 'v':
            //GS v
            Uart_Getch();                //'0'
            Uart_Getch();                //m, '0'
            temp16 = Uart_Getch();       //xl
            temp16 += Uart_Getch() << 8; //xh
            temp8 = Uart_Getch();        //yl
            Uart_Getch();                //yh
            temp16 *= temp8;             //(x)*(y)
            // Uart_Getch(); 
            // temp16 *= Uart_Getch();
            // Uart_Getch();

            LCD_ShowStatus(PRINTING);
            while (temp16--)
            {
                RXBuf[rxPtr++] = Uart_Getch();
                if (rxPtr == SMP640_LINE_SIZE)
                {
                    SMP640_Print_Line_Without_PowerDown(RXBuf, 48, heatTime);
                    rxPtr = 0;
                }
            }
            readyCommand = onProcessingCommand;
            onProcessingCommand = EOF;
            break;

        //走纸
        case 'J':
            temp8 = Uart_Getch();
            readyCommand = onProcessingCommand;
            onProcessingCommand = EOF;
            break;

        //结束处理
        default:
            switch (readyCommand)
            {
            case 'J':
                while (temp8--)
                {
                    //SMP640_Motor_DotLine();
                }
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
