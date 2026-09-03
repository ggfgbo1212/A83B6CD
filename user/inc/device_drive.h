/*
***************************************************************************
* Copyright (C) 2020-2020 Ismartware Limited. All rights reserved. 
* File name: ic_common.h
* Description: api for ic common use
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date:15/02/23 
* History: none
***************************************************************************/

#ifndef __DEVICE_DRIVE_H__
#define __DEVICE_DRIVE_H__

#include "device_policy.h" 

#define ON_OFF1_GPIO   	GPIO11
#define ON_OFF1_FUN_SEL GPIO11_FUNC_SEL_GPIO
#define ON_OFF1_0  	    Gpio_Set_Output_Value(ON_OFF1_GPIO, 0);
#define ON_OFF1_1  	   	Gpio_Set_Output_Value(ON_OFF1_GPIO, 1);

enum device_info_get_e
{
	READ_DEVICE_STATE,
	READ_DEVICE_VOL,
	READ_DEVICE_CUR,
	READ_DEVICE_TEMPER,
	READ_DEVICE_PROTOCOL,
	READ_DEVICE_REQ_INFO,
	SET_DEVICE_POWER,
	SET_DEVICE_REBROADCAST,
};


void IC_Device_Init(device_policy_t  *tmpDevicePolicy);

void IC_Device_Read(device_policy_t  *tmpDevicePolicy);

void ATM_Init_Normal(void);
u32 My_GetSystemTimeMark(void);
u32 My_GetSystemTimePass(u32 tm);

void Slave_Update_Chart(device_policy_t  *tmpDevicePolicy);

void Usr_Gpio_Init(void);

void My_Time_Delay(u32 tim);
#endif