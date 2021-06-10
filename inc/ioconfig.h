/************************
 * STC8ϵ�� IO����
 * @Author  DT9025A
 * @Date    21/1/15
 * @License Apache License 2.0
 * 
 * ���ļ�����STC8H���ĵ���д, ����Ŀ��ƽ̨��������
 ***********************/
#ifndef __STC8_IO_CONFIG_H__
#define __STC8_IO_CONFIG_H__

#include <STC8H.h>

#ifdef __STC8H_H_
#warning Ĭ��ͷ�ļ�δ����, Ŀ��ƽ̨ΪSTC8H
#endif

//IO����, ʹ��ʱ����Ӻ�
/**
 * Pn.0
 */
#define PIN_0 0x01
/**
 * Pn.1
 */
#define PIN_1 0x02
/**
 * Pn.2
 */
#define PIN_2 0x04
/**
 * Pn.3
 */
#define PIN_3 0x08
/**
 * Pn.4
 */
#define PIN_4 0x10
/**
 * Pn.5
 */
#define PIN_5 0x20
/**
 * Pn.6
 */
#define PIN_6 0x40
/**
 * Pn.7
 */
#define PIN_7 0x80
/**
 * ��ǰ�˿ڵ�����IO
 */
#define PIN_ALL 0xFF

//IOģʽ
/**
 * ��׼˫��
 */
#define PIN_MODE_STANDARD 0, 0
/**
 * �������
 */
#define PIN_MODE_PUSHPULL 1, 0
/**
 * ��������
 */
#define PIN_MODE_HIRGRESIN 0, 1
/**
 * ��©���
 */
#define PIN_MODE_OPENDRAIN 1, 1

//PULLUP״̬
/**
 * ��������
 */
#define PIN_PULLUP_ENABLE 1
/**
 * �ر�����
 */
#define PIN_PULLUP_DISABLE 0

//NCS״̬
/**
 * 
 */
#define PIN_NCS_ENABLE 0
/**
 * 
 */
#define PIN_NCS_DISABLE 1

//SR״̬
#define PIN_SWITCHRATE_HIGH 0
#define PIN_SWITCHRATE_LOW 1

//DR״̬
#define PIN_DRAINRATE_HIGH 0
#define PIN_DRAINRATE_LOW 1

//IE״̬
#define PIN_DIGITALSIGINPUTE_DISABLE 0
#define PIN_DIGITALSIGINPUTE_ENABLE 1

//IOģʽ
/**
 * ����IOģʽ
 * @param port �˿�(��: P0)
 * @param pin ����(PIN_n)
 * @param mode ģʽ
 */
#define PIN_MODE_CONFIG(port, pin, mode) _PINMODE_CONFIG(port, pin, mode)
#define _PINMODE_CONFIG(port, pin, stat0, stat1)      \
    stat0 ? (port##M0 |= pin) : (port##M0 &= (~pin)); \
    stat1 ? (port##M1 |= pin) : (port##M1 &= (~pin));

//������������, ��������ǰ���Ƚ�P_SW2�����λ��1
#define PIN_PULLUP_CONFIG(port, pin, stat) \
    stat ? (port##PU |= pin) : (port##PU &= (~pin));

//ʩ���ش���������, ��������ǰ���Ƚ�P_SW2�����λ��1
#define PIN_NCS_CONFIG(port, pin, stat) \
    stat ? (port##NCS |= pin) : (port##NCS &= (~pin));

//�˿�ת����������, ��������ǰ���Ƚ�P_SW2�����λ��1
#define PIN_SWITCHRATE_CONFIG(port, pin, stat) \
    stat ? (port##SR |= pin) : (port##SR &= (~pin));

//�˿�������������, ��������ǰ���Ƚ�P_SW2�����λ��1
#define PIN_DRAINRATE_CONFIG(port, pin, stat) \
    stat ? (port##DR |= pin) : (port##DR &= (~pin));

//�����ź�ʹ������, ��������ǰ���Ƚ�P_SW2�����λ��1
#define PIN_DIGITALSIGINPUTENABLE_CONFIG(port, pin, stat) \
    stat ? (port##IE |= pin) : (port##IE &= (~pin));

#endif
