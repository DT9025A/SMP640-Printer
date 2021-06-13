#ifndef __SMP640_H__
#define __SMP640_H__

#include "adc.h"
#include "defines.h"
#include "delay.h"
#include "spi.h"
#include <STC8H.H>
#include <math.h>
#include <string.h>

/*************����*************/
//��ӡͷһ�е�������
#define SMP640_LINE_SIZE 48

//����һ�������������
void SMP640_Latch();

//��մ�ӡͷ�ڵĻ�������
void SMP640_Clear_Register();

//�ж�SMP640�Ƿ����
bit SMP640_Exists();
/*************����*************/


/*************������*************/
//��ӡͷ����������
#define SMP640_BUF_SIZE 1

//��ʼ����ӡ�������Ϳ����б�
#define SMP640_BUFFER_INIT                                       \
    memset(SMP640_Buffer, 0, SMP640_BUF_SIZE *SMP640_LINE_SIZE); \
    memset(SMP640_Buffer_Available_Line, 0x00, SMP640_BUF_SIZE / 8 + 1)

//��տ����б���ĳһ�еĿ���״̬
#define SMP640_BUFFER_LINE_AVAILABLE_CLEAR(line) \
    SMP640_Buffer_Available_Line[line / 8] &= ~(1 << (line % 8));

//ָ�뵽ĳһ��
#define SMP640_BUFFER_LINE_PTR(n) &SMP640_Buffer[n][0]

//��ȡ��һ���ݿ�����
#define SMP640_BUFFER_LINE_NEXT_AVAILABLE() SMP640_Buffer_NextLine(1)

//��ȡ��һ����
#define SMP640_BUFFER_LINE_NEXT_EMPTY() SMP640_Buffer_NextLine(0)

//��ӡ������
extern uint8_t xdata SMP640_Buffer[SMP640_BUF_SIZE][SMP640_LINE_SIZE];

//�����������б�
extern uint8_t xdata SMP640_Buffer_Available_Line[SMP640_BUF_SIZE / 8 + 1];

//˳�����һ��Buffer, ��dat��ָ�ڴ��ȡSMP640_LINE_SIZE������, ��ά�������б�, ����ֵָʾ�����Ƿ�ɹ�
bit SMP640_Buffer_FillLine(uint8_t *dat);

//�ӿ����б��ȡĳһ�������Ƿ����
bit SMP640_Buffer_IsLineAvailable(uint16_t line);

//��ȡ����ָ�������Ե���һ��, û�з���0xffff
uint16_t SMP640_Buffer_NextLine(bit available);
/*************������*************/


/*************�������*************/
//ֹͣ����ĵ������
#define MOTOR_STEP_STOP 4

//�������������ʱ��
#define MOTOR_DELAY_US 800

//�����ֽ��step��
void SMP640_Motor_Step(uint8_t step);

//�����ֽһ����
void SMP640_Motor_DotLine();
/*************�������*************/


/*************����*************/
//STROBE����
#define STB_1 0x40
#define STB_2 0x20
#define STB_3 0x10
#define STB_4 0x08
#define STB_5 0x04
#define STB_6 0x02
#define STB_ALL 0xff
#define STB_NUL 0x00

//�򿪼��ȵ�Դ
#define SMP640_VHEAT_ON SMP640_VHEAT = 1

//�رռ��ȵ�Դ
#define SMP640_VHEAT_OFF SMP640_VHEAT = 0

//��/�ص�ǰ�еļ���, ����ָ������STB��
void SMP640_Strobe(uint8_t stb);

//��ȡ����ͷ�¶�
float SMP640_Heat_Temp();
/*************����*************/


/*************��ӡ*************/
//����ѡͨģʽ
enum STB_MODE {
    STROBE_1,
    STROBE_2,
    STROBE_3,
    STROBE_6
};

//��ӡһ��, ����Ϊdat������, sizeΪdat���������ݵĳ���, <48�ֽں󲿲�0, ����delay΢��
void SMP640_Print_Line(uint8_t *dat, uint8_t size, uint16_t delay);

//��ӡһ��, ����Ϊdat������, sizeΪdat���������ݵĳ���, <48�ֽں󲿲�0, ����delay΢��, ���رյ�Դ
void SMP640_Print_Line_Without_PowerDown(uint8_t *dat, uint8_t size, uint16_t delay);

//����Ƿ���ֽ��, ��ֽ����1
bit SMP640_PaperCheck();
/*************��ӡ*************/

#endif
