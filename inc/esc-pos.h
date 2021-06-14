/**
 * ESC/POSָ�
 * 
 * ָ��֧���б�����:
 * ESC @ / 1B 40
 * ESC J / 1B 4A N  �ڴ˱�����Ϊ����ֽδ��ӡ����
 * ESC d / 1B 64 N  �ڴ˱�����Ϊ��ֽn * 8����
 * GS v 0 / 1D 76 30  �ڴ˱�����Ϊֱ�Ӵ�ӡ
 * 
 */

#ifndef __ESC_POS_H__
#define __ESC_POS_H__

#include "defines.h"
#include "lcd.h"
#include "smp640.h" //��ӡ
#include "uart.h"   //������

#define CMD_NUL 0x00
#define HT 0x09
#define LF 0x0A
#define CR 0x0D
#define ESC 0x1B
#define DLE 0x10
#define GS 0x1D
#define FS 0x1C
#define STX 0x02
#define US 0x1F
#define CAN 0x18
#define CLR 0x0C
#define EOT 0x04
#define EOF 0xFF

extern uint16_t heatTime;
extern uint16_t count;

void UART_Process_Data();

#endif
