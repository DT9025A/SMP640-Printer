#ifndef __POWER_H_
#define __POWER_H_

#include <STC8H.h>
#include "adc.h"
#include "defines.h"

typedef struct _PowerStatus
{
    //��һ�ζ�ȡ����CPU��ѹ/mv
    float voltage;
    //����USB�˿ڹ���
    uint8_t USB_TRANS_POWER : 1;
    //����USB�˿ڹ���: USB_TRANS_POWER=0 && pwr>3.2v
    uint8_t USB_EXTERN_POWER : 1;
    //��ǰϵͳ�Ƿ��ڵ͵�ѹ״̬
    uint8_t LOW_POWER : 1;
} PowerStatus;

//��Դ������Ϣ
extern PowerStatus SysPowerStatus;

//��ȡ��ǰ��CPU��ѹ
float GetPowerVoltage();
//��ȡ��ǰ�ĵ�Դ��Ϣ, ���������SysPowerStatus, ��������&SysPowerStatus
PowerStatus *GetPowerStatus();

#endif