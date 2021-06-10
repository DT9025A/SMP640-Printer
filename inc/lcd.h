/*********************
 *     LCD����
 * 
 * @Author:DT9025A
 *********************/

#ifndef __LCD_H__
#define __LCD_H__

#include <STC8H.H>
#include "defines.h"
#include "delay.h"
#include "smp640.h"

//С�ַ��б�
enum SMALL_CHAR {
    CH_PRINTING2 = 0,
    CH_PAPERNOTEXISTS,
    CH_PAPEREXISTS,
    CH_IDLE,
    CH_PRINTING,
    CH_CESIUS,
    CH_OVERHEAT,
    CH_TEMPOK
};

//��ӡ״̬
enum STATUS
{
    TRANSFERING,
    PRINTING,
    IDLE,
    STOP
};

//Һ����ʼ��
void LCD_Init(void);
//Һ������
void LCD_Clear(void);
//Һ����λ
void LCD_SetPosition(unsigned char x, unsigned char y);
//�ַ���λ���
void LCD_PrintChar(unsigned char x, unsigned char y, unsigned char c);
//�ַ�����λ���
void LCD_PrintString(unsigned char x, unsigned char y, unsigned char *s);
// ��LCD��һ����ʾ����״̬
void LCD_ShowStatus(uint8_t s);

#endif