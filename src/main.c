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

//指示是否过热
bit isOverHeating = 0;
//指示是否需要更新显示状态
bit stateChanged = 0;
//指示是否允许打印
bit enablePrinting = 1;

//指示是否需要通知上位机缺纸
bit needInformNoPaper = 0;
//指示是否已通知上位机缺纸
bit informedNoPaper = 1;

//指示是否需要通知上位机过热
bit needInformOverHeat = 0;
//指示是否已通知上位机过热
bit informedOverHeat = 1;

//指示是否需要通知上位机低压
bit needInformLowPower = 0;
//指示是否已通知上位机低压
bit informedLowPower = 1;

//初始化为过热状态
float SysTempearature = TEMP_ALERT + 1;
//sprintf格式化缓冲区
uint8_t LCD_Buffer[8];
//加热时间(us)
uint16_t heatTime = 6000;
uint8_t xdata FLASH_PAGE[256];

void main()
{
    //循环计数变量
    uint16_t count = 0;
    //获取行变量
    uint16_t line = 0xffff;

    IO_Init();                                 //设置IO模式
    SPIFLASH_NCS;
    LCD_Init();                                //初始化LCD1602
    LCD_Clear();                               //清屏
    LCD_PrintString(0, 0, "  Initializing  "); //显点字
    SPI_Init();                                //初始化SPI
    UART_Init();                               //初始化UART
    Interrupt_Init();                          //初始化中断
    delay_ms(500);


    LCD_PrintString(0, 0, " Loading FLASH  ");
    count = SPIFLASH_ReadMFDVID();
    if (count & 0xff == 0)
    {
        LCD_PrintString(0, 0, "Unable to detect");
        LCD_PrintString(0, 1, "flash size.     ");
        while (1) //不存在, 在这停顿
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
    LCD_PrintString(0, 0, "Checking Printer"); //显点字
    if (!SMP640_Exists())                      //判断打印头是否存在
    {
        LCD_PrintString(0, 0, "  Printer ERROR ");
        LCD_PrintString(0, 1, "Data trans error");
        while (1) //不存在, 在这停顿
            ;
    }
    SMP640_Clear_Register();            //存在, 清空缓存数据
    SMP640_BUFFER_INIT;                 //初始化缓冲区
    SMP640_Motor_DotLine();             //
    SMP640_Motor_DotLine();             //进行两次电机动作清空初始状态
    SMP640_Motor_Step(MOTOR_STEP_STOP); //关电机
    SMP640_VHEAT_OFF;                   //关加热
    SMP640_Strobe(STB_NUL);             //清零STB
    delay_ms(500);                      //不让字闪的太快
    LCD_ShowStatus(IDLE);               //初始状态
    LCD_ShowData();                     //数据

    while (1)
    {
        //是否更新数据显示
        if (stateChanged)
        {
            LCD_ShowData();
            stateChanged = 0;
        }

        //过热检测
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

        //缺纸检测
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

        //低电压检测
        if (SysPowerStatus.LOW_POWER)
        {
            LCD_ShowStatus(STOP);
            enablePrinting = 0;
            stateChanged = 1;
            informedLowPower = 0;
        }

        //闲置超时更新界面
        if (count > 6000)
        {
            count = 0;
            stateChanged = 1;
        }

        //闲置关电
        if (count > 5)
        {
            //这个移动到LCD_ShowStatus(IDLE)里面了
            //SMP640_VHEAT_OFF;
            //SMP640_Motor_Step(MOTOR_STEP_STOP); //关闭电机和STB
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

        //打印
        if (enablePrinting)
            if ((line = SMP640_BUFFER_LINE_NEXT_AVAILABLE()) != 0xffff)
            {
                LCD_ShowStatus(PRINTING);
                SMP640_Print_Line_Without_PowerDown(SMP640_BUFFER_LINE_PTR(line), SMP640_LINE_SIZE, heatTime);
                SMP640_BUFFER_LINE_AVAILABLE_CLEAR(line);
                /* if (SMP640_BUFFER_LINE_NEXT_AVAILABLE() == 0xffff)
                {
                    SMP640_VHEAT_OFF;
                    SMP640_Motor_Step(MOTOR_STEP_STOP); //关闭电机和STB
                    LCD_ShowStatus(IDLE);
                } */
                count = 0;
            }

        //处理打印数据
        UART_Process_Data();

        count++;
    }
}

//初始化IO
void IO_Init()
{
    //P0: 基本输入输出
    PIN_MODE_CONFIG(P0, PIN_ALL, PIN_MODE_STANDARD);

    //P1.0: 高阻输入ADC
    PIN_MODE_CONFIG(P1, PIN_ALL, PIN_MODE_STANDARD);
    PIN_MODE_CONFIG(P1, PIN_0, PIN_MODE_HIRGRESIN);
    PIN_MODE_CONFIG(P1, PIN_4, PIN_MODE_HIRGRESIN);
    MOSI = SCLK = 0;
    HC595_LATCH = 0;

    //P2: 基本输入输出
    PIN_MODE_CONFIG(P2, PIN_ALL, PIN_MODE_STANDARD);
    SMP640_VHEAT = 0;

    //P3.4 3.5: 推挽驱动LED P3.6: 开漏缺纸检测
    PIN_MODE_CONFIG(P3, PIN_ALL, PIN_MODE_STANDARD);
    PIN_MODE_CONFIG(P3, PIN_4 + PIN_5, PIN_MODE_PUSHPULL);
    PIN_MODE_CONFIG(P3, PIN_6, PIN_MODE_HIRGRESIN);
    SMP640_LATCH = 1;
    LED_TRANSFERING = LED_WORKING = 1;

    //P5: 高阻输入 电压检测
    PIN_MODE_CONFIG(P5, PIN_4, PIN_MODE_HIRGRESIN);
}

//初始化中断
void Interrupt_Init()
{
    IT0 = 1; //下降沿中断
    EX0 = 1; //外部中断0
    IT1 = 1; //下降沿中断
    EX1 = 1; //外部中断1
    ES = 1;  //串口中断
    EA = 1;  //总中断
}

//外部中断0 中断服务程序
//按下复位至ISP区域
void INT0_ISR() interrupt 0
{
    IAP_CONTR |= (1 << 6);
    IAP_CONTR |= (1 << 5);
}

//外部中断1 中断服务程序
void INT1_ISR() interrupt 2
{
    SMP640_BUFFER_INIT;
}

//获取过热状态
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

//获取并在屏幕上显示当前状态
void LCD_ShowData()
{
    // 第二行 系统状态
    // 温度数据 温度 纸张 打印
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
