#include "protocol.h"

uint8_t onProcessingCommand = CMD_NUL; //��ǰ���ڴ�����ָ��
uint8_t readyCommand = CMD_NUL;        //У����ɽ�Ҫ������ָ��
uint8_t checksum;                      //У���
uint8_t length;                        //������

//uint32_t transLength; //'��������'ָ������ݳ���

bit lastCommandSuccess = 1;      //��һ��ָ��ɹ����
uint8_t RXBuf[SMP640_LINE_SIZE]; //'��������'ָ��Ļ�����
uint8_t rxPtr;                   //'��������'ָ��Ļ������±�

uint16_t temp16;
uint8_t temp8;

void Chksum()
{
    if (Uart_Getch() == checksum)
        ;
    else
    {
        UART_SendData(ERR_CHECKSUM_ERROR); //У��ʹ���
        lastCommandSuccess = 0;            //��һ��ָ��δ�ɹ����
    }

    readyCommand = onProcessingCommand;
    onProcessingCommand = CMD_FIN; //��������
}

void UART_Process_Data()
{
    if (Uart_Available())
    {
        //LCD_ShowStatus(TRANSFERING); //�ڴ�������
        LED_TRANSFERING = 0;
        switch (onProcessingCommand)
        {

        //û���ڴ�����ָ��
        case CMD_NUL:
            if (Uart_Getch() == PACKAGE_HEAD)
            {
                //LCD_ShowStatus(TRANSFERING);        //�ڴ�������
                onProcessingCommand = Uart_Getch();      //�����ڴ���ָ��
                length = Uart_Getch();                   //�������ݳ���
                checksum = onProcessingCommand + length; //����У���
                rxPtr = 0;
                temp16 = 50;
            }
            else
            {
                UART_SendData(ERR_PACKAGE_NOT_ACCEPTED);      //���ݰ�δ������
                readyCommand = onProcessingCommand = CMD_FIN; //��������
                lastCommandSuccess = 0;                       //��һ��ָ��δ�ɹ����
            }
            break;

        //0����ָ��
        //ֹͣ��ӡ
        case CMD_STOP:
        //������ӡ
        case CMD_START:
        //��ѯ�¶�
        case CMD_TEMP:
        //��ջ�����
        case CMD_CLEAR_BUFFER:
        //��ȡЭ��汾�ַ���
        case CMD_VERSION_STRING:
        //��ȡЭ��汾��
        case CMD_VERSION_CODE:
            Chksum();
            break;

        //��������
        case CMD_TRANSFER:
            while (length--)
            {
                RXBuf[rxPtr] = Uart_Getch();
                checksum += RXBuf[rxPtr];
                rxPtr++;
            }
            Chksum();
            break;

        //���ü���ģʽ
        case CMD_STROBE_MODE:
            temp8 = Uart_Getch();
            checksum += temp8;
            Chksum();
            break;

        //��������ָ��
        //���ü���ʱ��
        case CMD_HEAT_TIME:
        //��ֽ
        case CMD_DOTLINE:
            while (length--)
            {
                temp16 <<= 8;
                temp16 += Uart_Getch();
                checksum += (temp16 & 0xff);
            }
            Chksum();
            break;

        //��������
        default:
            if (Uart_Getch() == PACKAGE_END)
            {
                lastCommandSuccess = 1; //��һ��ָ��ɹ����

                switch (readyCommand) //����У����ȷ, ��������
                {
                case CMD_STOP:
                    enablePrinting = 0;
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_START:
                    enablePrinting = 1;
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_TEMP:
                    LCD_ShowData();
                    UART_SendData(isOverHeating ? ERR_OVERHEAT : 0);
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_TRANSFER:
                    UART_SendData(SMP640_Buffer_FillLine(RXBuf) ? ERR_LINE_FILLED : ERR_LINE_UNFILLED);
                    break;

                case CMD_HEAT_TIME:
                    heatTime = temp16;
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_DOTLINE:
                    while (temp16--)
                    {
                        SMP640_Motor_DotLine();
                    }
                    SMP640_Motor_Step(MOTOR_STEP_STOP);
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_CLEAR_BUFFER:
                    SMP640_BUFFER_INIT;
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_VERSION_STRING:
                    UART_SendString(PROTOCOL_VERSION);
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_STROBE_MODE:
                    strobeMode = temp8;
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                case CMD_VERSION_CODE:
                    UART_SendData(PROTOCOL_MAJOR_VERSION);
                    UART_SendData(PROTOCOL_MINOR_VERSION);
                    UART_SendData(ERR_OK); //ָ�����
                    break;

                default:
                    break;
                }
            }
            else
            {
                UART_SendData(ERR_PACKAGE_END_ERROR); //��β����
            }
            readyCommand = onProcessingCommand = CMD_NUL; //��λ

            LED_TRANSFERING = 1;
            if (enablePrinting)
            {
                if (SMP640_BUFFER_LINE_NEXT_AVAILABLE() == 0XFFFF)
                    LCD_ShowStatus(IDLE);
            }
            else
            {
                LCD_ShowStatus(STOP);
            }
            break;
        }
    }
}