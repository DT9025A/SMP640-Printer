/*********************
 *       —” ±
 * 
 * @Author:DT9025A
 *********************/

#ifndef __DELAY_H__
#define __DELAY_H__

#include <STC8H.H>
#include "defines.h"
#include "intrins.h"

void delay_ms(uint16_t ms);
void delay_us(uint16_t us);
void delay_1ms();
void delay_1us();

#endif