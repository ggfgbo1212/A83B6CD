/*
***************************************************************************
* Copyright (C) 2022-2023 Ismartware Limited. All rights reserved.
* File name: iic_slave_policy.c
* Description: functions about ic device to drive the hard device .
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date:
* History:
***************************************************************************
*/
#include "usr_common.h"
#include "source_policy.h"

#include "zr_abn.h"
#include "iic_slave_policy.h"
#include "iic_slave_reg.h"
#include "slave_policy_drive.h"
#include <stdio.h>

// 常用变量定义
#define COMMAND_VARIABLE
#ifdef COMMAND_VARIABLE

// 最新寄存器数据
u8 reg_new_data[0x100];
////上次寄存器数据

// 0x8F
u8 reg_0x8F_data[8];
u8 reg_0x90_data[8];

// 0x99-0x9E Sink Cap Extended 功能相关变量
u8 sendComandTime = 0x01;      // 0x9D: 发送前延时, 单位 10ms
u8 getSinkExtendedStep = 0; // 0x9E bit7-4: 执行状态码

u8 sysBuckClk = 11;
u8 sysSleep = 0;

// 寄存器轮询
u8 reg_polling = 0x00;

u32 pd140wCapIIC[10]; // PDO 可以进行修改

port_mode_e cur_port_mode = SINGLE_C_MODE; // 当前端口状态

u8 set_reg_addr = 0x3c; // 设置寄存器地址

#define PD_DEFAULT_MAX_POWER 100U

// pps0是否使能(0/1)
u8 is_pps0_enable = 0x01;
// pps1是否使能
u8 is_pps1_enable = 0x01;
// pps2是否使能
u8 is_pps2_enable = 0x01;
// spravs是否使能
u8 is_spr_avs_enable = 0x01;
// avs0是否使能
u8 is_avs0_enable = 0x01;
// 9v固定挡位是否使能
u8 is_9v_fixed_gear_enable = 0x01;
// 12v固定挡位是否使能
u8 is_12v_fixed_gear_enable = 0x00;
// 15v固定挡位是否使能
u8 is_15v_fixed_gear_enable = 0x01;
// 20v固定挡位是否使能
u8 is_20v_fixed_gear_enable = 0x01;
// 28v固定挡位是否使能
u8 is_28v_fixed_gear_enable = 0x01;

//---------5v--------
// 5V挡位电流
u16 fix5v_cur = 3000;
// 5v标志
u32 fix5v_flags = PDO_FIXED_EPR_MODE | PDO_FIXED_UNCONSTRAINED_POWER | PDO_FIXED_DUAL_DATA_ROLE;
// 5v峰值电流
u32 fix5v_peak_cur = PDO_FIXED_NO_PEAK_CURRENT;

//---------9v---------
// 9v挡位电压
u16 fix9v_vol = 9000;
// 9v挡位电流
u16 fix9v_cur = 3000;
// 9v峰值电流
u32 fix9v_peak_cur = PDO_FIXED_NO_PEAK_CURRENT;

//--------12v---------
// 12v挡位电压
u16 fix12v_vol = 12000;
// 12v挡位电流
u16 fix12v_cur = 3000;
// 12v峰值电流
u32 fix12v_peak_cur = PDO_FIXED_NO_PEAK_CURRENT;

//--------15v---------
// 15v挡位电压
u16 fix15v_vol = 15000;
// 15v挡位电流
u16 fix15v_cur = 3000;
// 15v峰值电流
u32 fix15v_peak_cur = PDO_FIXED_NO_PEAK_CURRENT;

//--------20v---------
// 15v挡位电压
u16 fix20v_vol = 20000;
// 20v挡位电流
u16 fix20v_cur = 5000;
// 20v峰值电流
u32 fix20v_peak_cur = PDO_FIXED_NO_PEAK_CURRENT;

//--------28v---------
// 28v挡位电压
u16 fix28v_vol = 28000;
// 28v挡位电流
u16 fix28v_cur = 5000;
// 28v峰值电流
u32 fix28v_peak_cur = PDO_FIXED_NO_PEAK_CURRENT;

u8 spr_avs_9v15v_max_cur = 200; // 9v-15v最大电流 unit: 20mA/bit
u8 spr_avs_15v20v_max_cur = 150; // 15v-28v最大电流 unit: 20mA/bit


//--------pps0------
// pps0最大电流
u16 pps0_max_cur = 5000;
// pps0最小电压
u16 pps0_min_vol = 5000;
// pps0最大电压
u16 pps0_max_vol = 11000;
// pps0标志
u32 pps0_flags = 0;

//--------pps1------
// pps0最大电流
u16 pps1_max_cur = 5000;
// pps0最小电压
u16 pps1_min_vol = 5000;
// pps0最大电压
u16 pps1_max_vol = 16000;
// pps0标志
u32 pps1_flags = 0;

//--------pps2------
// pps0最大电流
u16 pps2_max_cur = 5000;
// pps0最小电压
u16 pps2_min_vol = 5000;
// pps0最大电压
u16 pps2_max_vol = 21000;
// pps0标志
u32 pps2_flags = 0;

//--------avs0------
// avs0最小电压
u16 avs0_min_vol = 15000;
// avs0最大电压
u16 avs0_max_vol = 28000;
// avs0PD功率
u8 avs0_power = 140;

// 当SPR pdo打开时，它应设置为零
u8 cap_spr = 0;

// Vout平均电压
u16 vout_avg_vol = 0x00;

// Vout原始电压
u16 vout_real_vol = 0x00;

// Vin电压
u16 vin_vol = 0x00;

// port1 Iout平均电流
u16 port1_iout_avg_cur = 0x00;

// port1 Iout原始电流
u16 port1_iout_real_cur = 0x00;

// ADC温度
u16 adc_temperature = 0x00;
// Adc_Die_Get

// port2 Iout平均电流
u16 port2_iout_avg_cur = 0x00;

// port2 Iout原始电流
u16 port2_iout_real_cur = 0x00;

// DAC电压
u16 dac_vol = 0x00;

// Device_Get_Requesting_Voltage
// 获取当前设备请求的电压
u16 cur_device_req_vol = 0x00;

// PD请求的电压
u16 pd_request_vol = 0x00;

// PD请求的电流
u16 pd_request_cur = 0x00;

// 用于判断是否电流电压是否发生变化
u16 oldVol = 0x00;
u16 oldPort1Cur = 0x00;
u16 oldPort2Cur = 0x00;

// IRQ功能是否开启
u8 isIRQEnable = 0;

// 调压、调流中断是否使能
u8 adjustVolCur = 0;

// 调压、调流中断是否发生
u8 adjustVolCurIfExist = 0;

// 端口是否在线
u8 oldPortIsExist = 0;

// Port1空载电流
u8 port1NoLoadCur = 15;
// Port2空载电流
u8 port2NoLoadCur = 15;

// 场景转换时间 min 默认20分钟
u8 sceneConversionTime = 20;

// pps增加偏置限流
u8 ppsOffsetGearPort1 = 0;
// avs增加偏置限流
u8 avsOffsetGearPort1 = 0;

// pps增加偏置限流
u8 ppsOffsetGearPort2 = 0;
// avs增加偏置限流
u8 avsOffsetGearPort2 = 0;

ufcs_output_cap_t ufcsPdoOutputCap[] = {

	{
		.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID1, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
		.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(1100), // unit : 10mV
		.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(1100), // unit : 10mV
		.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(550),  // unit : 10mV
		.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(550),  // unit : 10mV
		.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300), // unit : 10mA
		.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300), // unit : 10mA
		.fields.minCurrent = 100,						  // unit : 10mV
	},

	{
		.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID2, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
		.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2100), // unit : 10mV
		.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2100), // unit : 10mV
		.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1100), // unit : 10mV
		.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1100), // unit : 10mV
		.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300), // unit : 10mA
		.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300), // unit : 10mA
		.fields.minCurrent = 100,						  // unit : 10mV
	},

	{
		.fields.head = UFCS_CAP_SET_HEAD(UFCS_CAP_ID3, UFCS_CURR_STEP_50MA, UFCS_VOL_MIN_STEP_10MV),
		.fields.hiMaxVolage = UFCS_CAP_SET_HI_BYTE(2100), // unit : 10mV
		.fields.loMaxVolage = UFCS_CAP_SET_LO_BYTE(2100), // unit : 10mV
		.fields.hiMinVolage = UFCS_CAP_SET_HI_BYTE(1100), // unit : 10mV
		.fields.loMinVolage = UFCS_CAP_SET_LO_BYTE(1100), // unit : 10mV
		.fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(300), // unit : 10mA
		.fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(300), // unit : 10mA
		.fields.minCurrent = 100,						  // unit : 10mV
	},
};

#endif

static void abnOccurredHook(u32 pending);
static void abnReleaseHook(u32 pending);

