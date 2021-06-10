#include "adc.h"

void ADC_Init (unsigned char channel) {
    //通道引脚设为高阻模式

    //SELECT CONVERT PIN
    ADC_CONTR = channel;

    //SPEED
    ADCCFG = ADC_SPEED;

    //DATA MODE
    ADCCFG |= 0x20;

    //POWER
    ADC_CONTR |= 0x80;
}

int ADC_Convert() {
    //START
    ADC_CONTR |= 0x40;
    _nop_();
    _nop_();

    //CHECK FLAG
    while (! (ADC_CONTR & 0x20));

    //CLEAR FLAG
    ADC_CONTR &= ~0x20;

    return (int)ADC_RES << 8 | ADC_RESL;
}

int ADC_Read (unsigned char channel) {
    ADC_Init (channel);
    return ADC_Convert();
}

CALC_TYPE ADC_ReadValueWithReference (unsigned char channel) {
    CALC_TYPE m;

    m = (CALC_TYPE)ADC_REF * ADC_MAX / ADC_Read (ADC_REFCH);
    return (CALC_TYPE)ADC_Read (channel) / ADC_MAX * m;
}