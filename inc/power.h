#ifndef __POWER_H_
#define __POWER_H_

#include <STC8H.h>
#include "adc.h"
#include "defines.h"

typedef struct _PowerStatus
{
    //上一次读取到的CPU电压/mv
    float voltage;
    //数据USB端口供电
    uint8_t USB_TRANS_POWER : 1;
    //额外USB端口供电: USB_TRANS_POWER=0 && pwr>3.2v
    uint8_t USB_EXTERN_POWER : 1;
    //当前系统是否处于低电压状态
    uint8_t LOW_POWER : 1;
} PowerStatus;

//电源供电信息
extern PowerStatus SysPowerStatus;

//获取当前的CPU电压
float GetPowerVoltage();
//获取当前的电源信息, 结果储存在SysPowerStatus, 函数返回&SysPowerStatus
PowerStatus *GetPowerStatus();

#endif