// PD PDO更新
void PD_PDO_Update(u8 *fix_sum, u8 *spr_sum, u8 *epr_fix, u8 *epr_sum)
{
	// 数组下标
	u8 array_sub = 0x00;

	pd140wCapIIC[array_sub] = PDO_FIXED_VSAFE5V(fix5v_cur, fix5v_flags, fix5v_peak_cur);
	array_sub++;

	if (is_9v_fixed_gear_enable)
	{
		pd140wCapIIC[array_sub] = PDO_FIXED_OTHERS(fix9v_vol, fix9v_cur, fix9v_peak_cur);
		array_sub++;
	}

	if (is_12v_fixed_gear_enable)
	{
		pd140wCapIIC[array_sub] = PDO_FIXED_OTHERS(fix12v_vol, fix12v_cur, fix12v_peak_cur);
		array_sub++;
	}

	if (is_15v_fixed_gear_enable)
	{
		pd140wCapIIC[array_sub] = PDO_FIXED_OTHERS(fix15v_vol, fix15v_cur, fix15v_peak_cur);
		array_sub++;
	}

	if (is_20v_fixed_gear_enable)
	{
		pd140wCapIIC[array_sub] = PDO_FIXED_OTHERS(fix20v_vol, fix20v_cur, fix20v_peak_cur);
		array_sub++;
	}

	*fix_sum = array_sub;
	if(is_spr_avs_enable)
	{
		pd140wCapIIC[array_sub] = PDO_SPR_AVS(spr_avs_15v20v_max_cur * 20, spr_avs_9v15v_max_cur * 20);
		array_sub++;
	}
	if (is_pps0_enable)
	{
		pd140wCapIIC[array_sub] = PDO_PPS(pps0_max_cur, pps0_min_vol, pps0_max_vol, pps0_flags);
		array_sub++;
	}

	if (is_pps1_enable && array_sub <= 6)
	{
		pd140wCapIIC[array_sub] = PDO_PPS(pps1_max_cur, pps1_min_vol, pps1_max_vol, pps1_flags);
		array_sub++;
	}

	if (is_pps2_enable && array_sub <= 6)
	{
		pd140wCapIIC[array_sub] = PDO_PPS(pps2_max_cur, pps2_min_vol, pps2_max_vol, pps2_flags);
		array_sub++;
	}

	if (array_sub > 7)
	{
		array_sub = 7;
	}

	*spr_sum = array_sub;

	// 添加占位符
	while ((7 - array_sub) > 0)
	{
		pd140wCapIIC[array_sub] = cap_spr;
		array_sub++;
	}

#ifndef SW3561X
	// 更新掉28V相关

	*epr_fix = 0;
	if (is_28v_fixed_gear_enable)
	{
		pd140wCapIIC[array_sub] = PDO_FIXED_OTHERS(fix28v_vol, fix28v_cur, fix28v_peak_cur);
		array_sub++;

		*epr_fix = 1;
		*epr_sum = 1;
	}

	if (is_avs0_enable)
	{
		pd140wCapIIC[array_sub] = PDO_AVS(avs0_min_vol, avs0_max_vol, avs0_power);
		array_sub++;

		*epr_sum = 2;
	}
#endif
}

