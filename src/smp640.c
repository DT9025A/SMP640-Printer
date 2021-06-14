#include "smp640.h"

#pragma region �������

//����һ�������������
void SMP640_Latch()
{
    SMP640_LATCH = 0;
    delay_us(10);
    SMP640_LATCH = 1;
}

//��մ�ӡͷ�ڵĻ�������
void SMP640_Clear_Register()
{
    unsigned char p;

    for (p = 0; p < 48; p++)
    {
        SPI_SendByte(0x00);
    }
    SMP640_Latch();
}

//595�����������, ����STB
void HC595_Latch()
{
    HC595_LATCH = 1;
    delay_us(10);
    HC595_LATCH = 0;
}

//�ж�SMP640�Ƿ����
bit SMP640_Exists()
{
    unsigned char j, k;
    bit accept = 1;

    SMP640_Clear_Register();
    for (j = 0; j < SMP640_LINE_SIZE; j++)
    {
        k = SPI_SendByte(j);
    }

    SPCTL |= CPHA; //�ֲ���д������ز���
    for (j = 0; j < SMP640_LINE_SIZE; j++)
    {
        k = SPI_SendByte(0) >> 1; //��һʱ��
        if (k != j)
            accept = 0;
    }

    SPI_Init(); //���³�ʼ��SPI
    SMP640_Clear_Register();
    return accept;
}
#pragma endregion

#pragma region ����

//����������㳣��
code float Rp = 30000.0;
code float T2 = (273.15 + 25.0);
code float Bx = 3950.0;
code float Ka = 273.15;
code float arg1 = 3950.0 / (273.15 + 25.0);

//��/�ص�ǰSTB�ļ���, ����ָ������STB��
void SMP640_Strobe(uint8_t stb)
{
    SPI_SendByte(stb);
    HC595_Latch();
}

//��ȡ����ͷ�¶�
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

#pragma region ��ӡ
uint8_t xdata strobeMode = STROBE_3;

//�Ƚ�����
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

    //������ȫ��0ֱ�Ӽ�Ϊ��ֽ
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
            ��������㹻����ͬʱѡͨ���STB, ������ӡ�ٶ�
            ��ѡͨ���STBֱ��Ӱ���ӡͷ�¶Ⱥʹ�ӡ����
            �����������������
            �Դ˿�����������ͷ������ѹ�����Ӽ���ʱ��
            ��ӡ�ٶ����ɴﵽ����ʱ��3ms,220-230��/�� 27-29cm/��, ��5V�¼���Ч������
            
            ����Ч���Ϻõļ������� (�޿հ�����):
            VHEAD=8V, STBȫ��, ����3ms, ������Ϊ7A����, ���¼���, 230��/��
            [�Ƽ�] VHEAD=8V, STB��3��, ÿ3������2ms, ������Ϊ2.5A����, 150��/��, 384������6��
            VHEAD=8V, STB��2��, ÿ2������2ms, ������Ϊ2.3A����, 110��/��
            [�Ƽ�] VHEAD=5V, STB��3��, ÿ3������7ms, ������Ϊ2.5A����, 60��/��, 384������5��
            VHEAD=5V, STB��2��, ÿ2������6ms, ������Ϊ2.5A����, 45��/��
            VHEAD=5V, STB��1��, ÿ1������6ms, ������Ϊ1.5A����, 30��/��
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

//��ӡһ��, ����Ϊdat������, sizeΪdat���������ݵĳ���, <48�ֽں󲿲�0, ����delay����
void SMP640_Print_Line(uint8_t *dat, uint8_t size, uint16_t delay)
{
    SMP640_Print_Line_Without_PowerDown(dat, size, delay);
    SMP640_VHEAT_OFF;
    SMP640_Motor_Step(MOTOR_STEP_STOP);
}

//����Ƿ���ֽ��, ��ֽ����1
bit SMP640_PaperCheck()
{
    return !SMP640_CO;
}

#pragma endregion

#pragma region �������

//��ǰ�������
bit step = 0;

//�����ֽ��step��
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

//�����ֽһ����
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

#pragma region ������

//��ӡ������
uint8_t xdata SMP640_Buffer[SMP640_BUF_SIZE][SMP640_LINE_SIZE];
uint8_t xdata SMP640_Buffer_Available_Line[SMP640_BUF_SIZE / 8 + 1];

//�ӿ����б��ȡĳһ�������Ƿ����
bit SMP640_Buffer_IsLineAvailable(uint16_t line)
{
    if (line > SMP640_BUF_SIZE - 1)
    {
        return 0;
    }

    return (SMP640_Buffer_Available_Line[line / 8] >> (line % 8)) & 1;
}

//˳�����һ��Buffer, ��dat��ָ�ڴ��ȡSMP640_LINE_SIZE������, ��ά�������б�, ����ֵָʾ�����Ƿ�ɹ�
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

//��ȡ����ָ�������Ե���һ��
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