#include "protocol.h"

uint8_t onProcessingCommand = CMD_NUL; //当前正在处理的指令
uint8_t readyCommand = CMD_NUL;        //校验完成将要处理的指令
uint8_t checksum;                      //校验和
uint8_t length;                        //包长度

//uint32_t transLength; //'接收数据'指令的数据长度

bit lastCommandSuccess = 1;      //上一个指令成功完成
uint8_t RXBuf[SMP640_LINE_SIZE]; //'接收数据'指令的缓冲区
uint8_t rxPtr;                   //'接收数据'指令的缓冲区下标

uint16_t temp16;
uint8_t temp8;

void Chksum()
{
    if (Uart_Getch() == checksum)
        ;
    else
    {
        UART_SendData(ERR_CHECKSUM_ERROR); //校验和错误
        lastCommandSuccess = 0;            //上一个指令未成功完成
    }

    readyCommand = onProcessingCommand;
    onProcessingCommand = CMD_FIN; //结束处理
}

void UART_Process_Data()
{
    if (Uart_Available())
    {
        //LCD_ShowStatus(TRANSFERING); //在传输数据
        LED_TRANSFERING = 0;
        switch (onProcessingCommand)
        {

        //没有在处理的指令
        case CMD_NUL:
            if (Uart_Getch() == PACKAGE_HEAD)
            {
                //LCD_ShowStatus(TRANSFERING);        //在传输数据
                onProcessingCommand = Uart_Getch();      //设置在处理指令
                length = Uart_Getch();                   //设置数据长度
                checksum = onProcessingCommand + length; //设置校验和
                rxPtr = 0;
                temp16 = 50;
            }
            else
            {
                UART_SendData(ERR_PACKAGE_NOT_ACCEPTED);      //数据包未被接受
                readyCommand = onProcessingCommand = CMD_FIN; //结束处理
                lastCommandSuccess = 0;                       //上一个指令未成功完成
            }
            break;

        //0数据指令
        //停止打印
        case CMD_STOP:
        //启动打印
        case CMD_START:
        //查询温度
        case CMD_TEMP:
        //清空缓冲区
        case CMD_CLEAR_BUFFER:
        //获取协议版本字符串
        case CMD_VERSION_STRING:
        //获取协议版本号
        case CMD_VERSION_CODE:
            Chksum();
            break;

        //接收数据
        case CMD_TRANSFER:
            while (length--)
            {
                RXBuf[rxPtr] = Uart_Getch();
                checksum += RXBuf[rxPtr];
                rxPtr++;
            }
            Chksum();
            break;

        //设置加热模式
        case CMD_STROBE_MODE:
            temp8 = Uart_Getch();
            checksum += temp8;
            Chksum();
            break;

        //单字数据指令
        //设置加热时间
        case CMD_HEAT_TIME:
        //走纸
        case CMD_DOTLINE:
            while (length--)
            {
                temp16 <<= 8;
                temp16 += Uart_Getch();
                checksum += (temp16 & 0xff);
            }
            Chksum();
            break;

        //结束处理
        default:
            if (Uart_Getch() == PACKAGE_END)
            {
                lastCommandSuccess = 1; //上一个指令成功完成

                switch (readyCommand) //所有校验正确, 进行设置
                {
                case CMD_STOP:
                    enablePrinting = 0;
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_START:
                    enablePrinting = 1;
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_TEMP:
                    LCD_ShowData();
                    UART_SendData(isOverHeating ? ERR_OVERHEAT : 0);
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_TRANSFER:
                    UART_SendData(SMP640_Buffer_FillLine(RXBuf) ? ERR_LINE_FILLED : ERR_LINE_UNFILLED);
                    break;

                case CMD_HEAT_TIME:
                    heatTime = temp16;
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_DOTLINE:
                    while (temp16--)
                    {
                        SMP640_Motor_DotLine();
                    }
                    SMP640_Motor_Step(MOTOR_STEP_STOP);
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_CLEAR_BUFFER:
                    SMP640_BUFFER_INIT;
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_VERSION_STRING:
                    UART_SendString(PROTOCOL_VERSION);
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_STROBE_MODE:
                    strobeMode = temp8;
                    UART_SendData(ERR_OK); //指令完成
                    break;

                case CMD_VERSION_CODE:
                    UART_SendData(PROTOCOL_MAJOR_VERSION);
                    UART_SendData(PROTOCOL_MINOR_VERSION);
                    UART_SendData(ERR_OK); //指令完成
                    break;

                default:
                    break;
                }
            }
            else
            {
                UART_SendData(ERR_PACKAGE_END_ERROR); //包尾错误
            }
            readyCommand = onProcessingCommand = CMD_NUL; //复位

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
