#include "smp640.h"

#pragma region 数据相关

//进行一次数据锁存操作
void SMP640_Latch()
{
    SMP640_LATCH = 0;
    delay_us(10);
    SMP640_LATCH = 1;
}

//清空打印头内的缓存数据
void SMP640_Clear_Register()
{
    unsigned char p;

    for (p = 0; p < 48; p++)
    {
        SPI_SendByte(0x00);
    }
    SMP640_Latch();
}

//595数据锁存操作, 控制STB
void HC595_Latch()
{
    HC595_LATCH = 1;
    delay_us(10);
    HC595_LATCH = 0;
}

//判断SMP640是否存在
bit SMP640_Exists()
{
    unsigned char j, k;
    bit accept = 1;

    SMP640_Clear_Register();
    for (j = 0; j < SMP640_LINE_SIZE; j++)
    {
        k = SPI_SendByte(j);
    }

    SPCTL |= CPHA; //手册上写输出后沿采样
    for (j = 0; j < SMP640_LINE_SIZE; j++)
    {
        k = SPI_SendByte(0) >> 1; //快一时钟
        if (k != j)
            accept = 0;
    }

    SPI_Init(); //重新初始化SPI
    SMP640_Clear_Register();
    return accept;
}
#pragma endregion

#pragma region 加热

//热敏电阻计算常量
code float Rp = 30000.0;
code float T2 = (273.15 + 25.0);
code float Bx = 3950.0;
code float Ka = 273.15;
code float arg1 = 3950.0 / (273.15 + 25.0);

//开/关当前STB的加热, 参数指定加热STB号
void SMP640_Strobe(uint8_t stb)
{
    SPI_SendByte(stb);
    HC595_Latch();
}

//获取加热头温度
float SMP640_Heat_Temp()
{
    float Rt;
    float temp;
    int read;

    read = ADC_Read(0);
    Rt = (-30000.0f * read) / (read - 1723.52f);

    // temp = Rt / Rp;
    // temp = log(temp); //ln(Rt/Rp)
    // temp /= Bx;       //ln(Rt/Rp)/B
    // temp += (1 / T2);
    // temp = 1 / (temp);
    // temp -= Ka;
    temp = Rt / Rp;
    temp = log(temp) + arg1;
    temp = Bx / temp;
    temp -= Ka;
    return temp;
}

#pragma endregion

#pragma region 打印
uint8_t xdata strobeMode = STROBE_3;

//比较数据
bit memcmp_val(uint8_t *src, uint8_t size, uint8_t val)
{
    uint8_t ptr;

    for (ptr = 0; ptr < size; ptr++)
    {
        if (src[ptr] != val)
        {
            return 0;
        }
    }
    return 1;
}

void SMP640_Print_Line_Without_PowerDown(uint8_t *dat, uint8_t size, uint16_t delay)
{
    uint8_t i, dms;
    uint16_t dus;

    LED_WORKING = 0;
    dms = delay / 1000;
    dus = delay % 1000 / 2;
    SMP640_Clear_Register();

    if (size > 48)
    {
        return;
    }

    //该数据全是0直接简化为走纸
    if (!memcmp_val(dat, size, 0))
    {
        for (i = 0; i < size; i++)
        {
            SPI_SendByte(dat[i]);
        }

        for (; i < 48; i++)
        {
            SPI_SendByte(0x00);
        }
        delay_us(1);
        SMP640_Latch();

        SMP640_Strobe(STB_NUL);
        SMP640_VHEAT_ON;
        if (delay != 0)
        {
            /*
            如果电流足够可以同时选通多个STB, 提升打印速度
            但选通多个STB直接影响打印头温度和打印质量
            需求电流会大幅度增加
            对此可以提升加热头驱动电压或增加加热时间
            打印速度最快可达到加热时间3ms,220-230行/秒 27-29cm/秒, 但5V下加热效果不好
            
            测试效果较好的加热条件 (无空白数据):
            VHEAD=8V, STB全开, 加热3ms, 最大电流为7A左右, 升温极快, 230行/秒
            [推荐] VHEAD=8V, STB开3个, 每3个加热2ms, 最大电流为2.5A左右, 150行/秒, 384行升温6度
            VHEAD=8V, STB开2个, 每2个加热2ms, 最大电流为2.3A左右, 110行/秒
            [推荐] VHEAD=5V, STB开3个, 每3个加热7ms, 最大电流为2.5A左右, 60行/秒, 384行升温5度
            VHEAD=5V, STB开2个, 每2个加热6ms, 最大电流为2.5A左右, 45行/秒
            VHEAD=5V, STB开1个, 每1个加热6ms, 最大电流为1.5A左右, 30行/秒
            */

            switch (strobeMode)
            {
            case STROBE_1:
                if (!memcmp_val(dat, 8, 0))
                {
                    //STB1
                    SMP640_Strobe(STB_1);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 8, 8, 0))
                {
                    //STB2
                    SMP640_Strobe(STB_2);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 16, 8, 0))
                {
                    //STB3
                    SMP640_Strobe(STB_3);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 24, 8, 0))
                {
                    //STB4
                    SMP640_Strobe(STB_4);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 32, 8, 0))
                {
                    //STB5
                    SMP640_Strobe(STB_5);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 40, 8, 0))
                {
                    //STB6
                    SMP640_Strobe(STB_6);
                    delay_ms(dms);
                    delay_us(dus);
                }
                break;

            case STROBE_2:
                if (!memcmp_val(dat, 16, 0))
                {
                    //STB1 STB2
                    SMP640_Strobe(STB_1 + STB_2);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 16, 16, 0))
                {
                    //STB3 STB4
                    SMP640_Strobe(STB_3 + STB_4);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 32, 16, 0))
                {
                    //STB5 STB6
                    SMP640_Strobe(STB_5 + STB_6);
                    delay_ms(dms);
                    delay_us(dus);
                }
                break;

            case STROBE_3:
                if (!memcmp_val(dat, 24, 0))
                {
                    //STB1 STB2 STB3
                    SMP640_Strobe(STB_1 + STB_2 + STB_3);
                    delay_ms(dms);
                    delay_us(dus);
                }

                if (!memcmp_val(dat + 24, 24, 0))
                {
                    //STB4 STB5 STB6
                    SMP640_Strobe(STB_4 + STB_5 + STB_6);
                    delay_ms(dms);
                    delay_us(dus);
                }
                break;

            case STROBE_6:
                if (!memcmp_val(dat, 48, 0))
                {
                    SMP640_Strobe(STB_ALL);
                    delay_ms(dms);
                    delay_us(dus);
                }
                break;

            default:
                break;
            }
        }
    }

    SMP640_Strobe(STB_NUL);
    SMP640_Motor_DotLine();
    LED_WORKING = 1;
}

