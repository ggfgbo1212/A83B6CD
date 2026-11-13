/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file device_compatibility_policy.h
 * @brief device_compatibility_policy
 * @author SZ team
 ****************************************************************************/
#ifndef _SOURCE_POLICY_H_
#define _SOURCE_POLICY_H_

#include "api.h"

typedef struct
{
	bool isOccBroadcast : 1;
	bool revSoftReset : 1;         //设备主动发软复位(原有策略)
	bool isClosePPSGear : 1;       //已关闭PPS挡位
	bool isPdLinkAbnormal : 1;     //PD链路异常(硬复位/源端软复位)
	bool isAvsClosed : 1;          //已去掉SPR AVS挡位
	bool isSinkModesValid : 1;      //Sink Modes(0x9F)内容有效
	uint16_t reqVolMv;             //最近一次Request电压(APDO有效)
	uint8_t reqObjIndex;           //最近一次Request位置(从1开始)
	uint16_t reqSeq;               //有效Request计数
}Source_Patch_Data_t;
extern Source_Patch_Data_t Source_Patch_Data;


//设备兼容性策略
void Device_Compatibility_Policy(void);

//清除Sink Modes缓存(0x9F)及有效性标志
void Source_Clear_SinkModes(void);


#endif
