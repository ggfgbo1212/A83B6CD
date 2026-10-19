/****************************************************************************
 * @copyright Copyright(C) 2024-2024 Ismartware Limited. All rights reserved.
 * @file device_compatibility_policy.c
 * @brief device_compatibility_policy
 * @author SZ team
 ****************************************************************************/

#include "source_policy.h"
#include "iic_slave_policy.h"
#include "iic_slave_reg.h"
#include "slave_policy_drive.h"

Source_Patch_Data_t Source_Patch_Data;

#define PC_POLICY				//笔记本策略
#define PHONE_POLICY			//手机策略
#define OTHER_POLICY			//其他策略


#ifdef PHONE_POLICY
//软件解决多档PPS设备导致的兼容性问题
//检测到接收到软件复位
//连续三次就发生一次软件复位
static void Solve_PPS_Device_Policy(void)
{
	static uint8_t curStep = 0;
	static uint8_t curRevSoftResetNum = 0;
	static uint32_t curRevTime = 0;
	uint8_t curProtocol = 0;
	uint8_t curPPSGear = 0;
	uint8_t pdFixNum = 0;
	uint8_t pdSprNum = 0;
	uint8_t pdMaxPower = 0;
	uint8_t pdEprPdoNum = 0;
	uint8_t pdEprFixNum = 0;
	
	config_data_t* config = Config_Get();
	if(Source_Patch_Data.revSoftReset && (curStep < 2))
	{
//		printf("revSoftReset:%d",revSoftReset);
		Source_Patch_Data.revSoftReset = 0;
		switch(curStep)
		{
			case 0x00:
				curStep = 0x01;
				curRevTime = My_GetSystemTimeMark();
				break;
			case 0x01:
				if(My_GetSystemTimePass(curRevTime) < 1000)
				{
					curRevSoftResetNum ++;
					if(curRevSoftResetNum > 3)
					{
						curStep = 2;
					}
				}
				else
				{
					curStep = 3;
				}
				break;
			default:
				break;
		}		
	}
	
	//发送软件复位
	if(curStep == 2)
	{
		curStep = 3;
		//清除PPS挡位
		Source_Patch_Data.isClosePPSGear = 1;
		//清除PPS挡位
		config->sprPdoNumber = config->sprFixPdoNumber;     //spr pdo num
//		printf("sprSrcPdo:%d",config->sprSrcPdoNumber);
		//发送软件复位
		Pd_Send_Soft_Reset();
//		printf("curStep:%d",curStep);
	}
	else if(curStep == 3)
	{
		//判断设备离线
		if(Device_Get_Port_State() == 0)
		{
			curStep = 0;
			Source_Patch_Data.revSoftReset = 0;
			Source_Patch_Data.isClosePPSGear = 0;
			curRevSoftResetNum = 0;
			
			//恢复PDO参数
			PD_PDO_Update(&pdFixNum, &pdSprNum, &pdEprFixNum, &pdEprPdoNum);
			config->pdMaxPower = regValue(PD_MAX_POWER_REG);      //140w power
			config->sprFixPdoNumber = pdFixNum;  //spr fix pdo num
			config->sprPdoNumber = pdSprNum;     //spr pdo num
			config->eprFixPdoNumber = pdEprFixNum;  //epr fix pdo num
			config->eprPdoNumber = pdEprPdoNum;	  //epr pdo num,it is eprFixPdoNumber add avs pdo num	
			
//			printf("isClosePPSGear:%d",isClosePPSGear);
		}
	}
}


/*	Samsung_Galaxy_S23
* 设备兼容性问题说明
* 原因：设备在直充类协议下重新广播PD，会触发芯片Vout_OVP保护
* 方案：在重新广播PD时，关闭OVP保护5s
* 结果：
*/
void Samsung_Galaxy_S23_Policy(void)
{
	static u8 policyStep = 0;
	static u32 tempTime = 0;
	
	if(Source_Patch_Data.isOccBroadcast)
	{
		Source_Patch_Data.isOccBroadcast = false;
		policyStep = 1;
	}
		
	switch(policyStep)
	{
		case 0:
			//不执行操作
			break;
		case 1:
			Vout_Ovp_Ctrl(false);
			tempTime = My_GetSystemTimeMark();
			policyStep = 2;
			break;
		case 2:
			if(My_GetSystemTimePass(tempTime) > 5000)
			{
				Vout_Ovp_Ctrl(true);
				policyStep = 0;
			}
			break;
		default:
			break;
	}
}
#endif

//设备兼容性策略
void Device_Compatibility_Policy(void)
{
	Samsung_Galaxy_S23_Policy();
	Solve_PPS_Device_Policy();
}
 
 
 