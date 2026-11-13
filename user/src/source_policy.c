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
#include <stdio.h>

Source_Patch_Data_t Source_Patch_Data;

//清除Sink Modes缓存及有效性标志
//用于：不支持/Not_Supported/超时/拔出等情况上报
void Source_Clear_SinkModes(void)
{
	reg_new_data[0x99] = 0x00;	//VID low
	reg_new_data[0x9A] = 0x00;	//VID high
	reg_new_data[0x9B] = 0x00;	//PID low
	reg_new_data[0x9C] = 0x00;	//PID high
	reg_new_data[0x9F] = 0x00;	//Sink Modes
	Source_Patch_Data.isSinkModesValid = 0;
}

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


//PPS大跨步断充兼容策略（真我13Pro+ 17.7V反复断充）
//异常报文：手机Request Position6 PPS 17.7V/1A，源端5V->17.7V约需255ms才发PS_RDY，
//  超过手机PPS约250ms的PS_RDY超时；手机不应答后源端发Soft Reset，再发Hard Reset。
//  实测 Request->SoftReset 262~265ms、Request->HardReset 268~271ms，约2.2s循环一次。
//判据：请求的是PPS挡位(位置在固定PDO之后)、电压>=15V，且Request后150~400ms内发生
//  硬复位或源端软复位，即判定为该设备PPS大跨步时序不兼容。
//处理：第一次只去掉SPR AVS挡位(PPS保留)，源端重新广播Source Cap；若同一设备再次出现
//  同样异常，再去掉PPS只保留固定挡位；设备拔出后恢复原始PDO(含SPR AVS)。
#define PPS_REQ_HIGH_VOL_MV		15000U	//高压PPS请求判定门限
#define PPS_TIMEOUT_MIN_MS		150U	//Request->断充时间下限
#define PPS_TIMEOUT_MAX_MS		400U	//Request->断充时间上限
#define PORT_DETACH_HOLD_MS		1000U	//端口持续离线判定时间

extern u8 is_spr_avs_enable;			//iic_slave_policy.c中SPR AVS挡位使能
extern u8 getSinkExtendedStep;		//iic_slave_policy.c中Sink Cap Extended执行状态码

