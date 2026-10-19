/*
***************************************************************************
* Copyright (C) 2022-2023 Ismartware Limited. All rights reserved. 
* File name: i2c_slave_reg.h
* Description: functions about ic device to drive the hard device .
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date: 
* History: 
***************************************************************************
*/

#define regValue(x)		reg_new_data[x]

#define UFCS_PPS0_MIN_VOL_LOW_REG	0x60
#define UFCS_PPS0_MIN_VOL_HI_REG	0x61
#define UFCS_PPS1_MIN_VOL_LOW_REG	0x62
#define UFCS_PPS1_MIN_VOL_HI_REG	0x63
#define UFCS_PPS2_MIN_VOL_LOW_REG	0x64
#define UFCS_PPS2_MIN_VOL_HI_REG	0x65

#define UFCS_PPS0_MAX_VOL_LOW_REG	0x68
#define UFCS_PPS0_MAX_VOL_HI_REG	0x69
#define UFCS_PPS1_MAX_VOL_LOW_REG	0x6A
#define UFCS_PPS1_MAX_VOL_HI_REG	0x6B
#define UFCS_PPS2_MAX_VOL_LOW_REG	0x6C
#define UFCS_PPS2_MAX_VOL_HI_REG	0x6D

#define UFCS_PPS0_MAX_CUR_LOW_REG	0x70
#define UFCS_PPS0_MAX_CUR_HI_REG	0x71
#define UFCS_PPS1_MAX_CUR_LOW_REG	0x72
#define UFCS_PPS1_MAX_CUR_HI_REG	0x73
#define UFCS_PPS2_MAX_CUR_LOW_REG	0x74
#define UFCS_PPS2_MAX_CUR_HI_REG	0x75