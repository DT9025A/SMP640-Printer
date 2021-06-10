/*********************
 *     各种定义
 * 
 * @Author:DT9025A
 *********************/

#ifndef __IODEF_H__
#define __IODEF_H__

#include <STC8H.H>

/*****************typedefs*****************/
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int32_t;
/*****************typedefs*****************/


/*****************IO*****************/
//------P0------
sbit MOTOR_IBP = P0 ^ 0;
sbit MOTOR_IBN = P0 ^ 1;
sbit MOTOR_IAP = P0 ^ 2;
sbit MOTOR_IAN = P0 ^ 3;

//------P1------
//sbit SMP640_TM = P1 ^ 0;
sbit BUZZER = P1 ^ 1;
sbit MOSI = P1 ^ 3;
sbit MISO = P1 ^ 4;
sbit SCLK = P1 ^ 5;
sbit W25Q_CS = P1 ^ 6;
sbit HC595_LATCH = P1 ^ 7;

//------P2------
sbit LCD_RS = P2 ^ 0;
sbit LCD_RW = P2 ^ 1;
sbit LCD_EN = P2 ^ 2;
sbit LCD_DB4 = P2 ^ 3;
sbit LCD_DB5 = P2 ^ 4;
sbit LCD_DB6 = P2 ^ 5;
sbit LCD_DB7 = P2 ^ 6;
sbit SMP640_VHEAT = P2 ^ 7;

//------P3------
//sbit RXD = P3 ^ 0;
//sbit TXD = P3 ^ 1;
sbit KEYA = P3 ^ 2;
sbit KEYB = P3 ^ 3;
sbit LED_WORKING = P3 ^ 4;
sbit LED_TRANSFERING = P3 ^ 5;
sbit SMP640_CO = P3 ^ 6;
sbit SMP640_LATCH = P3 ^ 7;

//------P5------
sbit USB_PWR_TEST = P5 ^ 4;
/*****************IO*****************/

#endif