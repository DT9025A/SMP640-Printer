/************************
 * STC8系列 IO配置
 * @Author  DT9025A
 * @Date    21/1/15
 * @License Apache License 2.0
 * 
 * 本文件基于STC8H的文档编写, 根据目标平台自行适配
 ***********************/
#ifndef __STC8_IO_CONFIG_H__
#define __STC8_IO_CONFIG_H__

#include <STC8H.h>

#ifdef __STC8H_H_
#warning 默认头文件未更改, 目标平台为STC8H
#endif

//IO定义, 使用时将其加和
/**
 * Pn.0
 */
#define PIN_0 0x01
/**
 * Pn.1
 */
#define PIN_1 0x02
/**
 * Pn.2
 */
#define PIN_2 0x04
/**
 * Pn.3
 */
#define PIN_3 0x08
/**
 * Pn.4
 */
#define PIN_4 0x10
/**
 * Pn.5
 */
#define PIN_5 0x20
/**
 * Pn.6
 */
#define PIN_6 0x40
/**
 * Pn.7
 */
#define PIN_7 0x80
/**
 * 当前端口的所有IO
 */
#define PIN_ALL 0xFF

//IO模式
/**
 * 标准双向
 */
#define PIN_MODE_STANDARD 0, 0
/**
 * 推挽输出
 */
#define PIN_MODE_PUSHPULL 1, 0
/**
 * 高阻输入
 */
#define PIN_MODE_HIRGRESIN 0, 1
/**
 * 开漏输出
 */
#define PIN_MODE_OPENDRAIN 1, 1

//PULLUP状态
/**
 * 开启上拉
 */
#define PIN_PULLUP_ENABLE 1
/**
 * 关闭上拉
 */
#define PIN_PULLUP_DISABLE 0

//NCS状态
/**
 * 
 */
#define PIN_NCS_ENABLE 0
/**
 * 
 */
#define PIN_NCS_DISABLE 1

//SR状态
#define PIN_SWITCHRATE_HIGH 0
#define PIN_SWITCHRATE_LOW 1

//DR状态
#define PIN_DRAINRATE_HIGH 0
#define PIN_DRAINRATE_LOW 1

//IE状态
#define PIN_DIGITALSIGINPUTE_DISABLE 0
#define PIN_DIGITALSIGINPUTE_ENABLE 1

//IO模式
/**
 * 设置IO模式
 * @param port 端口(例: P0)
 * @param pin 引脚(PIN_n)
 * @param mode 模式
 */
#define PIN_MODE_CONFIG(port, pin, mode) _PINMODE_CONFIG(port, pin, mode)
#define _PINMODE_CONFIG(port, pin, stat0, stat1)      \
    stat0 ? (port##M0 |= pin) : (port##M0 &= (~pin)); \
    stat1 ? (port##M1 |= pin) : (port##M1 &= (~pin));

//上拉电阻设置, 进行设置前需先将P_SW2的最高位置1
#define PIN_PULLUP_CONFIG(port, pin, stat) \
    stat ? (port##PU |= pin) : (port##PU &= (~pin));

//施密特触发器设置, 进行设置前需先将P_SW2的最高位置1
#define PIN_NCS_CONFIG(port, pin, stat) \
    stat ? (port##NCS |= pin) : (port##NCS &= (~pin));

//端口转换速率设置, 进行设置前需先将P_SW2的最高位置1
#define PIN_SWITCHRATE_CONFIG(port, pin, stat) \
    stat ? (port##SR |= pin) : (port##SR &= (~pin));

//端口驱动电流设置, 进行设置前需先将P_SW2的最高位置1
#define PIN_DRAINRATE_CONFIG(port, pin, stat) \
    stat ? (port##DR |= pin) : (port##DR &= (~pin));

//数字信号使能设置, 进行设置前需先将P_SW2的最高位置1
#define PIN_DIGITALSIGINPUTENABLE_CONFIG(port, pin, stat) \
    stat ? (port##IE |= pin) : (port##IE &= (~pin));

#endif
