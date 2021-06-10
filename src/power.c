#include "power.h"

PowerStatus SysPowerStatus;

float GetPowerVoltage()
{
    return (float)ADC_REF * 1024 / ADC_Read(ADC_REFCH);
}

PowerStatus *GetPowerStatus()
{
    SysPowerStatus.voltage = GetPowerVoltage();
    if (USB_PWR_TEST)
        SysPowerStatus.USB_TRANS_POWER = 1;
    else
    {
        SysPowerStatus.USB_TRANS_POWER = 0;
        if (SysPowerStatus.voltage > 3200.0f)
            SysPowerStatus.USB_EXTERN_POWER = 1;
        else
            SysPowerStatus.USB_EXTERN_POWER = 0;
    }
    if (SysPowerStatus.voltage < 3000.0f)
        SysPowerStatus.LOW_POWER = 1;
    else
        SysPowerStatus.LOW_POWER = 0;
    return &SysPowerStatus;
}