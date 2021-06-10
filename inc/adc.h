/*********************
 *     ADC����
 * 
 * @Author:DT9025A
 *********************/

#ifndef __ADC_H_
#define __ADC_H_

#include <STC8H.h>
#include <intrins.h>

//10λADC��ȡ�õ����ֵ 10λΪ2^10-1=1023
#define ADC_MAX     1023
//ADC�ڲ��ο���ѹ/mv
#define ADC_REF     1193
//ADC�ڲ��ο���ѹ���ӵ���ͨ��
#define ADC_REFCH  15
//ADCʱ���ٶ�=SysClk/2/(ADC_SPEED+1)
#define ADC_SPEED   15
//������������
#define CALC_TYPE float

//��ʼ��ADCͨ��
void ADC_Init (unsigned char pin);
//����һ��ADCת��
int ADC_Convert();
//��ȡһ��ADCת�����
int ADC_Read (unsigned char pin);
//���ػ��ڲο���ѹ��mv���
CALC_TYPE ADC_ReadValueWithReference (unsigned char pin);

#endif