u8 UFCS_PDO_Update(void)
{
	u8 array_sub = 0x00;
	u8 capID[3] = {UFCS_CAP_ID1, UFCS_CAP_ID2, UFCS_CAP_ID3};

	u8 ufcs_pps_cur_step = 0;
	u8 ufcs_pps_vol_step = 0;

	if (regValue(UFCS_PPS_ENABLE_REG) & 0x01)
	{
		ufcsPdoOutputCap[array_sub].fields.hiMaxVolage = regValue(UFCS_PPS0_MAX_VOL_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMaxVolage = regValue(UFCS_PPS0_MAX_VOL_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.hiMinVolage = regValue(UFCS_PPS0_MIN_VOL_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMinVolage = regValue(UFCS_PPS0_MIN_VOL_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.hiMaxCurrent = regValue(UFCS_PPS0_MAX_CUR_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMaxCurrent = regValue(UFCS_PPS0_MAX_CUR_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.minCurrent = regValue(UFCS_PPS0_MIN_CUR_REG);

		ufcs_pps_cur_step = ((regValue(UFCS_PPS0_STEP_REG) >> 1) & 0x07);
		ufcs_pps_vol_step = (regValue(UFCS_PPS0_STEP_REG) & 0x01);
		ufcsPdoOutputCap[array_sub].fields.head = UFCS_CAP_SET_HEAD(capID[array_sub], ufcs_pps_cur_step, ufcs_pps_vol_step);

		array_sub += 1;
	}

	if (regValue(UFCS_PPS_ENABLE_REG) & 0x02)
	{
		ufcsPdoOutputCap[array_sub].fields.hiMaxVolage = regValue(UFCS_PPS1_MAX_VOL_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMaxVolage = regValue(UFCS_PPS1_MAX_VOL_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.hiMinVolage = regValue(UFCS_PPS1_MIN_VOL_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMinVolage = regValue(UFCS_PPS1_MIN_VOL_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.hiMaxCurrent = regValue(UFCS_PPS1_MAX_CUR_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMaxCurrent = regValue(UFCS_PPS1_MAX_CUR_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.minCurrent = regValue(UFCS_PPS1_MIN_CUR_REG);

		ufcs_pps_cur_step = ((regValue(UFCS_PPS1_STEP_REG) >> 1) & 0x07);
		ufcs_pps_vol_step = (regValue(UFCS_PPS1_STEP_REG) & 0x01);
		ufcsPdoOutputCap[array_sub].fields.head = UFCS_CAP_SET_HEAD(capID[array_sub], ufcs_pps_cur_step, ufcs_pps_vol_step);

		array_sub += 1;
	}

	if (regValue(UFCS_PPS_ENABLE_REG) & 0x04)
	{
		ufcsPdoOutputCap[array_sub].fields.hiMaxVolage = regValue(UFCS_PPS2_MAX_VOL_HI_REG);
		;
		ufcsPdoOutputCap[array_sub].fields.loMaxVolage = regValue(UFCS_PPS2_MAX_VOL_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.hiMinVolage = regValue(UFCS_PPS2_MIN_VOL_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMinVolage = regValue(UFCS_PPS2_MIN_VOL_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.hiMaxCurrent = regValue(UFCS_PPS2_MAX_CUR_HI_REG);
		ufcsPdoOutputCap[array_sub].fields.loMaxCurrent = regValue(UFCS_PPS2_MAX_CUR_LOW_REG);

		ufcsPdoOutputCap[array_sub].fields.minCurrent = regValue(UFCS_PPS2_MIN_CUR_REG);

		ufcs_pps_cur_step = ((regValue(UFCS_PPS2_STEP_REG) >> 1) & 0x07);
		ufcs_pps_vol_step = (regValue(UFCS_PPS2_STEP_REG) & 0x01);
		ufcsPdoOutputCap[array_sub].fields.head = UFCS_CAP_SET_HEAD(capID[array_sub], ufcs_pps_cur_step, ufcs_pps_vol_step);

		array_sub += 1;
	}

	return array_sub;
}

// 寄存器初始化
void Reg_Init(void)
{
	config_data_t *config = Config_Get();
	abn_config_t abnCfg = {0};
	Abn_Get_Config(&abnCfg);

	reg_new_data[0x00] = 0x06;
	reg_new_data[0x01] = cur_port_mode << 5;
	reg_new_data[0x02] = PD_DEFAULT_MAX_POWER; // 默认功率100W
	reg_new_data[0x03] = ((is_pps0_enable << 7) | (is_pps1_enable << 6) | (is_pps2_enable << 5) | (is_spr_avs_enable << 4));	//bit4: SPR AVS使能
	reg_new_data[0x04] = (is_avs0_enable << 0);													// 0x12;
	reg_new_data[0x05] = ((fix5v_cur / 10) & 0x0300) >> 8;										// 0x0B;
	reg_new_data[0x06] = (fix5v_cur / 10) & 0x00FF;												// 0xB8;
	reg_new_data[0x07] = (is_9v_fixed_gear_enable << 2) | (((fix9v_vol / 50) & 0x0300) >> 8);	// 0x23;
	reg_new_data[0x08] = (fix9v_vol / 50) & 0x00FF;												// 0x28;
	reg_new_data[0x09] = ((fix9v_cur / 10) & 0x0300) >> 8;										// 0x0B;
	reg_new_data[0x0A] = (fix9v_cur / 10) & 0x00FF;												// 0xB8;
	reg_new_data[0x0B] = (is_12v_fixed_gear_enable << 2) | (((fix12v_vol / 50) & 0x0300) >> 8); // 0x2E;
	reg_new_data[0x0C] = (fix12v_vol / 50) & 0x00FF;											// 0xE0;
	reg_new_data[0x0D] = ((fix12v_cur / 10) & 0x0300) >> 8;										// 0x0B;
	reg_new_data[0x0E] = (fix12v_cur / 10) & 0x00FF;											// 0xB8;
	reg_new_data[0x0F] = (is_15v_fixed_gear_enable << 2) | (((fix15v_vol / 50) & 0x0300) >> 8); // 0x3A;

	reg_new_data[0x10] = (fix15v_vol / 50) & 0x00FF;											// 0x98;
	reg_new_data[0x11] = ((fix15v_cur / 10) & 0x0300) >> 8;										// 0x0B;
	reg_new_data[0x12] = (fix15v_cur / 10) & 0x00FF;											// 0xB8;
	reg_new_data[0x13] = (is_20v_fixed_gear_enable << 2) | (((fix20v_vol / 50) & 0x0300) >> 8); // 0x4E;
	reg_new_data[0x14] = (fix20v_vol / 50) & 0x00FF;											// 0x20;
	reg_new_data[0x15] = ((fix20v_cur / 10) & 0x0300) >> 8;										// 0x13;
	reg_new_data[0x16] = (fix20v_cur / 10) & 0x00FF;											// 0x88;
	reg_new_data[0x17] = 0x00;
	reg_new_data[0x18] = (pps0_min_vol / 100) & 0x00FF; // 0xE4;
	reg_new_data[0x19] = 0x00;
	reg_new_data[0x1A] = (pps0_max_vol / 100) & 0x00FF; // 0x08;
	reg_new_data[0x1B] = 0x00;
	reg_new_data[0x1C] = (pps0_max_cur / 50) & 0x00FF; // 0x88;

	// 更新掉28V相关
	reg_new_data[0x1D] = (is_28v_fixed_gear_enable << 2) | (((fix28v_vol / 50) & 0x0300) >> 8); // 0x6D;
	reg_new_data[0x1E] = (fix28v_vol / 50) & 0x00FF;											// 0x60;
	reg_new_data[0x1F] = ((fix28v_cur / 10) & 0x0300) >> 8;										// 0x13;

	reg_new_data[0x20] = (fix28v_cur / 10) & 0x00FF; // 0x88;
	reg_new_data[0x21] = 0x00;
	reg_new_data[0x22] = (avs0_min_vol / 100) & 0x00FF;		   // 0x98;
	reg_new_data[0x23] = ((avs0_max_vol / 100) & 0x0100) >> 8; // 0x6D;
	reg_new_data[0x24] = (avs0_max_vol / 100) & 0x00FF;		   // 0x60;
	reg_new_data[0x25] = avs0_power;						   // 0x8C;

	reg_new_data[0x26] = 0x00;
	reg_new_data[0x27] = 0x00;
	reg_new_data[0x28] = 0x00;
	reg_new_data[0x29] = 0x00;
	reg_new_data[0x2A] = 0x00;
	reg_new_data[0x2B] = 0x00;
	reg_new_data[0x2C] = 0x00;
	reg_new_data[0x2D] = 0x00;
	reg_new_data[0x2E] = 0x00;
	reg_new_data[0x2F] = 0x00;

	reg_new_data[0x30] = 0x00;
	reg_new_data[0x31] = 0x00;
	reg_new_data[0x32] = 0x00;
	reg_new_data[0x33] = 0x00;
	reg_new_data[0x34] = 0x18;													  //
	reg_new_data[0x35] = config->tfcpMaxPower;									  // 0x00;
	reg_new_data[0x36] = config->voocMaxPower;									  // 0x00;
	reg_new_data[0x37] = config->superVoocMaxPower;								  // 0x00;
	reg_new_data[0x38] = (config->scpMaxCurrent & 0xFF00) >> 8;					  // 0x00;
	reg_new_data[0x39] = config->scpMaxCurrent & 0x00FF;						  // 0x00;
	reg_new_data[0x3A] = config->scpMaxPower;									  // 0x00;
	reg_new_data[0x3B] = (config->scpMaxVoltage & 0xFF00) >> 8;					  // 0x00;
	reg_new_data[0x3C] = config->scpMaxVoltage & 0x00FF;						  // 0x00;
	reg_new_data[0x3D] = config->scpAdapterType;								  // 0x00;
	reg_new_data[0x3E] = config->dpdmFixPower;									  // 0x00;
	reg_new_data[0x3F] = (config->typecCcTerm << 4) | (config->fixVoltageOffset); // 0x00;

	reg_new_data[0x40] = (config->fixCurrentOffset << 4) | (config->qc2p0MaxVoltage);																																																								  // 0x00;
	reg_new_data[0x41] = (config->qc3p0MaxVoltage << 4) | (config->peMaxVoltage);																																																									  // 0x00;
	reg_new_data[0x42] = (config->qc3MaxPower << 4) | (config->p1CurrentLimit);																																																										  // 0x00;
	reg_new_data[0x43] = (config->p2CurrentLimit << 4) | (config->cableCompensationValue);																																																							  // 0x00;
	reg_new_data[0x44] = (config->afcMaxVoltage << 4) | (config->sfcpMaxVoltage);																																																									  // 0x00;
	reg_new_data[0x45] = (config->fcpMaxVoltage << 4);																																																																  // 0x00;
	reg_new_data[0x46] = (config->isCableCompensaved << 7) | (config->isTypecVconnEnabled << 6) | (config->isVconnSwapEnabled << 5) | (config->isResponseDiscoveryId << 4) | (config->isTfcpEnabled << 3) | (config->isVooc1p0Enabled << 2) | (config->isVooc4p0Enabled << 1) | (config->isSuperVooc2p0Enabled << 0); // 0x00;
	reg_new_data[0x47] = (config->isPeEnabled << 7) | (config->isQc2p0Enabled << 6) | (config->isQc3p0Enabled << 5) | (config->isQc3plusEnabled << 4) | (config->isAfcEnabled << 3) | (config->isFcpEnabled << 2) | (config->isHvScpEnabled << 1) | (config->isLvScpEnabled << 0);									  // 0x00;
	reg_new_data[0x48] = (config->isSfcpEnabled << 7) | (config->isAppleEnabled << 6) | (config->isSamsungEnabled << 5) | (config->isUfcsEnabled << 4) | (config->isPdGatingVoocEnabled << 3) | (config->isPdGatingScpEnabled << 2) | (config->isVoocCableStricMode << 1) | (config->isEmarkEnabled << 0);			  // 0x00;

	reg_new_data[0x49] = (config->isPdEnabled << 7) | (config->isVconnLowPowerModeEnabled << 6) | (config->isInvalidPpsEnabled << 5) | (config->isPdo5v2aEnabled << 4) | (config->isPortCNoLoad << 3) | (config->isTypecCertEnabled << 2) | (config->isPdCertEnabled << 1) | (config->isPdAlertEnabled << 0); // 0x00;

	reg_new_data[0x4A] = (config->isAbnLpsEnabled << 7);
	reg_new_data[0x4B] = 0x00;
	reg_new_data[0x4C] = 0x00;
	reg_new_data[0x4D] = 0x00;
	reg_new_data[0x4E] = 0x00;
	reg_new_data[0x4F] = 0x00;

	reg_new_data[0x50] = ((pps1_min_vol / 100) & 0x00FF); // 0x00;
	reg_new_data[0x51] = 0x00;
	reg_new_data[0x52] = ((pps1_max_vol / 100) & 0x00FF); // 0x00;
	reg_new_data[0x53] = 0x00;
	reg_new_data[0x54] = ((pps1_max_cur / 50) & 0x00FF); ///
	reg_new_data[0x55] = 0x00;
	reg_new_data[0x56] = 0x00;
	reg_new_data[0x57] = 0x00;
	reg_new_data[0x58] = 0x00;
	reg_new_data[0x59] = 0x00;
	reg_new_data[0x5A] = 0x00;
	reg_new_data[0x5B] = 0x00;
	reg_new_data[0x5C] = 0x00;
	reg_new_data[0x5D] = 0x00;
	reg_new_data[0x5E] = 0x00;
	reg_new_data[0x5F] = 0x00;

	reg_new_data[0x60] = UFCS_VOL_CUR_LOW(UFCS_PPS0_DEFAULT_MIN_VOL);
	reg_new_data[0x61] = UFCS_VOL_CUR_HI(UFCS_PPS0_DEFAULT_MIN_VOL);
	reg_new_data[0x62] = UFCS_VOL_CUR_LOW(UFCS_PPS1_DEFAULT_MIN_VOL);
	reg_new_data[0x63] = UFCS_VOL_CUR_HI(UFCS_PPS1_DEFAULT_MIN_VOL);
	reg_new_data[0x64] = UFCS_VOL_CUR_LOW(UFCS_PPS2_DEFAULT_MIN_VOL);
	reg_new_data[0x65] = UFCS_VOL_CUR_HI(UFCS_PPS2_DEFAULT_MIN_VOL);

	reg_new_data[0x68] = UFCS_VOL_CUR_LOW(UFCS_PPS0_DEFAULT_MAX_VOL);
	reg_new_data[0x69] = UFCS_VOL_CUR_HI(UFCS_PPS0_DEFAULT_MAX_VOL);
	reg_new_data[0x6A] = UFCS_VOL_CUR_LOW(UFCS_PPS1_DEFAULT_MAX_VOL);
	reg_new_data[0x6B] = UFCS_VOL_CUR_HI(UFCS_PPS1_DEFAULT_MAX_VOL);
	reg_new_data[0x6C] = UFCS_VOL_CUR_LOW(UFCS_PPS2_DEFAULT_MAX_VOL);
	reg_new_data[0x6D] = UFCS_VOL_CUR_HI(UFCS_PPS2_DEFAULT_MAX_VOL);

	reg_new_data[0x70] = UFCS_VOL_CUR_LOW(UFCS_PPS0_DEFAULT_MAX_CUR);
	reg_new_data[0x71] = UFCS_VOL_CUR_HI(UFCS_PPS0_DEFAULT_MAX_CUR);
	reg_new_data[0x72] = UFCS_VOL_CUR_LOW(UFCS_PPS1_DEFAULT_MAX_CUR);
	reg_new_data[0x73] = UFCS_VOL_CUR_HI(UFCS_PPS1_DEFAULT_MAX_CUR);
	reg_new_data[0x74] = UFCS_VOL_CUR_LOW(UFCS_PPS2_DEFAULT_MAX_CUR);
	reg_new_data[0x75] = UFCS_VOL_CUR_HI(UFCS_PPS2_DEFAULT_MAX_CUR);

	reg_new_data[0x78] = UFCS_VOL_CUR_LOW(UFCS_PPS0_DEFAULT_MIN_CUR);
	reg_new_data[0x79] = UFCS_VOL_CUR_LOW(UFCS_PPS1_DEFAULT_MIN_CUR);
	reg_new_data[0x7A] = UFCS_VOL_CUR_LOW(UFCS_PPS2_DEFAULT_MIN_CUR);

	reg_new_data[0x7C] = ((UFCS_PPS0_DEFAULT_ENABLE << 0) | (UFCS_PPS1_DEFAULT_ENABLE << 1) |
						  (UFCS_PPS2_DEFAULT_ENABLE << 2));

	reg_new_data[0x7D] = ((UFCS_PPS0_DEFAULT_VOL_STEP << 0) | (UFCS_PPS0_DEFAULT_CUR_STEP << 1));
	reg_new_data[0x7E] = ((UFCS_PPS1_DEFAULT_VOL_STEP << 0) | (UFCS_PPS1_DEFAULT_CUR_STEP << 1));
	reg_new_data[0x7F] = ((UFCS_PPS2_DEFAULT_VOL_STEP << 0) | (UFCS_PPS2_DEFAULT_CUR_STEP << 1));

	reg_new_data[0x81] = (11 << 1);

	reg_new_data[0x82] = (uint8_t)(config->vid);
	reg_new_data[0x83] = (uint8_t)((config->vid) >> 8);

	reg_new_data[0x84] = (uint8_t)(config->bcdDevice);
	reg_new_data[0x85] = (uint8_t)((config->bcdDevice) >> 8);

	reg_new_data[0x86] = (uint8_t)(config->pid);
	reg_new_data[0x87] = (uint8_t)((config->pid) >> 8);

	reg_new_data[0x88] = (uint8_t)(config->xid);
	reg_new_data[0x89] = (uint8_t)((config->pid) >> 8);
	reg_new_data[0x8A] = (uint8_t)((config->pid) >> 16);
	reg_new_data[0x8B] = (uint8_t)((config->pid) >> 24);
	// 0x8C
	reg_new_data[NOLOAD_THRESHOLD_REG] = 0x44;
	// 0x8D
	reg_new_data[SYS_SLEEP_CTRL_REG] = 0x00;

	// 0x91-HSMOS电阻值/OCP保护门限
	reg_new_data[0x91] = (Device_Get_Mos_Resistance() << 4) | Device_Get_Vr_Ocp();

	// 0x92-LPS ADC比较门限/LPS异常上管电流比较门限
	reg_new_data[0x92] = (abnCfg.bitFields.lpsAdcCurrTh << 4) | abnCfg.bitFields.lpsHmosCurrTh;

	reg_new_data[0x93] = sceneConversionTime;

	reg_new_data[0x94] = (avsOffsetGearPort1 << 4) | ppsOffsetGearPort1;

	reg_new_data[0x95] = (avsOffsetGearPort2 << 4) | ppsOffsetGearPort2;

	reg_new_data[0x96] = 0x32;

	reg_new_data[0x97] = spr_avs_9v15v_max_cur;
	
	reg_new_data[0x98] = spr_avs_15v20v_max_cur;

	// 0x99-0x9E: Sink Cap Extended 相关寄存器
	reg_new_data[0x99] = 0x00;
	reg_new_data[0x9A] = 0x00;
	reg_new_data[0x9B] = 0x00;
	reg_new_data[0x9C] = 0x00;
	reg_new_data[0x9D] = 0x01;
	reg_new_data[0x9E] = 0x02;
	reg_new_data[0x9F] = 0x00;
}

// 执行寄存器功能
void Execute_Register_Function(u8 regAddr, u8 regData)
{
	u16 temp = 0x00;

	u8 reSetConfig = 0; // config是否改变
	u8 reSetAbnCfg = 0; // abnConfig是否改变

	u8 reSetUfcs = 0;
	u8 ufcsNum = 0;

	u8 reSetPD = 0;
	u8 pdFixNum = 0;
	u8 pdSprNum = 0;
	u8 pdMaxPower = 0;
	u8 pdEprPdoNum = 0;
	u8 pdEprFixNum = 0;

	config_data_t *config = Config_Get();
	abn_config_t abnCfg = {0};
	Abn_Get_Config(&abnCfg);
	switch (regAddr)
	{
	case 0x01:
		// bit7-5
		temp = (regData & 0xE0) >> 5;
		switch (temp)
		{
		case 0x00:
			config->portMode = SINGLE_A_MODE;
			cur_port_mode = SINGLE_A_MODE;
			break;
		case 0x01:
			config->portMode = SINGLE_C_MODE;
			cur_port_mode = SINGLE_C_MODE;
			break;
		case 0x02:
			config->portMode = DUO_AA_MODE;
			cur_port_mode = DUO_AA_MODE;
			break;
		case 0x03:
			config->portMode = DUO_CA_MODE;
			cur_port_mode = DUO_CA_MODE;
			break;
		case 0x04:
			config->portMode = DUO_CC_MODE;
			cur_port_mode = DUO_CC_MODE;
			break;
		}
		Config_Apply();

		// bit2 发送软件复位
		temp = regData & 0x04;
		if (temp)
		{
			Pd_Send_Soft_Reset();
			// 执行完成，清零
			reg_new_data[regAddr] &= 0xFB;
		}
		// bit1 发送Hardreset
		temp = regData & 0x02;
		if (temp)
		{
			Pd_Src_Send_Hard_Reset();
			// 执行完成，清零
			reg_new_data[regAddr] &= 0xFD;
		}

		// bit0  重新广播PDO
		temp = regData & 0x01;
		if (temp)
		{
			Pd_Rebroadcast_Src_Cap();
			// 执行完成，清零
			reg_new_data[regAddr] &= 0xFE;
			Source_Patch_Data.isOccBroadcast = true;
		}
		break;
	case 0x02:
		// bit7-0
		config->pdMaxPower = regData;

		break;
	case 0x03:
		// bit7
		is_pps0_enable = (regData >> 7);
		is_pps1_enable = (regData >> 6) & 0x01;
		is_pps2_enable = (regData >> 5) & 0x01;
		is_spr_avs_enable = (regData >> 4) & 0x01;
		break;
	case 0x05:
		// bit1-0
		fix5v_cur = (fix5v_cur / 10) & 0x00FF;
		fix5v_cur = (fix5v_cur | ((regData & 0x03) << 8)) * 10;
		break;
	case 0x06:
		// bit7-0
		fix5v_cur = (fix5v_cur / 10) & 0xFF00;
		fix5v_cur = (fix5v_cur | regData) * 10;
		break;
	case 0x07:
		// bit2
		is_9v_fixed_gear_enable = (regData & 0x04) >> 2;
		// bit1-0
		fix9v_vol = (fix9v_vol / 50) & 0x00FF;
		fix9v_vol = (fix9v_vol | ((regData & 0x03) << 8)) * 50;
		break;
	case 0x08:
		// bit7-0
		fix9v_vol = (fix9v_vol / 50) & 0xFF00;
		fix9v_vol = (fix9v_vol | regData) * 50;
		break;
	case 0x09:
		// bit1-0
		fix9v_cur = (fix9v_cur / 10) & 0x00FF;
		fix9v_cur = (fix9v_cur | ((regData & 0x03) << 8)) * 10;
		break;
	case 0x0A:
		// bit7-0
		fix9v_cur = (fix9v_cur / 10) & 0xFF00;
		fix9v_cur = (fix9v_cur | regData) * 10;
		break;
	case 0x0B:
		// bit2
		is_12v_fixed_gear_enable = (regData & 0x04) >> 2;
		// bit1-0
		fix12v_vol = (fix12v_vol / 50) & 0x00FF;
		fix12v_vol = (fix12v_vol | ((regData & 0x03) << 8)) * 50;
		break;
	case 0x0C:
		// bit7-0
		fix12v_vol = (fix12v_vol / 50) & 0xFF00;
		fix12v_vol = (fix12v_vol | regData) * 50;
		break;
	case 0x0D:
		// bit1-0
		fix12v_cur = (fix12v_cur / 10) & 0x00FF;
		fix12v_cur = (fix12v_cur | ((regData & 0x03) << 8)) * 10;
		break;
	case 0x0E:
		// bit7-0
		fix12v_cur = (fix12v_cur / 10) & 0xFF00;
		fix12v_cur = (fix12v_cur | regData) * 10;
		break;
	case 0x0F:
		// bit2
		is_15v_fixed_gear_enable = (regData & 0x04) >> 2;
		// bit1-0
		fix15v_vol = (fix15v_vol / 50) & 0x00FF;
		fix15v_vol = (fix15v_vol | ((regData & 0x03) << 8)) * 50;
		break;
	case 0x10:
		// bit7-0
		fix15v_vol = (fix15v_vol / 50) & 0xFF00;
		fix15v_vol = (fix15v_vol | regData) * 50;
		break;
	case 0x11:
		// bit1-0
		fix15v_cur = (fix15v_cur / 10) & 0x00FF;
		fix15v_cur = (fix15v_cur | ((regData & 0x03) << 8)) * 10;
		break;
	case 0x12:
		// bit7-0
		fix15v_cur = (fix15v_cur / 10) & 0xFF00;
		fix15v_cur = (fix15v_cur | regData) * 10;
		break;
	case 0x13:
		// bit2
		is_20v_fixed_gear_enable = (regData & 0x04) >> 2;
		// bit1-0
		fix20v_vol = (fix20v_vol / 50) & 0x00FF;
		fix20v_vol = (fix20v_vol | ((regData & 0x03) << 8)) * 50;
		break;
	case 0x14:
		// bit7-0
		fix20v_vol = (fix20v_vol / 50) & 0xFF00;
		fix20v_vol = (fix20v_vol | regData) * 50;
		break;
	case 0x15:
		// bit1-0
		fix20v_cur = (fix20v_cur / 10) & 0x00FF;
		fix20v_cur = (fix20v_cur | ((regData & 0x03) << 8)) * 10;
		break;
	case 0x16:
		// bit7-0
		fix20v_cur = (fix20v_cur / 10) & 0xFF00;
		fix20v_cur = (fix20v_cur | regData) * 10;
		break;
	case 0x17:
		// bit7-5

		// bit4
		// temp = (regData & 0x10) >> 4;
		if ((reg_new_data[regAddr] & 0x10) == 0x10)
		{
			reg_new_data[regAddr] |= 0x10;
		}

		// bit3
		// temp = (regData & 0x08) >> 3;
		if ((regData & 0x08) == 0x08)
		{
			Port_Force_Disconnect(0x00);
			// 执行完成，清零
			reg_new_data[regAddr] &= 0xF7;
		}
		// bit2
		// temp = (regData & 0x04) >> 2;
		if ((regData & 0x04) == 0x04)
		{
			Port_Force_Disconnect(0x01);
			// 执行完成，清零
			reg_new_data[regAddr] &= 0xFB;
		}
		// bit1
		// temp = (regData & 0x02) >> 1;
		if ((regData & 0x02) == 0x02)
		{
			Port_Reconnect(0x00);
			// 执行完成，清零
			reg_new_data[regAddr] &= 0xFD;
		}
		// bit0
		// temp = (regData & 0x01) >> 0;
		if ((regData & 0x01) == 0x01)
		{
			// 执行完成，清零
			Port_Reconnect(0x01);
			// 执行完成，清零
			reg_new_data[regAddr] &= 0xFE;
		}
		break;
	case 0x18:
		// bit7-0
		pps0_min_vol = regData * 100;
		break;
	case 0x19:
		// bit7 PPS0是否是恒功率模式
		if ((regData & 0x80) > 0)
		{
			pps0_flags = PDO_SPR_PPS_POWER_LIMITED;
		}
		else
		{
			pps0_flags = 0;
		}

		// bit6 PPS1是否是恒功率模式
		if ((regData & 0x40) > 0)
		{
			pps1_flags = PDO_SPR_PPS_POWER_LIMITED;
		}
		else
		{
			pps1_flags = 0;
		}

		// bit6 PPS1是否是恒功率模式
		if ((regData & 0x20) > 0)
		{
			pps2_flags = PDO_SPR_PPS_POWER_LIMITED;
		}
		else
		{
			pps2_flags = 0;
		}

		break;
	case 0x1A:
		// bit7-0
		pps0_max_vol = regData * 100;
		break;
	case 0x1B:
		break;
	case 0x1C:
		// bit7-0
		pps0_max_cur = regData * 50;
		break;

// 更新掉28V相关
#ifndef SW3561X
	case 0x04:
		is_avs0_enable = regData & 0x01;
		break;
	case 0x1D:
		// bit2
		is_28v_fixed_gear_enable = (regData & 0x04) >> 2;
		if (is_28v_fixed_gear_enable)
		{
			fix5v_flags = PDO_FIXED_EPR_MODE | PDO_FIXED_UNCONSTRAINED_POWER | PDO_FIXED_DUAL_DATA_ROLE;
		}
		else
		{
			fix5v_flags = PDO_FIXED_UNCONSTRAINED_POWER;
		}
		// bit1-0
		fix28v_vol = (fix28v_vol / 50) & 0x00FF;
		fix28v_vol = (fix28v_vol | ((regData & 0x03) << 8)) * 50;
		break;
	case 0x1E:
		// bit7-0
		fix28v_vol = (fix28v_vol / 50) & 0xFF00;
		fix28v_vol = (fix28v_vol | regData) * 50;
		break;
	case 0x1F:
		// bit1-0
		fix28v_cur = (fix28v_cur / 10) & 0x00FF;
		fix28v_cur = (fix28v_cur | ((regData & 0x03) << 8)) * 10;
		break;
	case 0x20:
		// bit7-0
		fix28v_cur = (fix28v_cur / 10) & 0xFF00;
		fix28v_cur = (fix28v_cur | regData) * 10;
		break;

	case 0x21:
		break;

		// 更新掉28V相关
	case 0x22:
		// bit7-0
		avs0_min_vol = regData * 100;
		break;
	case 0x23:
		// bit0
		avs0_max_vol = (avs0_max_vol / 100) & 0x00FF;
		avs0_max_vol = (avs0_max_vol | ((regData & 0x01) << 8)) * 100;
		break;
	case 0x24:
		// bit7-0
		avs0_max_vol = (avs0_max_vol / 100) & 0xFF00;
		avs0_max_vol = (avs0_max_vol | regData) * 100;
		break;
	case 0x25:
		// bit7-0
		avs0_power = regData;
		break;
#endif
	case 0x34:
		// bit4
		if (regData & 0x10)
		{
			Adc_Enabled();
		}
		else
		{
			Adc_Disabled();
		}
		// bit3
		if (regData & 0x08)
		{
			Adc_Vin_Enable();
		}
		else
		{
			Adc_Vin_Disable();
		}
		break;
	case 0x35:
		// bit7-0
		config->tfcpMaxPower = regData;
		reSetConfig = 1;
		break;
	case 0x36:
		// bit7-0
		config->voocMaxPower = regData;
		reSetConfig = 1;
		break;
	case 0x37:
		// bit7-0
		config->superVoocMaxPower = regData;
		reSetConfig = 1;
		break;
	case 0x38:
		// bit7-0
		temp = config->scpMaxCurrent & 0x00FF;
		config->scpMaxCurrent = temp | (regData << 8);
		reSetConfig = 1;
		break;
	case 0x39:
		// bit7-0
		temp = config->scpMaxCurrent & 0xFF00;
		config->scpMaxCurrent = temp | regData;
		reSetConfig = 1;
		break;
	case 0x3A:
		// bit7-0
		config->scpMaxPower = regData;
		reSetConfig = 1;
		break;
	case 0x3B:
		// bit7-0
		temp = config->scpMaxVoltage & 0x00FF;
		config->scpMaxVoltage = temp | (regData << 8);
		reSetConfig = 1;
		break;
	case 0x3C:
		// bit7-0
		temp = config->scpMaxVoltage & 0xFF00;
		config->scpMaxVoltage = temp | regData;
		reSetConfig = 1;
		break;
	case 0x3D:
		// bit7-0
		config->scpAdapterType = regData;
		reSetConfig = 1;
		break;
	case 0x3E:
		// bit7-0
		config->dpdmFixPower = regData;
		reSetConfig = 1;
		break;
	case 0x3F:
		// bit5-4
		temp = (regData & 0x30) >> 4;
		config->typecCcTerm = temp;
		// bit1-0
		temp = regData & 0x03;
		config->fixVoltageOffset = temp;
		reSetConfig = 1;
		break;
	case 0x40:
		// bit5-4
		temp = (regData & 0x30) >> 4;
		config->fixCurrentOffset = temp;
		// bit1-0
		temp = regData & 0x03;
		config->qc2p0MaxVoltage = temp;
		reSetConfig = 1;
		break;
	case 0x41:
		// bit5-4
		temp = (regData & 0x30) >> 4;
		config->qc3p0MaxVoltage = temp;
		// bit1-0
		temp = regData & 0x03;
		config->peMaxVoltage = temp;
		reSetConfig = 1;
		break;
	case 0x42:
		// bit5-4
		temp = (regData & 0x30) >> 4;
		config->qc3MaxPower = temp;
		// bit1-0
		temp = regData & 0x03;
		config->p1CurrentLimit = temp;
		reSetConfig = 1;
		break;
	case 0x43:
		// bit5-4
		temp = (regData & 0x30) >> 4;
		config->p2CurrentLimit = temp;
		// bit0
		temp = regData & 0x01;
		config->cableCompensationValue = temp;
		reSetConfig = 1;
		break;
	case 0x44:
		// bit4
		temp = (regData & 0x10) >> 4;
		config->afcMaxVoltage = temp;
		// bit0
		temp = regData & 0x01;
		config->sfcpMaxVoltage = temp;
		reSetConfig = 1;
		break;
	case 0x45:
		// bit4
		temp = (regData & 0xF0) >> 4;
		config->fcpMaxVoltage = temp;
		reSetConfig = 1;
		break;
	case 0x46:
		// bit7-0
		config->isCableCompensaved = (regData & 0x80) >> 7;
		config->isTypecVconnEnabled = (regData & 0x40) >> 6;
		config->isVconnSwapEnabled = (regData & 0x20) >> 5;
		config->isResponseDiscoveryId = (regData & 0x10) >> 4;
		config->isTfcpEnabled = (regData & 0x08) >> 3;
		config->isVooc1p0Enabled = (regData & 0x04) >> 2;
		config->isVooc4p0Enabled = (regData & 0x02) >> 1;
		config->isSuperVooc2p0Enabled = (regData & 0x01) >> 0;
		reSetConfig = 1;
		break;
	case 0x47:
		// bit7-0
		config->isPeEnabled = (regData & 0x80) >> 7;
		config->isQc2p0Enabled = (regData & 0x40) >> 6;
		config->isQc3p0Enabled = (regData & 0x20) >> 5;
		config->isQc3plusEnabled = (regData & 0x10) >> 4;
		config->isAfcEnabled = (regData & 0x08) >> 3;
		config->isFcpEnabled = (regData & 0x04) >> 2;
		config->isHvScpEnabled = (regData & 0x02) >> 1;
		config->isLvScpEnabled = (regData & 0x01) >> 0;
		reSetConfig = 1;
		break;
	case 0x48:
		// bit7-0
		config->isSfcpEnabled = (regData & 0x80) >> 7;
		config->isAppleEnabled = (regData & 0x40) >> 6;
		config->isSamsungEnabled = (regData & 0x20) >> 5;
		config->isUfcsEnabled = (regData & 0x10) >> 4;
		config->isPdGatingVoocEnabled = (regData & 0x08) >> 3;
		config->isPdGatingScpEnabled = (regData & 0x04) >> 2;
		config->isVoocCableStricMode = (regData & 0x02) >> 1;
		config->isEmarkEnabled = (regData & 0x01) >> 0;
		reSetConfig = 1;
		break;
	case 0x49:
		// bit7-0
		config->isPdEnabled = (regData & 0x80) >> 7;
		config->isVconnLowPowerModeEnabled = (regData & 0x40) >> 6;
		config->isInvalidPpsEnabled = (regData & 0x20) >> 5;
		config->isPdo5v2aEnabled = (regData & 0x10) >> 4;
		config->isPortCNoLoad = (regData & 0x08) >> 3;
		config->isTypecCertEnabled = (regData & 0x04) >> 2;
		config->isPdCertEnabled = (regData & 0x02) >> 1;
		config->isPdAlertEnabled = (regData & 0x01) >> 0;
		reSetConfig = 1;
		break;
	case 0x4A:
		// bit7
		config->isAbnLpsEnabled = (regData & 0x80) >> 7;
		reSetConfig = 1;
		break;
	case 0x4F:
		break;
	case 0x50:
		// bit7-0
		pps1_min_vol = regData * 100;
		break;
	case 0x51:
		break;
	case 0x52:
		// bit7-0
		pps1_max_vol = regData * 100;
		break;
	case 0x53:
		break;
	case 0x54:
		// bit7-0
		pps1_max_cur = regData * 50;
		break;
	case 0x59:
		// bit7 中断使能
		if ((regData & 0x80) == 0x80)
		{
			isIRQEnable = 1;
		}
		else
		{
			isIRQEnable = 0;
		}

		// bit0 调压调流中断使能
		if ((regData & 0x80) == 0x80 && (regData & 0x01) == 0x01)
		{
			adjustVolCur = 1;
		}
		else
		{
			adjustVolCurIfExist = 0;
			adjustVolCur = 0;
			INTERRUPGPIO_1;
			reg_new_data[regAddr] &= 0xFE;
		}

	case 0x5A:
		// bit0 调压调流是否发生
		if (adjustVolCurIfExist == 1 && (regData & 0x01) != 0x01)
		{
			INTERRUPGPIO_1;
			adjustVolCurIfExist = 0;
		}
		break;
	case 0x5B:
		// bit7-0
		pps2_min_vol = regData * 100;
		break;
	case 0x5C:
		break;
	case 0x5D:
		// bit7-0
		pps2_max_vol = regData * 100;
		break;
	case 0x5E:
		break;
	case 0x5F:
		// bit7-0
		pps2_max_cur = regData * 50;
		break;

	case 0x81:
		if (((regData >> 1) & 0x1F) != sysBuckClk)
		{
			switch ((regData >> 1) & 0x1F)
			{
			case 16:
				sysBuckClk = ((regData >> 1) & 0x1F);
				Device_Set_Buck_Clock(sysBuckClk);
				Device_Buck_Config(BUCK_CFG_125KHZ_22UH);
				break;
			case 6:
				sysBuckClk = ((regData >> 1) & 0x1F);
				Device_Set_Buck_Clock(sysBuckClk);
				Device_Buck_Config(BUCK_CFG_333KHZ_4UH7);
				break;
			case 11:
				sysBuckClk = ((regData >> 1) & 0x1F);
				Device_Set_Buck_Clock(sysBuckClk);
				Device_Buck_Config(BUCK_CFG_181KHZ_10UH);
				break;
			default:
				reg_new_data[0x81] &= 0x81;
				reg_new_data[0x81] |= (sysBuckClk << 1);
				break;
			}
		}

		Abn_Force_Ctrl(regData & 0x01);
		break;
	case 0x82:
		config->vid = (config->vid & 0xFF00) + regData;
		reSetConfig = 1;
		break;
	case 0x83:
		config->vid = ((config->vid & 0x00FF) + (regData << 8));
		reSetConfig = 1;
		break;
	case 0x84:
		config->bcdDevice = (config->bcdDevice & 0xFF00) + regData;
		reSetConfig = 1;
		break;
	case 0x85:
		config->bcdDevice = ((config->bcdDevice & 0x00FF) + (regData << 8));
		reSetConfig = 1;
		break;
	case 0x86:
		config->pid = (config->pid & 0xFF00) + regData;
		reSetConfig = 1;
		break;
	case 0x87:
		config->pid = ((config->pid & 0x00FF) + (regData << 8));
		reSetConfig = 1;
		break;
	case 0x88:
		config->xid = (config->xid & 0xFFFFFF00) + regData;
		reSetConfig = 1;
		break;
	case 0x89:
		config->xid = ((config->xid & 0xFFFF00FF) + (regData << 8));
		reSetConfig = 1;
		break;
	case 0x8A:
		config->xid = ((config->xid & 0xFF00FFFF) + (regData << 16));
		reSetConfig = 1;
		break;
	case 0x8B:
		config->xid = ((config->xid & 0x00FFFFFF) + (regData << 24));
		reSetConfig = 1;
		break;
	case 0x8C:
		Config_Set_Noload_Threshold(1, (regData & 0x0F));		 // 配置1口限流值
		Config_Set_Noload_Threshold(2, ((regData >> 4) & 0x0F)); // 配置2口限流值
		port1NoLoadCur = (regData & 0x0F) * 3;
		port2NoLoadCur = ((regData >> 4) & 0x0F) * 3;
		break;
	case 0x91:
		Device_Cfg_Mos_Resistance(((regData >> 4) & 0x0F)); // Rmos-lps
		Device_Set_Vr_Ocp((regData & 0x0F));				// ocp-lps

		break;
	case 0x92:
		abnCfg.bitFields.lpsAdcCurrTh = ((regData >> 4) & 0x0F);
		abnCfg.bitFields.lpsHmosCurrTh = (regData & 0x0F);
		reSetAbnCfg = 1;
		break;
	case 0x93:
		sceneConversionTime = regData;
		break;
	case 0x94:
		avsOffsetGearPort1 = ((regData >> 4) & 0x0F);
		ppsOffsetGearPort1 = (regData & 0x0F);
		break;
	case 0x95:
		avsOffsetGearPort2 = ((regData >> 4) & 0x0F);
		ppsOffsetGearPort2 = (regData & 0x0F);
		break;
	case 0x96:
		Device_Buck_Config_Temp_Parameter((regData >> 4) & 0x0F, (regData & 0x0F));
		break;
	case 0x97:
		spr_avs_9v15v_max_cur = regData;
		break;
	case 0x98:
		spr_avs_15v20v_max_cur = regData;
		break;
	case 0x9D:
		// 发送前延时, 单位 10ms, 0=不延时
		sendComandTime = regData;
		break;
	case 0x9E:
		// bit1-使能(主机写), bit7-4-状态码(固件维护)
		// 主机写入时重新恢复状态位
		reg_new_data[regAddr] = (reg_new_data[regAddr] & 0x0F) | (getSinkExtendedStep << 4);
		break;
	default:
		break;
	}

	if (reSetConfig) // 需要重置Config
	{
		Config_Apply();
	}

	if (reSetAbnCfg) // 需要重置abnConfig
	{
		Abn_Set_Config(&abnCfg);
	}

	if ((regAddr > 0x5F) && (regAddr < 0x80)) // 这个范围内需要重置UFCS
	{
		ufcsNum = UFCS_PDO_Update();
		config->ufcsSrcCapNum = ufcsNum;
		Source_Patch_Data.isOccBroadcast = true;
		Ufcs_Set_Power_Change();
		Config_Apply();
	}

	if (((regAddr > 0x01) && (regAddr < 0x17)) ||
		((regAddr > 0x17) && (regAddr < 0x1D)) ||
		((regAddr > 0x4F) && (regAddr < 0x55)) ||
		((regAddr > 0x5A) && (regAddr < 0x60))) // 这个范围内需要重置PDO
	{
		PD_PDO_Update(&pdFixNum, &pdSprNum, &pdEprFixNum, &pdEprPdoNum);

		config->pdMaxPower = regValue(PD_MAX_POWER_REG); // 140w power
		config->sprFixPdoNumber = pdFixNum;				 // spr fix pdo num
		config->sprPdoNumber = pdSprNum;				 // spr pdo num
		config->eprFixPdoNumber = pdEprFixNum;			 // epr fix pdo num
		config->eprPdoNumber = pdEprPdoNum;				 // epr pdo num,it is eprFixPdoNumber add avs pdo num
		Config_Apply();
	}
}

// 读寄存器数据
u8 ReadRegData(u8 addr)
{
	return reg_new_data[addr];
}
// 写寄存器数据(修改寄存器的值)
void WiterRegData(u8 addr, u8 data)
{
	reg_new_data[addr] = data;
}

// 实时更新寄存器数据
void RealTime_Update_Register_Data(u8 regAddr)
{
	u16 temp = 0x00;
	switch (regAddr)
	{
	case 0x00:
		reg_new_data[regAddr] = 0x01;
		break;
	case 0x26:
		// bit7-0
		vout_avg_vol = Adc_Vout_Get();
		reg_new_data[regAddr] = (vout_avg_vol & 0xFF00) >> 8;
		break;
	case 0x27:
		// bit7-0
		reg_new_data[regAddr] = (vout_avg_vol & 0x00FF);
		break;
	case 0x28:
		// bit7-0
		vin_vol = Adc_Vin_Get();
		reg_new_data[regAddr] = (vin_vol & 0xFF00) >> 8;
		break;
	case 0x29:
		// bit7-0
		reg_new_data[regAddr] = (vin_vol & 0x00FF);
		break;
	case 0x2A:
		// bit7-0
		port1_iout_avg_cur = Adc_Iout_Get(0x00);
		reg_new_data[regAddr] = (port1_iout_avg_cur & 0xFF00) >> 8;
		break;
	case 0x2B:
		// bit7-0
		reg_new_data[regAddr] = (port1_iout_avg_cur & 0x00FF);
		break;
	case 0x2C:
		// bit7-0
		adc_temperature = Adc_Get_Channel_Data(ADC_CHL_2);
		reg_new_data[regAddr] = (adc_temperature & 0xFF00) >> 8;
		break;
	case 0x2D:
		// bit7-0
		reg_new_data[regAddr] = (adc_temperature & 0x00FF);
		break;
	case 0x2E:
		// bit7-0
		port2_iout_avg_cur = Adc_Iout_Get(0x01);
		reg_new_data[regAddr] = (port2_iout_avg_cur & 0xFF00) >> 8;
		break;
	case 0x2F:
		// bit7-0
		reg_new_data[regAddr] = (port2_iout_avg_cur & 0x00FF);
		break;
	case 0x30:
		break;
	case 0x31:
		break;
	case 0x32:
		// bit7-0
		reg_new_data[regAddr] = (dac_vol & 0xFF00) >> 8;
		break;
	case 0x33:
		// bit7-0
		reg_new_data[regAddr] = (dac_vol & 0x00FF);
		break;

	case 0x4B:
		// bit7
		if (Device_Get_Fast_Charge_Port() == 0x01)
		{
			reg_new_data[regAddr] = (reg_new_data[regAddr] & 0x7F) | 0x80;
		}
		else
		{
			reg_new_data[regAddr] = (reg_new_data[regAddr] & 0x7F) | 0x00;
		}

		// bit6-0
		reg_new_data[regAddr] = (reg_new_data[regAddr] & 0x80) | Device_Get_Protocol(0x01);
		break;
	case 0x4C:
		// bit7
		if (Device_Get_Fast_Charge_Port() == 0x02)
		{
			reg_new_data[regAddr] = (reg_new_data[regAddr] & 0x7F) | 0x80;
		}
		else
		{
			reg_new_data[regAddr] = (reg_new_data[regAddr] & 0x7F) | 0x00;
		}

		// bit6-0
		reg_new_data[regAddr] = (reg_new_data[regAddr] & 0x80) | Device_Get_Protocol(0x02);
		break;

	case 0x4D:
		// bit7-0
		cur_device_req_vol = Device_Get_Requesting_Voltage();
		reg_new_data[regAddr] = (cur_device_req_vol & 0xFF00) >> 8;
		break;
	case 0x4E:
		// bit7-0
		reg_new_data[regAddr] = (cur_device_req_vol & 0x00FF);
		break;

	case 0x55:
		// bit7-0
		reg_new_data[regAddr] = (pd_request_vol & 0xFF00) >> 8;
		break;
	case 0x56:
		// bit7-0
		reg_new_data[regAddr] = (pd_request_vol & 0x00FF);
		break;

	case 0x57:
		// bit7-0
		reg_new_data[regAddr] = (pd_request_cur & 0xFF00) >> 8;
		break;
	case 0x58:
		// bit7-0
		reg_new_data[regAddr] = (pd_request_cur & 0x00FF);
		break;
	case 0x8F:
		// Port1
		// 设备是否在线(bit7 设备是否在线、bit6设备是否处于快充状态)
		// 协议(bit5-0 设备处于当前协议)
		reg_0x8F_data[0] = 0x8F;
		reg_0x8F_data[1] = ((reg_new_data[0x34] & 0x40) << 1) | ((reg_new_data[0x4B] & 0x80) >> 1) | ((reg_new_data[0x4B] & 0x3F));
		// Vout电压
		reg_0x8F_data[2] = reg_new_data[0x26];
		reg_0x8F_data[3] = reg_new_data[0x27];
		// Iout电流
		reg_0x8F_data[4] = reg_new_data[0x2A];
		reg_0x8F_data[5] = reg_new_data[0x2B];
		// 异常状态
		reg_0x8F_data[6] = reg_new_data[0x8E];

		reg_0x8F_data[7] = (~(reg_0x8F_data[0] + reg_0x8F_data[1] + reg_0x8F_data[2] + reg_0x8F_data[3] +
							  reg_0x8F_data[4] + reg_0x8F_data[5] + reg_0x8F_data[6]));

		break;
	case 0x90:
		// Port2
		// 设备是否在线(bit7 设备是否在线、bit6设备是否处于快充状态)
		// 协议(bit5-0 设备处于当前协议)
		reg_0x90_data[0] = 0x90;
		reg_0x90_data[1] = ((reg_new_data[0x34] & 0x80)) | ((reg_new_data[0x4C] & 0x80) >> 1) | ((reg_new_data[0x4C] & 0x3F));
		// Vout电压
		reg_0x90_data[2] = reg_new_data[0x26];
		reg_0x90_data[3] = reg_new_data[0x27];
		// Iout电流
		reg_0x90_data[4] = reg_new_data[0x2E];
		reg_0x90_data[5] = reg_new_data[0x2F];
		// 异常状态
		reg_0x90_data[6] = reg_new_data[0x8E];

		reg_0x90_data[7] = (~(reg_0x90_data[0] + reg_0x90_data[1] + reg_0x90_data[2] + reg_0x90_data[3] +
							  reg_0x90_data[4] + reg_0x90_data[5] + reg_0x90_data[6]));
		break;

	default:
		break;
	}
}

// 配置初始化
void Slave_Config_Init(void)
{
	u8 pdFixNum = 0;
	u8 pdSprNum = 0;
	u8 pdEprPdoNum = 0;
	u8 pdEprFixNum = 0;

	Nvic_Irq_Enable_All();
	config_data_t *config = Config_Get();
	config->portMode = cur_port_mode; // set to single c mode

	config->ufcsSrcCap = (const volatile uint8_t *)&ufcsPdoOutputCap; // ufcs src cap data pointer
	config->ufcsSrcCapNum = 2;										  // ufcs src cap num
	Ufcs_Set_Power_Change();

	config->dpdmFixPower = 18;

	config->qc2p0MaxVoltage = QC2P0_MAX_VOL_12V;
	config->qc3p0MaxVoltage = QC3P0_MAX_VOL_12V;
	config->qc3MaxPower = QC3_MAX_POWER_18W;
	config->isQc2p0Enabled = true;
	config->isQc3p0Enabled = true;
	config->isQc3plusEnabled = true;

	config->isFcpEnabled = true;
	config->isLvScpEnabled = true;
	config->isHvScpEnabled = true;
	config->scpMaxPower = 23;
	config->scpMaxCurrent = 23;
	config->scpMaxVoltage = 100;

	config->isAfcEnabled = true;
	config->isSfcpEnabled = true;
	config->isPeEnabled = true;
	config->isVooc1p0Enabled = true;
	config->isVooc4p0Enabled = true;
	config->isSuperVooc2p0Enabled = true;

	config->isAppleEnabled = true;
	config->isSamsungEnabled = true;

	config->isCableCompensaved = true;
	config->cableCompensationValue = CABLE_COMPENSAVE_VALUE_100MOHM;

	config->fixVoltageOffset = FIX_VOLTAGE_OFFSET_0MV;

	config->fixCurrentOffset = FIX_CURRENT_OFFSET_300MA;

	config->isPdGatingScpEnabled = false;

	PD_PDO_Update(&pdFixNum, &pdSprNum, &pdEprFixNum, &pdEprPdoNum);

	config->pdSrcCap = pd140wCapIIC;		   // pd src cap dtat pointer
	config->pdMaxPower = PD_DEFAULT_MAX_POWER; // 140w power
	config->sprFixPdoNumber = pdFixNum;		   // spr fix pdo num
	config->sprPdoNumber = pdSprNum;		   // spr pdo num
	config->eprFixPdoNumber = pdEprFixNum;	   // epr fix pdo num
	config->eprPdoNumber = pdEprPdoNum;		   // epr pdo num,it is eprFixPdoNumber add avs pdo num

	config->isPdEnabled = true;
	config->isPdCertEnabled = false;
	config->pdVersionMinor = 6; // for pps 5v min vol cert,must set 8 or above

	config->isPortCNoLoad = false;
	config->isEmarkEnabled = true;
	config->isPdo5v2aEnabled = false;
	config->isVconnSwapEnabled = false;
	config->isResponseDiscoveryId = false;
	config->vid = 0x2FE6;
	config->pid = 0;
	config->xid = 0;
	config->bcdDevice = 0;
	config->isPdAlertEnabled = true;
	config->isInvalidPpsEnabled = true;
	config->scpAdapterType = 0x0A;

	config->isTypecCertEnabled = false;
	config->typecCcTerm = RP_3A;
	config->isTypecVconnEnabled = true;
	config->isVconnLowPowerModeEnabled = false;
	config->typecConnectTime = 130;

	config->p1CurrentLimit = PORT_CURR_LIMIT_3P0A;
	config->p2CurrentLimit = PORT_CURR_LIMIT_3P0A;
	config->isAbnLpsEnabled = false;

	Config_Apply();

	Reg_Init();
	ATM_Init_Normal();
	Adc_Vin_Enable();
	ADC_Config_Init(); // 需要判断vin电压，在进行读取ADC确认VD引脚
	I2C_Drive_Init();
	// 强制关闭芯片温度检测
	Adc_Die_Force_Close(true);
	GPIO_Conifg_Init();
	Abnormal_Init_Hook_Func(abnOccurredHook, abnReleaseHook);
	ATM_Init_Disable();
}

uint8_t ocpNotCheckPort = 0;
uint32_t ocpNotCheckTime = 0;

// 检测端口是否
void Check_Port_Exist(void)
{
	u8 newPortIsExist = 0x00;

	if (ocpNotCheckPort == 0)
	{
		reg_new_data[0x34] = (reg_new_data[0x34] & 0x3F) | (Device_Get_Port_State() << 6);
	}
	else
	{
		if (My_GetSystemTimePass(ocpNotCheckTime) > 2000)
		{
			ocpNotCheckPort = 0;
		}
	}

	// bit5
	if (Device_Is_In_Fast_Charge())
	{
		reg_new_data[0x34] = (reg_new_data[0x34] & 0xDF) | 0x20;
	}
	else
	{
		reg_new_data[0x34] = (reg_new_data[0x34] & 0xDF);
	}

	if ((reg_new_data[0x34] & 0xC0) != 0x00)
	{
		newPortIsExist = 0x01;
	}
	else
	{
		newPortIsExist = 0x00;
	}

	if ((isIRQEnable == 0) && (oldPortIsExist != newPortIsExist))
	{
		oldPortIsExist = newPortIsExist;
		if (oldPortIsExist == 1)
		{
			INTERRUPGPIO_0;
		}
		else
		{
			INTERRUPGPIO_1;
		}
	}
}

// 0x99-0x9E: 读取对端 Sink Capabilities Extended (与参考工程 D40F 一致)
void GetSinkExtended(void)
{
	u8 curProtocol = 0;
	static u8 curStep = 0;
	static u16 onlineTime = 0;
	static u8 lastStep = 0xFF;

	switch (curStep)
	{
	case 0:
		// 使能位: 0x9E bit1
		if ((reg_new_data[0x9E] & 0x02) != 0x00)
		{
			curStep = 1;
			onlineTime = 0;
		}
		break;
	case 1:
		// 等待端口在线, 再等 sendComandTime*100 个主循环
		if ((reg_new_data[0x34] & 0x40) != 0x00)
		{
			onlineTime++;
			if (onlineTime > (u16)(sendComandTime * 100))
			{
				getSinkExtendedStep = 1;
				curStep = 2;
				onlineTime = 0;
			}
		}
		else
		{
			onlineTime = 0;
		}
		break;
	case 2:
		// 等待 PD 协商完成
		curProtocol = reg_new_data[0x4B] & 0x7F;
		if ((curProtocol == FC_PD_Fix5V) || (curProtocol == FC_PD_FixHV) ||
			(curProtocol == FC_PD_PPS) || (curProtocol == FC_PD_EPR_HV) ||
			(curProtocol == FC_PD_AVS) || (curProtocol == FC_PD_SPR_AVS))
		{
			if (Pd_Is_Ready())
			{
				User_Pd_Policy_Get_Sink_Cap_Extended_Run();
				curStep = 3;
				getSinkExtendedStep = 2;
				// printf("[SCE] TX Get_Sink_Cap_Extended (proto=%d, delay=%d)\r\n",
					//    (unsigned int)curProtocol, (unsigned int)sendComandTime);
			}
			else
			{
				onlineTime++;
				if (onlineTime > 1000)
				{
					onlineTime = 0;
					curStep = 3;
					getSinkExtendedStep = 4;
					// printf("[SCE] FAIL: Pd_Is_Ready timeout\r\n");
				}
			}
		}
		else
		{
			onlineTime++;
			if (onlineTime > 2000)
			{
				onlineTime = 0;
				curStep = 3;
				getSinkExtendedStep = 4;
				// printf("[SCE] FAIL: no PD protocol\r\n");
			}
		}
		if ((reg_new_data[0x34] & 0x40) == 0x00)
		{
			curStep = 1;
			onlineTime = 0;
			getSinkExtendedStep = 0;
		}
		break;
	case 3:
		// 拔出或关闭使能后回到空闲, 允许下次重新触发
		if ((reg_new_data[0x34] & 0x40) == 0x00)
		{
			curStep = 0;
			getSinkExtendedStep = 0;
		}
		else if ((reg_new_data[0x9E] & 0x02) == 0x00)
		{
			curStep = 0;
			getSinkExtendedStep = 0;
		}
		break;
	default:
		curStep = 0;
		break;
	}

	// 0x9E bit7-4 状态码回写: 仅在跳变时写一次
	if (lastStep != getSinkExtendedStep)
	{
		lastStep = getSinkExtendedStep;
		reg_new_data[0x9E] = (reg_new_data[0x9E] & 0x0F) | (getSinkExtendedStep << 4);
	}
}

#define REG_SUM 0x93

// IIC从机主策略
void Slave_Policy(void)
{
	u8 tmpAddr = 0;

	// 兼容性策略
	Device_Compatibility_Policy();

	UnVdm_Com_Policy();

	if (Read_Fifo(&tmpAddr) == 0)
	{
		Execute_Register_Function(tmpAddr, reg_new_data[tmpAddr]); // 修改寄存器功能实现
	}

	// 实时获取最新寄存器数据
	RealTime_Update_Register_Data(reg_polling);
	// 设备是否在线功能实现0x59
	Check_Port_Exist();

	// 0x99-0x9E: 读取对端 Sink Capabilities Extended
	GetSinkExtended();

	reg_polling++;

	if (reg_polling > REG_SUM)
	{
		reg_polling = 0;
	}
}

// *        bit8  : VOUT SCP

// *        bit9  : VOUT Slow OVP
// *        bit10 : VOUT Fast OVP
// *        bit11 : VIN UVLO
// *        bit12 : VIN OVP

// *        bit13 : VOUT UVP

// *        bit14 : DIE Warn
// *        bit15 : DM2 Weak Short

u8 startCheckAbnFlag = 0;
u32 startCheckAbnTime = 0;

// 异常发生中
static void abnOccurredHook(u32 pending)
{
	uint32_t abnormalState = pending;

	// VCONN-OCP-雷电线兼容性补丁
	if (abnormalState & 0x0060)
	{
		config_data_t *config = Config_Get();
		config->isTypecVconnEnabled = false;
		Config_Apply();
		// Pd_Src_Cap_Ignore_Emarker(true);
	}

	if (abnormalState & 0x7F00)
	{
		ocpNotCheckPort = 0x01;
		ocpNotCheckTime = My_GetSystemTimeMark();
	}

	reg_new_data[SYS_ABN_STATE_REG] = (uint8_t)(pending >> 8);

	if (reg_new_data[SYS_ABN_STATE_REG] & 0x21)
	{
		startCheckAbnFlag = 1;
		startCheckAbnTime = My_GetSystemTimeMark();
	}
}

// *        bit5  : VOUT SLOW OVP Release
// *        bit6  : VOUT FAST OVP  Release
// *        bit7  ：VIN UVLO Release
// *        bit8  ：VIN OVP Release

// *        bit9  ：DIE WARN Release
// *        bit10 ：DM2 Weak Short Release
// 异常复位
static void abnReleaseHook(u32 pending)
{
	// reg_new_data[SYS_ABN_STATE_REG] = (uint8_t)(pending >> 8);
	uint32_t abnormalState = (pending & 0x07E0) >> 4;
	u8 lowState = abnormalState & 0x1E;
	u8 highState = abnormalState & 0x60;
	u8 abnState = (highState << 1) | lowState;
	reg_new_data[SYS_ABN_STATE_REG] &= ~abnState;
}

// bit8-bit0  : VOUT SCP   bit13-bit5 : VOUT UVP  保护没有恢复异常标志，需要写逻辑
// 逻辑，只要Gate关闭就代表没有以上两个异常保护

void CheckAbnSCPOrUVP(void)
{
	if (startCheckAbnFlag == 1)
	{
		if (My_GetSystemTimePass(startCheckAbnTime) > 500)
		{
			if (!Power_Is_Gate_On(0x01) || !Power_Is_Gate_On(0x02))
			{
				reg_new_data[SYS_ABN_STATE_REG] &= ~0x21;
				startCheckAbnFlag = 0;
			}
		}
	}
}
