/*
***************************************************************************
* Copyright (C) 2020-2020 Ismartware Limited. All rights reserved. 
* File name: sw3516p_func.h
* Description: api for sw3566 
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date:15/02/23 
* History: none
***************************************************************************
*/

#ifndef __SW3566P_I2C_LIB_H__
#define __SW3566P_I2C_LIB_H__


#include "zr_i2c.h" 
#include "zr_rcc.h"
#include "zr_gpio.h"
#include "zr_nvic.h"
#include "api.h"
#include "device_policy.h"

#define I2C_ADDR_BASE 0x37

enum sw3566_i2c_reg
{
	//Read Addr 分配29字节
	PORT1_STATE_REG = 6, //读空间从6-34。
	PORT2_STATE_REG,
	REQ_VOL_REG,
	
	//Write Addr 分配29字节
	POWER_REG = 35, //读写空间分离。
	WAKE_REG = 36,
}; 

typedef struct 
{
	u8 i2cBuf[64];
	
	volatile u8 revFlag;
}usr_i2c_driver_t;

void I2C_Drive_Init(uint8_t slaveAddr);

u8 I2c_Write_Gruop0(u8 devAddr, u8 writeAddr, u8 *dataToWrite,u8 wLen);
u8 I2c_Read_Gruop0(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen);

u8 I2c_Write_Gruop1(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen);
u8 I2c_Read_Gruop1(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen);

u8 I2c_Write_Gruop2(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen);
u8 I2c_Read_Gruop2(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen);

u8 I2c_Write_Gruop3(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen);
u8 I2c_Read_Gruop3(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen);

void I2c_Slave_Drive_Init(uint8_t slaveAddr);
void i2c_Register_Time(u32 (* tmpTimePass)(u32 tm), u32 (* tmpTimeMark)());

#endif