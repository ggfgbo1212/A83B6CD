/*
***************************************************************************
* Copyright (C) 2022-2026 Ismartware Limited. All rights reserved.
* File name: i2c_ota_policy.h
* Description: I2C从机OTA功能接口，通过寄存器0xAA触发升级
* Version: V1.0
* Date:
* History:
***************************************************************************
*/
// 主机协议（两笔独立事务）：
// 1. 写命令：START + 地址(写) + 0xAA + 01 C0 00 2B 08 + STOP
// 2. 读应答：START + 地址(写) + 0xAA + Sr + 地址(读) + 7字节 + STOP
// 应答内容：AA 03 00 FF FF 3D 8C（CRC-16，多项式0x8005，初值0x0000，不反转）
// 应答发送完成后MCU复位，跳转至厂商Bootloader
//
// 注意：I2CS_Handler()的RX分支调用I2c_Ota_On_Rx_Byte()时，不能清除
//       I2C_S_RX_DATA_PENGING，否则会覆盖S_STATUS中的帧地址和长度信息。
#ifndef I2C_OTA_POLICY_H
#define I2C_OTA_POLICY_H

#include "system.h"

// OTA专用寄存器地址（本工程空闲）
#define I2C_OTA_REG_ADDR    (0xAAU)

// 0xAA命令帧收到首字节时调用：挂起RX中断，等待STOP（不能清除RX挂起标志）
void I2c_Ota_On_Rx_Byte(void);

// STOP中断中调用：校验5字节命令，匹配则预装载7字节应答；无OTA帧时直接返回
void I2c_Ota_On_Finish(void);

// 主机开始读取应答时调用：标记复位，由主循环执行
void I2c_Ota_On_Tx_Request(void);

// 主循环中调用：等待应答发送完成后复位，进入Bootloader
void I2c_Ota_Policy_Run(void);

#endif
