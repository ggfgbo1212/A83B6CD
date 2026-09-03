/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file main.c
 * @brief Main program body
 * @author SD1 & SW team
 ****************************************************************************/
#include <stdio.h>
#include "api.h"
#include "config.h"
#include "ufcs_caps.h"
#include "pdo.h"
#include "serial.h"
#include "zr_rcc.h"
#include "pd_message.h"
#include "user_pd_policy.h"
#include "sw3566_i2c_lib.h"
#include "device_drive.h" 
#include "zr_nvic.h"
#include "device_com.h" 

#define CUSTOMIZE_CFG

extern device_policy_t devicePolicy;
extern u32 SystemTimeCounter;

static void lowPowerOnBefore(void);
static void lowPowerOnAfter(void);


static const ufcs_output_cap_t ufcs100wOutputCap[] = {
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(340),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(340),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1100),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1100),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(550),         // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(550),         // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mV
    },
    {
        .fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
        .fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2100),        // unit : 10mV
        .fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2100),        // unit : 10mV
        .fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1100),        // unit : 10mV
        .fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1100),        // unit : 10mV
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(500),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(500),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },
};

//调流调压钩子函数
//static bool PowerAdjust(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr)
//{
//    if (Device_Get_Protocol(1) == FC_PD_PPS)
//    {
//        *port1Curr = 160;
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

//线补钩子函数
//static bool Compensate_Callback(uint16_t volInMv, bool* isEnabled, cable_compensave_value_e* value)
//{
//    if (volInMv == 5000)
//    {
//        *isEnabled = false;
//        *value = CABLE_COMPENSAVE_VALUE_65MOHM;
//        return true;
//    }
//    else if (volInMv == 9000)
//    {
//        *isEnabled = true;
//        *value = CABLE_COMPENSAVE_VALUE_100MOHM;
//        return true;
//    }
//    else
//    {
//        *isEnabled = true;
//        *value = CABLE_COMPENSAVE_VALUE_65MOHM;
//        return true;
//    }
//}



int main(void)
{
    /// device init shall be called firstly
    Device_Init();
    Nvic_Irq_Enable_All();
	
#ifdef CUSTOMIZE_CFG
    /// get system config, it shall be after Device_Init()
    config_data_t* config = Config_Get();
		//设置为单C模式
    config -> portMode = SINGLE_C_MODE;  
		//UFCS协议设置
	  config->isUfcsEnabled = true;
    config->ufcsSrcCap = (const uint8_t*)ufcs100wOutputCap;
    config->ufcsSrcCapNum = 3;
		//QC2.0协议设置
		config->isQc2p0Enabled = true;
		config->qc2p0MaxVoltage = QC2P0_MAX_VOL_20V;
		//QC3.0协议设置
		config->isQc3p0Enabled = true;
		config->qc3p0MaxVoltage = QC3P0_MAX_VOL_20V;
		config->qc3MaxPower = QC3_MAX_POWER_60W;
		//QC3.0+协议设置
		config->isQc3plusEnabled = true;
		//FCP协议设置
		config->isFcpEnabled = true;
		//SCP协议设置
    config->isHvScpEnabled = true;
    config->isLvScpEnabled = true;		
		config->scpMaxVoltage = 200;
    config->scpMaxCurrent = 40;
		//AFC协议设置
		config->isAfcEnabled = true;
		//SFCP协议设置
		config->isSfcpEnabled = true;
		//PE协议设置
		config->isPeEnabled = true;
		//三星模式
		config->isSamsungEnabled = true;
		//苹果模式
    config->isAppleEnabled = true;
		//DPDM最大功率
		//config->dpdmFixPower = 60;
		//线补
		config -> cableCompensationValue = CABLE_COMPENSAVE_VALUE_100MOHM;
		//电压偏置
    config -> fixVoltageOffset = FIX_VOLTAGE_OFFSET_150MV; 
		//电流偏置
		config -> fixCurrentOffset = FIX_CURRENT_OFFSET_450MA;	
		//默认不能删除
    config->isTypecCertEnabled = false;
    config->typecCcTerm = RP_3A;
    config->isTypecVconnEnabled = true;
    config->isVconnLowPowerModeEnabled = false;
    config->typecConnectTime = 130;
		//配置初始化
    Config_Apply();
		//PD二次开发
		User_Pd_Policy_Init();	
#endif
		//定时器先初始化
		ATM_Init_Normal();
		//i2c超时函数注册，必须在i2c设备初始化，否则程序将跑飞
		i2c_Register_Time(My_GetSystemTimePass, My_GetSystemTimeMark); 
		//主从机区分及初始化
		MasterSlaveIdentify(&devicePolicy);
		//线补钩子函数调用
		//Device_Register_Cable_Compensate_Hook(Compensate_Callback);
		//调流调压钩子函数调用
		//Device_Register_Power_Adjust_Hook(PowerAdjust);		
		//睡眠前后钩子函数，必须使用
		Low_Power_Register_Hook(lowPowerOnBefore, lowPowerOnAfter);
		//上电延时
		My_Time_Delay(200);
	
    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();
        // device policy process, secondary development
				//主设备挂的有I2C设备，所以需要单独控制 
				if(devicePolicy.devRole == MASTER_DEV)
				{
					//状态获取，在改变时需要更新PDO。
					Device_Policy(&devicePolicy);
					//实际的设备操作函数，单指令操作模式。
					IC_Device_Read(&devicePolicy);
					//GPIO相关操作
					//Gpio_Policy(&devicePolicy);
				}		
				else
				{
					//通信策略，只从用
					Dev_Com_Policy(&devicePolicy);
					//更新从机状态
					Slave_Update_Chart(&devicePolicy);
				}			
				//功率策略
				Power_Policy(&devicePolicy);
				//睡眠策略
				Sleep_Policy(&devicePolicy);	
        // sleep process
        Low_Power_Sleep();
    }
}

static void lowPowerOnBefore(void)
{
     
}

static void lowPowerOnAfter(void)
{
   SystemTimeCounter += 200;
}

