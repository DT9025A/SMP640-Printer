/**
 * *********************************************
 * 
 * SMP640 Driver
 * 
 * 33.1176MHz
 * 
 * *********************************************
*/

#include "main.h"

//ָʾ�Ƿ����
bit isOverHeating = 0;
//ָʾ�Ƿ���Ҫ������ʾ״̬
bit stateChanged = 0;
//ָʾ�Ƿ������ӡ
bit enablePrinting = 1;

//ָʾ�Ƿ���Ҫ֪ͨ��λ��ȱֽ
bit needInformNoPaper = 0;
//ָʾ�Ƿ���֪ͨ��λ��ȱֽ
bit informedNoPaper = 1;

//ָʾ�Ƿ���Ҫ֪ͨ��λ������
bit needInformOverHeat = 0;
//ָʾ�Ƿ���֪ͨ��λ������
bit informedOverHeat = 1;

//ָʾ�Ƿ���Ҫ֪ͨ��λ����ѹ
bit needInformLowPower = 0;
//ָʾ�Ƿ���֪ͨ��λ����ѹ
bit informedLowPower = 1;

//��ʼ��Ϊ����״̬
float SysTempearature = TEMP_ALERT + 1;
//sprintf��ʽ��������
uint8_t LCD_Buffer[8];
//����ʱ��(us)
uint16_t heatTime = 6000;
uint8_t xdata FLASH_PAGE[256];

void main()
{
    //ѭ����������
    uint16_t count = 0;
    //��ȡ�б���
    uint16_t line = 0xffff;

    IO_Init();                                 //����IOģʽ
    SPIFLASH_NCS;
    LCD_Init();                                //��ʼ��LCD1602
    LCD_Clear();                               //����
    LCD_PrintString(0, 0, "  Initializing  "); //�Ե���
    SPI_Init();                                //��ʼ��SPI
    UART_Init();                               //��ʼ��UART
    Interrupt_Init();                          //��ʼ���ж�
    delay_ms(500);


    LCD_PrintString(0, 0, " Loading FLASH  ");
    count = SPIFLASH_ReadMFDVID();
    if (count & 0xff == 0)
    {
        LCD_PrintString(0, 0, "Unable to detect");
        LCD_PrintString(0, 1, "flash size.     ");
        while (1) //������, ����ͣ��
            ;
    }
    sprintf(LCD_Buffer, "%02X %02X", count >> 8, count & 0xff);
    LCD_PrintString(0, 0, " DeviceID:      ");
    LCD_PrintString(10, 0, LCD_Buffer);

    LCD_PrintString(0, 1, " Flash size:    ");
    sprintf(LCD_Buffer, "%dMB", (1 << (count & 0x0f)) / 8);
    LCD_PrintString(12, 1, LCD_Buffer);
    delay_ms(2000);
    
    // LCD_PrintString(0, 0, "Erasing Flash...");
    // SPIFLASH_ChipErase();
    // SPIFLASH_WaitBUSY();
    // LCD_PrintString(0, 1, "    Erased.     ");
    // delay_ms(500);

    LCD_Clear();
    LCD_PrintString(0, 0, "Checking Printer"); //�Ե���
    if (!SMP640_Exists())                      //�жϴ�ӡͷ�Ƿ����
    {
        LCD_PrintString(0, 0, "  Printer ERROR ");
        LCD_PrintString(0, 1, "Data trans error");
        while (1) //������, ����ͣ��
            ;
    }
    SMP640_Clear_Register();            //����, ��ջ�������
    SMP640_BUFFER_INIT;                 //��ʼ��������
    SMP640_Motor_DotLine();             //
    SMP640_Motor_DotLine();             //�������ε��������ճ�ʼ״̬
    SMP640_Motor_Step(MOTOR_STEP_STOP); //�ص��
    SMP640_VHEAT_OFF;                   //�ؼ���
    SMP640_Strobe(STB_NUL);             //����STB
    delay_ms(500);                      //����������̫��
    LCD_ShowStatus(IDLE);               //��ʼ״̬
    LCD_ShowData();                     //����

    while (1)
    {
        //�Ƿ����������ʾ
        if (stateChanged)
        {
            LCD_ShowData();
            stateChanged = 0;
        }

        //���ȼ��
        if (isOverHeating && !needInformOverHeat)
        {
            LCD_ShowStatus(STOP);
            enablePrinting = 0;
            stateChanged = 1;
            needInformOverHeat = 1;
            informedOverHeat = 0;
        }
        else
        {
            needInformOverHeat = 0;
        }

        //ȱֽ���
        if (!SMP640_PaperCheck() && !needInformNoPaper)
        {
            LCD_ShowStatus(STOP);
            enablePrinting = 0;
            stateChanged = 1;
            needInformNoPaper = 1;
            informedNoPaper = 0;
        }
        else
        {
            needInformNoPaper = 0;
        }

        //�͵�ѹ���
        if (SysPowerStatus.LOW_POWER)
        {
            LCD_ShowStatus(STOP);
            enablePrinting = 0;
            stateChanged = 1;
            informedLowPower = 0;
        }

        //���ó�ʱ���½���
        if (count > 6000)
        {
            count = 0;
            stateChanged = 1;
        }

        //���ùص�
        if (count > 5)
        {
            //����ƶ���LCD_ShowStatus(IDLE)������
            //SMP640_VHEAT_OFF;
            //SMP640_Motor_Step(MOTOR_STEP_STOP); //�رյ����STB
            if (enablePrinting)
            {
                LCD_ShowStatus(IDLE);
            }
        }

        if (needInformNoPaper && !informedNoPaper)
        {
            UART_SendData(ERR_NOPAPER);
            informedNoPaper = 1;
        }
        if (needInformLowPower && !informedLowPower)
        {
            UART_SendData(ERR_LOWPOWER);
            informedLowPower = 0;
        }
        if (needInformOverHeat && !informedOverHeat)
        {
            UART_SendData(ERR_OVERHEAT);
            informedOverHeat = 0;
        }

        //��ӡ
        if (enablePrinting)
            if ((line = SMP640_BUFFER_LINE_NEXT_AVAILABLE()) != 0xffff)
            {
                LCD_ShowStatus(PRINTING);
                SMP640_Print_Line_Without_PowerDown(SMP640_BUFFER_LINE_PTR(line), SMP640_LINE_SIZE, heatTime);
                SMP640_BUFFER_LINE_AVAILABLE_CLEAR(line);
                /* if (SMP640_BUFFER_LINE_NEXT_AVAILABLE() == 0xffff)
                {
                    SMP640_VHEAT_OFF;
                    SMP640_Motor_Step(MOTOR_STEP_STOP); //�رյ����STB
                    LCD_ShowStatus(IDLE);
                } */
                count = 0;
            }

        //�����ӡ����
        UART_Process_Data();

        count++;
    }
}

