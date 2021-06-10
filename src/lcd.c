#include "lcd.h"

#define LCD_Command(x) LCD_DataOut(0, x)
#define LCD_Data(x) LCD_DataOut(1, x)

uint8_t lastState; //��һ��ʾ״̬

/*--------------------------------------------------------------*/
//����ʹ��
void LCD_Enable(void)
{
    LCD_EN = 1;
    delay_us(10);
    LCD_EN = 0;
}

/*--------------------------------------------------------------*/
//�������
void LCD_DataOut(unsigned char rs, unsigned char cmd)
{
    delay_us(10);
    LCD_RS = rs;

    LCD_DB7 = cmd & 0x80;
    LCD_DB6 = cmd & 0x40;
    LCD_DB5 = cmd & 0x20;
    LCD_DB4 = cmd & 0x10;
    LCD_Enable();
    delay_us(10);

    LCD_DB7 = cmd & 0x08;
    LCD_DB6 = cmd & 0x04;
    LCD_DB5 = cmd & 0x02;
    LCD_DB4 = cmd & 0x01;
    LCD_Enable();
    delay_ms(1);
}

/*--------------------------------------------------------------*/
//С�ַ�
unsigned char code Xword[] = {
    0X00, 0X15, 0X0E, 0X15, 0X1F, 0X11, 0X1F, 0X00, //��ӡ��2�� ���� 0x00
    0X00, 0X1F, 0X11, 0X11, 0X11, 0X11, 0X1F, 0X00, //ȱֽ��    ���� 0x01
    0X00, 0X11, 0X1F, 0X11, 0X1F, 0X11, 0X1F, 0X00, //ֽ�ž��������� 0x02
    0X00, 0X0A, 0X00, 0X11, 0X1F, 0X11, 0X1F, 0X00, //��ӡ���ã����� 0x03
    0X15, 0X0E, 0X04, 0X11, 0X1F, 0X11, 0X1F, 0X00, //��ӡ�У�  ���� 0x04
    0x18, 0x18, 0x07, 0x08, 0x08, 0x08, 0x07, 0x00, //�棬       ���� 0x05
    0X1D, 0X09, 0X09, 0X01, 0X15, 0X1C, 0X15, 0X00, //�¶ȹ��ߣ����� 0x06
    0X1C, 0X08, 0X08, 0X01, 0X12, 0X0C, 0X08, 0X00  //�¶����������� 0x07
};

// װ��CGRAM
void LCD_WriteCGRAM(void)
{
    unsigned char i;
    LCD_Command(0x06); // CGRAM��ַ�Զ���1
    LCD_Command(0x40); // CGRAM��ַ��Ϊ00��
    for (i = 0; i < 64; i++)
    {
        LCD_Data(Xword[i]); // ������д������
    }
}

/*--------------------------------------------------------------*/
//Һ����ʼ��
//��ʼ����֪��Ϊʲô���׳��� ������ʱ
void LCD_Init(void)
{
    LCD_RW = 0;
    delay_ms(1);
    LCD_Command(0x28);
    delay_ms(1);
    LCD_Enable();
    LCD_Command(0x28); //������ʾ
    delay_ms(1);
    LCD_WriteCGRAM(); //�Զ����ַ�
    delay_ms(1);
    LCD_Command(0x01); //��ʾ����
    delay_ms(1);
    LCD_Command(0x0c); //��ʾ��
    delay_ms(1);
    LCD_Command(0x02); //ָ�����
    delay_ms(10);
}

/*--------------------------------------------------------------*/
//Һ������
void LCD_Clear(void)
{
    LCD_Command(0x01);
    LCD_Command(0x02); //ָ�����
    delay_ms(1);
}

/*--------------------------------------------------------------*/
//Һ����λ
void LCD_SetPosition(unsigned char x, unsigned char y)
{
    if (y)
        LCD_Command(x | 0xc0);
    else
        LCD_Command(x | 0x80);
}

/*--------------------------------------------------------------*/
//�ַ���λ���
void LCD_PrintChar(unsigned char x, unsigned char y, unsigned char c)
{
    LCD_SetPosition(x, y);
    LCD_Data(c);
}

/*--------------------------------------------------------------*/
//�ַ�����λ���
void LCD_PrintString(unsigned char x, unsigned char y, unsigned char *s)
{
    LCD_SetPosition(x, y);
    while (*s)
    {
        LCD_Data(*s);
        s++;
    }
}

/*--------------------------------------------------------------*/
// ��LCD��һ����ʾ����״̬
void LCD_ShowStatus(uint8_t s)
{
    //��Ҫ����״̬
    if (lastState != s)
    {
        LCD_PrintChar(10, 1, CH_IDLE);
        //LED_TRANSFERING = 1;
        lastState = s;

        switch (s)
        {
        case TRANSFERING:
            LCD_PrintString(0, 0, "  Transfering.  ");
            //LED_TRANSFERING = 0;
            break;

        case PRINTING:
            LCD_PrintString(0, 0, "    Printing    ");
            LCD_PrintChar(10, 1, CH_PRINTING);
            break;

        case IDLE:
            LCD_PrintString(0, 0, "      Idle      ");
            SMP640_VHEAT_OFF;
            SMP640_Motor_Step(MOTOR_STEP_STOP); //�رյ����STB
            break;

        case STOP:
            LCD_PrintString(0, 0, "      STOP      ");
            SMP640_VHEAT_OFF;
            SMP640_Motor_Step(MOTOR_STEP_STOP); //�رյ����STB
            break;

        default:
            LCD_PrintString(0, 0, "    UNKNOWN.    ");
            SMP640_VHEAT_OFF;
            SMP640_Motor_Step(MOTOR_STEP_STOP); //�رյ����STB
            break;
        }
    }
}