//打印一行, 数据为dat内数据, size为dat内所存数据的长度, <48字节后部补0, 加热delay毫秒
void SMP640_Print_Line(uint8_t *dat, uint8_t size, uint16_t delay)
{
    SMP640_Print_Line_Without_PowerDown(dat, size, delay);
    SMP640_VHEAT_OFF;
    SMP640_Motor_Step(MOTOR_STEP_STOP);
}

//检测是否有纸张, 有纸返回1
bit SMP640_PaperCheck()
{
    return !SMP640_CO;
}

#pragma endregion

#pragma region 步进电机

//当前电机步数
bit step = 0;

//电机走纸第step步
void SMP640_Motor_Step(uint8_t step)
{
    switch (step)
    {
    case 0:
        MOTOR_IAN = 0;
        MOTOR_IAP = 1;
        MOTOR_IBP = 1;
        MOTOR_IBN = 0;
        break;

    case 1:
        MOTOR_IAN = 1;
        MOTOR_IAP = 0;
        MOTOR_IBP = 1;
        MOTOR_IBN = 0;
        break;

    case 2:
        MOTOR_IAN = 1;
        MOTOR_IAP = 0;
        MOTOR_IBP = 0;
        MOTOR_IBN = 1;
        break;

    case 3:
        MOTOR_IAN = 0;
        MOTOR_IAP = 1;
        MOTOR_IBP = 0;
        MOTOR_IBN = 1;
        break;

    default:
        MOTOR_IAN = 0;
        MOTOR_IAP = 0;
        MOTOR_IBP = 0;
        MOTOR_IBN = 0;
        break;
    }
}

//电机走纸一点行
void SMP640_Motor_DotLine()
{
    if (step)
    {
        step = 0;
        SMP640_Motor_Step(0);
        delay_us(MOTOR_DELAY_US);
        SMP640_Motor_Step(1);
    }
    else
    {
        step = 1;
        SMP640_Motor_Step(2);
        delay_us(MOTOR_DELAY_US);
        SMP640_Motor_Step(3);
    }
    delay_us(MOTOR_DELAY_US);
}

#pragma endregion

#pragma region 缓冲区

//打印缓冲区
uint8_t xdata SMP640_Buffer[SMP640_BUF_SIZE][SMP640_LINE_SIZE];
uint8_t xdata SMP640_Buffer_Available_Line[SMP640_BUF_SIZE / 8 + 1];

//从可用行表读取某一行数据是否可用
bit SMP640_Buffer_IsLineAvailable(uint16_t line)
{
    if (line > SMP640_BUF_SIZE - 1)
    {
        return 0;
    }

    return (SMP640_Buffer_Available_Line[line / 8] >> (line % 8)) & 1;
}

//顺序填充一行Buffer, 从dat所指内存读取SMP640_LINE_SIZE个数据, 并维护可用行表, 返回值指示操作是否成功
bit SMP640_Buffer_FillLine(uint8_t *dat)
{
    uint8_t line = 0;

    line = SMP640_BUFFER_LINE_NEXT_EMPTY();
    if (line == 0xffff)
        return 0;

    memcpy(&SMP640_Buffer[line][0], dat, SMP640_LINE_SIZE);
    SMP640_Buffer_Available_Line[line / 8] |= (1 << (line % 8));
    return 1;
}

//获取具有指定可用性的下一行
uint16_t SMP640_Buffer_NextLine(bit available)
{
    uint16_t line;

    for (line = 0; line < SMP640_BUF_SIZE; line++)
    {
        if (SMP640_Buffer_IsLineAvailable(line) == available)
        {
            return line;
        }
    }
    return 0xffff;
}
#pragma endregion