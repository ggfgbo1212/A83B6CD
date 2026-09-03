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

#ifndef IC_LIB_RELY_H
#define IC_LIB_RELY_H

#include "ic_common.h"
#include "user_define.h"

extern const struct sw_func *SWXXXX_Func[IC_SUM];

#define SW_Func1 SWXXXX_Func[PORT1_IC]
#define SW_Func2 SWXXXX_Func[PORT2_IC]
#define SW_Func3 SWXXXX_Func[PORT3_IC]
#define SW_Func4 SWXXXX_Func[PORT4_IC]
#define SW_Func5 SWXXXX_Func[PORT5_IC]

extern const u8 portNumBuf[5];

#pragma pack(1)

typedef struct 
{
	u8  mode; //0:直接获取，1:根据电流获取。
	u32 unloadCurrent ; //空载电流设置。
	u32 unLoadDebounce; //空载滤波时间
	
	u32 loadCurrent;	//带载电流设置。
	u32 loadDebounce;	//带载滤波时间。

	u32 gatherTime; //采样开始时间。
}get_port_state_mode;

#pragma pack()

u32 SW_Get_Actual_Temp(u8 port, int * temperture, float ntcBvalue, float ntcR25);
u32 SW_Get_Specify_State(u8 port, u8 num, get_port_state_mode *get_state_mode, u8 *state);

/******根据当前的温度值和温度功率档，返回是否需要更新PDO，温度定义在user_define.h中*****/
u32 NTC_To_Gear_Handle(int tmperture, u8 * curGear);
#endif




