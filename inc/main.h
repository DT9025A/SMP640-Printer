/*********************
 *     ��ͷ�ļ�
 * 
 * @Author:DT9025A
 *********************/

#ifndef __MAIN_H__
#define __MAIN_H__

#define _USE_ESC_POS_

#include "adc.h"
#include "defines.h"
#include "ioconfig.h"
#include "lcd.h"
#include "pic.h"
#include "power.h"
#include "protocol.h"
#include "esc-pos.h"
#include "smp640.h"
#include "spi.h"
#include "uart.h"
#include "flash.h"
#include <STC8H.H>
#include <stdio.h>

//���ȱ���
#define TEMP_ALERT 50
//���ڴ��¶�ȡ�����ȱ���
#define TEMP_DEALERT 35
//��
#define POWERDOWN_LOOP_COUNT 10

//ϵͳ�¶�
extern float SysTempearature;
//LCD��ʾ������
extern uint8_t LCD_Buffer[8];
//�Ƿ����
extern bit isOverHeating;
//�����ӡ, ����Ϊֹͣ��ӡ (��uart.c����)
extern bit enablePrinting;
//��Դ������Ϣ (��power.c����)
extern PowerStatus SysPowerStatus;
//����ģʽ (��smp640.c����)
extern uint8_t strobeMode;

//��ʼ��IO
void IO_Init();
//��ʼ���ж�
void Interrupt_Init();
//��ȡ������Ļ����ʾ��ǰ״̬
void LCD_ShowData();

#endif