//PPS大跨步+Hard Reset断充兼容策略
//说明：与Solve_PPS_Device_Policy(设备连续软复位策略)相互独立，本函数只处理
//  “高压PPS请求后紧接着Hard Reset/源端软复位”造成的反复断充。
static void Solve_PPS_HardReset_Disconnect_Policy(void)
{
	static uint8_t  isAvsEnableBak = 0;		//去掉SPR AVS前的使能值
	static uint16_t lastReqSeq = 0;		//已处理的Request序号
	static uint8_t  hasReq = 0;		//是否已锁存Request
	static uint32_t reqTime = 0;		//Request时刻
	static uint8_t  reqObjIndex = 0;		//Request位置
	static uint16_t reqVolMv = 0;		//Request电压
	static uint8_t  detachStep = 0;		//离线持续判定
	static uint32_t detachTime = 0;
	uint8_t pdFixNum = 0;
	uint8_t pdSprNum = 0;
	uint8_t pdEprFixNum = 0;
	uint8_t pdEprPdoNum = 0;
	uint8_t isPpsTimeout = 0;
	uint32_t pdo = 0;
	uint32_t dt = 0;

	config_data_t* config = Config_Get();

	//1.锁存最近一次Request(位置/电压/时刻)
	if(Source_Patch_Data.reqSeq != lastReqSeq)
	{
		lastReqSeq = Source_Patch_Data.reqSeq;
		reqTime = My_GetSystemTimeMark();         //收到 Request 的时刻
		reqObjIndex = Source_Patch_Data.reqObjIndex;
		reqVolMv = Source_Patch_Data.reqVolMv;
		hasReq = 1;
	}

	//2.判定PPS大跨步超时断充：高压PPS请求后150~400ms内发生硬复位/源端软复位
	if(Source_Patch_Data.isPdLinkAbnormal)
	{
		Source_Patch_Data.isPdLinkAbnormal = 0;
		dt = My_GetSystemTimePass(reqTime);
		if(hasReq && (reqVolMv >= PPS_REQ_HIGH_VOL_MV) &&
		   (dt >= PPS_TIMEOUT_MIN_MS) && (dt <= PPS_TIMEOUT_MAX_MS))
		{
			//位置从1开始(PD规范)：固定挡位/SPR AVS在类型判断时被排除
			if((reqObjIndex >= 1) && (reqObjIndex <= config->sprPdoNumber) && (reqObjIndex <= 10))
			{
				pdo = config->pdSrcCap[reqObjIndex - 1];
				if(((pdo >> 30) == 3) && (((pdo >> 28) & 0x3) == 0))
				{
					isPpsTimeout = 1;
				}
			}
		}
	}

	//3.处理：先去掉SPR AVS，仍异常再去掉PPS
	if(isPpsTimeout)
	{
		//同一次断充会连续上报软复位和硬复位，处理后清掉请求，避免重复触发
		hasReq = 0;
		reqTime = 0;
		reqObjIndex = 0;
		reqVolMv = 0;

		if(Source_Patch_Data.isAvsClosed == 0)
		{
			//第一次：去掉SPR AVS挡位，PPS保留，下一次Source Cap即不带SPR AVS
			isAvsEnableBak = is_spr_avs_enable;
			is_spr_avs_enable = 0;
			PD_PDO_Update(&pdFixNum, &pdSprNum, &pdEprFixNum, &pdEprPdoNum);
			config->sprFixPdoNumber = pdFixNum;
			config->sprPdoNumber = pdSprNum;
			config->eprFixPdoNumber = pdEprFixNum;
			config->eprPdoNumber = pdEprPdoNum;
			Source_Patch_Data.isAvsClosed = 1;
			// printf("close spravs,sprPdoNum:%d\r\n",config->sprPdoNumber);
		}
	}

	//4.设备拔出后恢复原始PDO(含SPR AVS)
	//  端口需要持续离线一段时间才认为真正拔出，避免Hard Reset期间Vbus掉到0造成误判
	if(Device_Get_Port_State() == 0)
	{
		if(detachStep == 0)
		{
			detachStep = 1;
			detachTime = My_GetSystemTimeMark();
		}
	}
	else
	{
		detachStep = 0;
	}

	if((detachStep == 1) && (My_GetSystemTimePass(detachTime) >= PORT_DETACH_HOLD_MS) &&
	   Source_Patch_Data.isAvsClosed)
	{
		detachStep = 0;
		Source_Patch_Data.isPdLinkAbnormal = 0;
		Source_Patch_Data.isClosePPSGear = 0;
		hasReq = 0;
		lastReqSeq = Source_Patch_Data.reqSeq;
		reqTime = 0;
		reqObjIndex = 0;
		reqVolMv = 0;

		is_spr_avs_enable = isAvsEnableBak;	//恢复SPR AVS使能
		Source_Patch_Data.isAvsClosed = 0;

		//恢复PDO参数
		PD_PDO_Update(&pdFixNum, &pdSprNum, &pdEprFixNum, &pdEprPdoNum);
		config->pdMaxPower = regValue(PD_MAX_POWER_REG);
		config->sprFixPdoNumber = pdFixNum;
		config->sprPdoNumber = pdSprNum;
		config->eprFixPdoNumber = pdEprFixNum;
		config->eprPdoNumber = pdEprPdoNum;
		// printf("restore pdo\r\n");
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

//Sink不支持SPR AVS时的兼容策略
//说明：对端在Sink Capabilities Extended中回报bit5=0(不支持AVS)时，
//  在不掉电的前提下重播一份不含SPR AVS的Source Cap；设备拔出后恢复。
//  去掉AVS后PDO下标整体前移一位，因此只在固定挡位合约下执行。
#define NO_AVS_SETTLE_MS		100U	//收到报文后稳定等待
#define NO_AVS_DETACH_HOLD_MS	1000U	//端口持续离线判定时间(Hard Reset期间Vbus会掉0)

static void Solve_NoAvsSink_Policy(void)
{
	static u8 isSprAvsEnableBak = 0x01;		//去除SPR AVS前的使能值
	static u8 noAvsStep = 0;
	static u32 noAvsTime = 0;
	static u32 detachTime = 0;
	u8 protocol = 0;
	u8 pdFixNum = 0;
	u8 pdSprNum = 0;
	u8 pdEprFixNum = 0;
	u8 pdEprPdoNum = 0;

	config_data_t* config = Config_Get();

	//端口不在线且还没进入"等待拔出"流程: 复位后不进入状态机
	if((Device_Get_Port_State() == 0) && (noAvsStep < 2))
	{
		noAvsStep = 0;
		return;
	}

	switch(noAvsStep)
	{
		case 0:
			//已处理过, 等设备拔出后恢复
			if(Source_Patch_Data.isAvsClosed)
			{
				noAvsStep = 2;
				break;
			}
			//状态码 5: 对端明确回复 Not_Supported
			//此时无 Sink Modes 可读, 直接按不支持 AVS 处理
			if(getSinkExtendedStep != 5)
			{
				if(getSinkExtendedStep != 3)				break;	//还没拿到回复
				if(Source_Patch_Data.isSinkModesValid == 0)	break;	//报文长度不足, Sink Modes不可信
				if((reg_new_data[0x9F] & 0x20) != 0)		break;	//对端支持AVS
			}
			if(is_spr_avs_enable == 0)					break;	//AVS已关闭

			//非PD协议(SCP/VOOC/UFCS等)不处理
			protocol = reg_new_data[0x4B] & 0x7F;
			if((protocol < FC_PD_Fix5V))	break;

			noAvsTime = My_GetSystemTimeMark();
			noAvsStep = 1;
			break;
		case 1:
			if(My_GetSystemTimePass(noAvsTime) < NO_AVS_SETTLE_MS)	break;

			//去掉SPR AVS挡位，重建PDO表
			isSprAvsEnableBak = is_spr_avs_enable;
			is_spr_avs_enable = 0;
			PD_PDO_Update(&pdFixNum, &pdSprNum, &pdEprFixNum, &pdEprPdoNum);
			config->sprFixPdoNumber = pdFixNum;
			config->sprPdoNumber = pdSprNum;
			config->eprFixPdoNumber = pdEprFixNum;
			config->eprPdoNumber = pdEprPdoNum;
			Config_Apply();

			//重播新的Source Cap，Sink重新Request，挡位不变则PS_RDY，Vbus不掉
			Source_Patch_Data.isAvsClosed = 1;
			Source_Patch_Data.isOccBroadcast = true;	//重播期间关闭OVP保护5s
			Pd_Rebroadcast_Src_Cap();
			// printf("no avs sink: close spravs, sprPdoNum:%d\r\n", config->sprPdoNumber);
			noAvsStep = 2;
			break;
		case 2:
			//已处理, 等待设备拔出
			if(Device_Get_Port_State() == 0)
			{
				detachTime = My_GetSystemTimeMark();
				noAvsStep = 3;
			}
			break;
		case 3:
			//拔出判定: 1s内回来说明是瞬断(Hard Reset会掉Vbus), 继续等; 否则确认拔出
			if(Device_Get_Port_State() != 0)
			{
				noAvsStep = 2;
				break;
			}
			if(My_GetSystemTimePass(detachTime) < NO_AVS_DETACH_HOLD_MS)	break;

			//设备拔出, 本设备信息作废, 恢复原始PDO(含SPR AVS)
			Source_Clear_SinkModes();
			if(Source_Patch_Data.isAvsClosed)
			{
				is_spr_avs_enable = isSprAvsEnableBak;
				PD_PDO_Update(&pdFixNum, &pdSprNum, &pdEprFixNum, &pdEprPdoNum);
				config->sprFixPdoNumber = pdFixNum;
				config->sprPdoNumber = pdSprNum;
				config->eprFixPdoNumber = pdEprFixNum;
				config->eprPdoNumber = pdEprPdoNum;
				Source_Patch_Data.isAvsClosed = 0;
			}
			noAvsStep = 0;
			break;
		default:
			noAvsStep = 0;
			break;
	}
}

//设备兼容性策略
void Device_Compatibility_Policy(void)
{
	Samsung_Galaxy_S23_Policy();
	Solve_PPS_Device_Policy();
	// Solve_PPS_HardReset_Disconnect_Policy();
	Solve_NoAvsSink_Policy();
}
 
 
 
