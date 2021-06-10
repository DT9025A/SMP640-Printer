/*********************
 *     ADC操作
 * 
 * @Author:DT9025A
 *********************/

#ifndef __ADC_H_
#define __ADC_H_

#include <STC8H.h>
#include <intrins.h>

//10位ADC可取得的最大值 10位为2^10-1=1023
#define ADC_MAX     1023
//ADC内部参考电压/mv
#define ADC_REF     1193
//ADC内部参考电压连接到的通道
#define ADC_REFCH  15
//ADC时钟速度=SysClk/2/(ADC_SPEED+1)
#define ADC_SPEED   15
//计算数据类型
#define CALC_TYPE float

//初始化ADC通道
void ADC_Init (unsigned char pin);
//进行一次ADC转换
int ADC_Convert();
//读取一次ADC转换结果
int ADC_Read (unsigned char pin);
//返回基于参考电压的mv结果
CALC_TYPE ADC_ReadValueWithReference (unsigned char pin);

#endif