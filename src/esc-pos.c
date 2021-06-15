#include "esc-pos.h"

//��ǰ���ڴ����ָ��
uint8_t onProcessingCommand = CMD_NUL;
//У����ɽ�Ҫ�����ָ��
uint8_t readyCommand = CMD_NUL;

//'GS v 0'ָ��Ļ�����
uint8_t xdata RXBuf[SMP640_LINE_SIZE];
//'GS v 0'ָ��Ļ������±�
uint8_t rxPtr;

//GS v 0�Ѵ�ӡ������, Ϊ����ESC J
uint8_t printedLine = 0;

//��ʱ����
uint8_t temp8;
//��ʱ����
uint16_t temp16;

//����ָ��Ԥ����
void FinCMD()
{
    readyCommand = onProcessingCommand;
    onProcessingCommand = EOF;
}

void UART_Process_Data()
{
    if (Uart_Available() == 1)
    {
        switch (onProcessingCommand)
        {
        case CMD_NUL:
            //û���ڴ����ָ��
            temp8 = Uart_Getch();
            if (temp8 == ESC || temp8 == GS)
            {
                onProcessingCommand = Uart_Getch(); //�����ڴ���ָ��
                rxPtr = 0;
            }
            break;

        case '@':
            //ESC @
            FinCMD();
            break;

        case 'r':
            //ESC r
            FinCMD();
            break;

        case 'v':
            //GS v
            Uart_Getch();                //'0'
            Uart_Getch();                //m, '0'
            temp16 = Uart_Getch();       //xl
            temp16 += Uart_Getch() << 8; //xh
            temp8 = Uart_Getch();        //yl
            Uart_Getch();                //yh �õ���������û���ֹ�yh��������
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

        case 'J':
            //ESC J ��ӡ��ֽ
            temp8 = Uart_Getch();
            FinCMD();
            break;

        case 'd':
            //ESC d ��������ֽ
            temp8 = Uart_Getch();
            temp8 *= 8;
            printedLine = 0;
            FinCMD();
            break;

        //��������
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

            case '@':
                printedLine = 0;
                LCD_ShowStatus(IDLE);
                count = 0;
            case 'v':
                break;

            default:
                LCD_ShowStatus(IDLE);
                count = 0;
                break;
            }
            readyCommand = onProcessingCommand = CMD_NUL; //��λ
            break;
        }
    }
}
