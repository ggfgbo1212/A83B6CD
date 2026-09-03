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

#ifndef IC_COMMON_H
#define IC_COMMON_H

#include <stdint.h>
#include <system.h>

#define IC_SUM 8 //此数目对应以下枚举数目的总数

enum sw_type_e
{
 SW_2325  = 0,
 SW_2303,	
 SW_3516P,
 SW_3516H,
 SW_3526,
 SW_3536,	
 SW_3566,
 SW_NET_DEV,
};

//统一电压档位数据，每种芯片应该根据对应的值做相应的修改。
enum voltage_gear_e
{
	 FIX_VOL_5V = 0,
	 FIX_VOL_9V,
	 FIX_VOL_12V,
	 FIX_VOL_15V,
	 FIX_VOL_20V,
	 PPS0_3V3_5V9,	
	 PPS1_3V3_11V,
	 PPS2_3V3_16V,	
	 PPS2_3V3_21V,	
};

///////////QUICK CHARGE PROTOCOL POS
enum quick_charge_protocol_e
{
	QC2_PROTOCOL_IC = 1,
	QC3_PROTOCOL_IC,
	QC3_P_PROTOCOL_IC,
	QC4_P_PROTOCOL_IC,	
	FCP_PROTOCOL_IC,
	H_SCP_PROTOCOL_IC,
	L_SCP_PROTOCOL_IC,
	PD_FIX_PROTOCOL_IC,
	PD_PPS_PROTOCOL_IC,
	PE1_1_PROTOCOL_IC,
	PE2_0_PROTOCOL_IC,
	SFCP_PROTOCOL_IC,
	AFC_PROTOCOL_IC,
};


struct sw_i2c_drive
{
	 u8 (*i2c_read_byte)(u8 devAddr,u8 readAddr, u8 *data, u8 rLen); 
	 u8 (*i2c_write_byte)(u8 devAddr,u8  writeAddr,u8 *dataToWrite, u8 wLen);
};


struct sw_func
{
	u32 (* SW_Init)(u8 port); 

	u32 (* SW_Set_System_Power)(u8 port, u16 power);
	u32 (* SW_User_Define)(u8 port, u8 funcNo, u32 *dat); //用户自定义功能函数
	
	u32 (* SW_Set_VID_PID)(u8 port,u32 setVID,u32 setPID);

	u32 (* SW_Rebroadcast_PDO)(u8 port, u8 way, u32 time);

	
	u32 (* SW_Set_Line_Com)(u8 port, u8 setState,  u32 setValue);
	u32 (* SW_Set_Current_Limit)(u8 port, u8 num,u32 setValue);
	u32 (* SW_Set_Unload_Current)(u8 port,u8 num, u32 setValue);
	u32 (* SW_Set_NTC_Current)(u8 port, u32 setValue);
	u32 (* SW_Set_DAC)(u8 port, u32 setValue);
	
	u32 (* SW_Get_Vin)(u8 port, u32 *vinVol);
	u32 (* SW_Get_Vbus)(u8 port, u32 *vbusVol);
	u32 (* SW_Get_Current)(u8 port, u8 num, u32 *current);
	u32 (* SW_Get_NTC_Temperature)(u8 port, int *temperature);
	u32 (* SW_Get_Port_State)(u8 port, u8 num, u8 *portState);
	u32 (* SW_Get_Sys_Power)(u8 port,u32 *sysPower);
	u32 (* SW_Get_Port_Protocol)(u8 port,  u8 *protocol);
	
//	u32 (* SW_Set_QC)(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
//	u32 (* SW_On_Off_QC)(u8 port,u8 version,u8 setState);	
//	u32 (* SW_On_Off_Emarker)(u8 port, u8 setState);
//	u32 (* SW_Dpdm_Power_Same_PD)(u8 port, u8 setState);
//	u32 (* SW_On_Off_Buck)(u8 port, u8 setState);
//	u32 (* SW_Set_Port_Mode)(u8 port, u8 mode);	
//	u32 (* SW_On_Off_Charge_Protocol)(u8 port,u8 num,u8 setState);
//	u32 (* SW_Set_PE)(u8 port, u32 maxVol);
//	u32 (* SW_On_Off_PE)(u8 port, u8 setState);
//	u32 (* SW_Set_PD_Gear)(u8 port, u8 *pwrAddr, u8 volGear, u8 setState); 
//	u32 (* SW_Set_PD_Vol_Cur)(u8 port,  u8 volGear,  u32 cur);
   
//	u32 (* SW_Set_UFCS_Gear)(u8 port, u8 *pwrAddr, u8 pos, u8 setState);	
//	u32 (* SW_isExist)(u8 port, u8 *existValue); 
//	u32 (* SW_I2C_Unlock)(u8 port);
//	u32 (* SW_Unlock_Force)(u8 port);
//	u32 (* SW_Unlock_High_Reg)(u8 port, u8 setState);//	
	
//	u32 (* SW_Set_FCP_XSCP)(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
//	u32 (* SW_On_Off_FCP_XSCP)(u8 port, u8 fcpVersion, u8 setState);
};

//extern const struct sw_i2c_drive sw_port_i2c_drive[4];
extern const unsigned char icAddr[4];

#endif




