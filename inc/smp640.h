#ifndef __SMP640_H__
#define __SMP640_H__

#include "adc.h"
#include "defines.h"
#include "delay.h"
#include "spi.h"
#include <STC8H.H>
#include <math.h>
#include <string.h>

/*************数据*************/
//打印头一行的数据量
#define SMP640_LINE_SIZE 48

//进行一次数据锁存操作
void SMP640_Latch();

//清空打印头内的缓存数据
void SMP640_Clear_Register();

//判断SMP640是否存在
bit SMP640_Exists();
/*************数据*************/


/*************缓冲区*************/
//打印头缓冲区行数
#define SMP640_BUF_SIZE 1

//初始化打印缓冲区和可用行表
#define SMP640_BUFFER_INIT                                       \
    memset(SMP640_Buffer, 0, SMP640_BUF_SIZE *SMP640_LINE_SIZE); \
    memset(SMP640_Buffer_Available_Line, 0x00, SMP640_BUF_SIZE / 8 + 1)

//清空可用行表中某一行的可用状态
#define SMP640_BUFFER_LINE_AVAILABLE_CLEAR(line) \
    SMP640_Buffer_Available_Line[line / 8] &= ~(1 << (line % 8));

//指针到某一行
#define SMP640_BUFFER_LINE_PTR(n) &SMP640_Buffer[n][0]

//获取下一数据可用行
#define SMP640_BUFFER_LINE_NEXT_AVAILABLE() SMP640_Buffer_NextLine(1)

//获取下一空行
#define SMP640_BUFFER_LINE_NEXT_EMPTY() SMP640_Buffer_NextLine(0)

//打印缓冲区
extern uint8_t xdata SMP640_Buffer[SMP640_BUF_SIZE][SMP640_LINE_SIZE];

//缓冲区可用行表
extern uint8_t xdata SMP640_Buffer_Available_Line[SMP640_BUF_SIZE / 8 + 1];

//顺序填充一行Buffer, 从dat所指内存读取SMP640_LINE_SIZE个数据, 并维护可用行表, 返回值指示操作是否成功
bit SMP640_Buffer_FillLine(uint8_t *dat);

//从可用行表读取某一行数据是否可用
bit SMP640_Buffer_IsLineAvailable(uint16_t line);

//获取具有指定可用性的下一行, 没有返回0xffff
uint16_t SMP640_Buffer_NextLine(bit available);
/*************缓冲区*************/


/*************步进电机*************/
//停止电机的电机步数
#define MOTOR_STEP_STOP 4

//步进电机换相间隔时长
#define MOTOR_DELAY_US 800

//电机走纸第step步
void SMP640_Motor_Step(uint8_t step);

//电机走纸一点行
void SMP640_Motor_DotLine();
/*************步进电机*************/


/*************加热*************/
//STROBE控制
#define STB_1 0x40
#define STB_2 0x20
#define STB_3 0x10
#define STB_4 0x08
#define STB_5 0x04
#define STB_6 0x02
#define STB_ALL 0xff
#define STB_NUL 0x00

//打开加热电源
#define SMP640_VHEAT_ON SMP640_VHEAT = 1

//关闭加热电源
#define SMP640_VHEAT_OFF SMP640_VHEAT = 0

//开/关当前行的加热, 参数指定加热STB号
void SMP640_Strobe(uint8_t stb);

//获取加热头温度
float SMP640_Heat_Temp();
/*************加热*************/


/*************打印*************/
//加热选通模式
enum STB_MODE {
    STROBE_1,
    STROBE_2,
    STROBE_3,
    STROBE_6
};

//打印一行, 数据为dat内数据, size为dat内所存数据的长度, <48字节后部补0, 加热delay微秒
void SMP640_Print_Line(uint8_t *dat, uint8_t size, uint16_t delay);

//打印一行, 数据为dat内数据, size为dat内所存数据的长度, <48字节后部补0, 加热delay微秒, 不关闭电源
void SMP640_Print_Line_Without_PowerDown(uint8_t *dat, uint8_t size, uint16_t delay);

//检测是否有纸张, 有纸返回1
bit SMP640_PaperCheck();
/*************打印*************/

#endif
