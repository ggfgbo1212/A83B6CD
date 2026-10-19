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
	bool revSoftReset : 1;
	bool isClosePPSGear : 1;
}Source_Patch_Data_t;
extern Source_Patch_Data_t Source_Patch_Data;


//设备兼容性策略
void Device_Compatibility_Policy(void);


#endif