//��ʼ��IO
void IO_Init()
{
    //P0: �����������
    PIN_MODE_CONFIG(P0, PIN_ALL, PIN_MODE_STANDARD);

    //P1.0: ��������ADC
    PIN_MODE_CONFIG(P1, PIN_ALL, PIN_MODE_STANDARD);
    PIN_MODE_CONFIG(P1, PIN_0, PIN_MODE_HIRGRESIN);
    PIN_MODE_CONFIG(P1, PIN_4, PIN_MODE_HIRGRESIN);
    MOSI = SCLK = 0;
    HC595_LATCH = 0;

    //P2: �����������
    PIN_MODE_CONFIG(P2, PIN_ALL, PIN_MODE_STANDARD);
    SMP640_VHEAT = 0;

    //P3.4 3.5: ��������LED P3.6: ��©ȱֽ���
    PIN_MODE_CONFIG(P3, PIN_ALL, PIN_MODE_STANDARD);
    PIN_MODE_CONFIG(P3, PIN_4 + PIN_5, PIN_MODE_PUSHPULL);
    PIN_MODE_CONFIG(P3, PIN_6, PIN_MODE_HIRGRESIN);
    SMP640_LATCH = 1;
    LED_TRANSFERING = LED_WORKING = 1;

    //P5: �������� ��ѹ���
    PIN_MODE_CONFIG(P5, PIN_4, PIN_MODE_HIRGRESIN);
}

//��ʼ���ж�
void Interrupt_Init()
{
    IT0 = 1; //�½����ж�
    EX0 = 1; //�ⲿ�ж�0
    IT1 = 1; //�½����ж�
    EX1 = 1; //�ⲿ�ж�1
    ES = 1;  //�����ж�
    EA = 1;  //���ж�
}

//�ⲿ�ж�0 �жϷ������
//���¸�λ��ISP����
void INT0_ISR() interrupt 0
{
    IAP_CONTR |= (1 << 6);
    IAP_CONTR |= (1 << 5);
}

//�ⲿ�ж�1 �жϷ������
void INT1_ISR() interrupt 2
{
    SMP640_BUFFER_INIT;
}

//��ȡ����״̬
void GetOverheatingStatus()
{
    if (SysTempearature > TEMP_ALERT)
    {
        isOverHeating = 1;
    }
    if (isOverHeating)
    {
        if (SysTempearature < TEMP_DEALERT)
        {
            isOverHeating = 0;
        }
    }
}

//��ȡ������Ļ����ʾ��ǰ״̬
void LCD_ShowData()
{
    // �ڶ��� ϵͳ״̬
    // �¶����� �¶� ֽ�� ��ӡ
    SysTempearature = SMP640_Heat_Temp();
    GetPowerStatus();
    GetOverheatingStatus();

    sprintf(LCD_Buffer, "%.1f\5", SysTempearature);
    LCD_PrintString(0, 1, LCD_Buffer);
    LCD_PrintString(12, 1, "STB");
    switch (strobeMode)
    {
    case STROBE_1:
        LCD_PrintString(15, 1, "1");
        break;

    case STROBE_2:
        LCD_PrintString(15, 1, "2");
        break;

    case STROBE_3:
        LCD_PrintString(15, 1, "3");
        break;

    case STROBE_6:
        LCD_PrintString(15, 1, "6");
        break;

    default:
        LCD_PrintString(15, 1, "N");
        break;
    }
    // sprintf(LCD_Buffer, "%.1fV", SysPowerStatus.voltage / 1000);
    // LCD_PrintString(12, 1, LCD_Buffer);

    if (isOverHeating)
        LCD_PrintChar(6, 1, CH_OVERHEAT);
    else
        LCD_PrintChar(6, 1, CH_TEMPOK);

    if (SMP640_PaperCheck())
        LCD_PrintChar(8, 1, CH_PAPEREXISTS);
    else
        LCD_PrintChar(8, 1, CH_PAPERNOTEXISTS);
}
