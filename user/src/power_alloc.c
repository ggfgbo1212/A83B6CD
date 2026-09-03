/*
***************************************************************************
* Copyright (C) 2020-2020 Ismartware Limited. All rights reserved. 
* File name: pd3_1_protocol.c
* Description: pd31 protocol layer driver file
* Author:  BuJianLaiZhe
* Version: V0.1
* Date: 2022.08
* History: 
***************************************************************************
*/
#include "power_alloc.h"
#include "ufcs_caps.h"
#include "pdo.h"
#include "config.h"
#include "api.h"
#include "user_define.h"

#define	PORT1		0 
#define	PORT2		1
#define	PORT3		2
#define	PORT4		3
#define	PORT5		4

#define	SUM_1	    0 
#define	SUM_2	    1
#define	SUM_3		2
#define	SUM_4		3
#define	SUM_5		4

#define NORMAL_TEMP 0
#define OVER_TEMP   1

#define NO_RELATE   0
#define RELATED     1

#define RELATED_1   0
#define RELATED_2   1



static const ufcs_output_cap_t ufcs30wOutputCap[] = {
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
        .fields.hiMaxCurrent = UFCS_CAP_SET_HI_BYTE(150),        // unit : 10mA
        .fields.loMaxCurrent = UFCS_CAP_SET_LO_BYTE(150),        // unit : 10mA
        .fields.minCurrent = 100,                                // unit : 10mA
    },
};


const u32 pd140wCap[9] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_EPR_MODE | PDO_FIXED_UNCONSTRAINED_POWER | PDO_FIXED_DUAL_DATA_ROLE, PDO_FIXED_NO_PEAK_CURRENT),  //support epr mode, accept dr swap, not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 5000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_PPS(5000, 3300, 21000, 0),  //not support constant power!!!!!!!
    0,   // it shall set to zero when the EPR pdo is invlaid
    PDO_FIXED_OTHERS(28000, 5000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
		PDO_AVS(15000, 28000, 140),
};

const u32 pd100wCap[6] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),  //support epr mode, accept dr swap, not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 5000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_PPS(5000, 3300, 21000, 0),  //not support constant power!!!!!!!
};

const u32 pd75wCap[6] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),  //not support epr mode, not support dr swap, not support peak currrent!!!!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 3750, PDO_FIXED_PEAK_CURRENT_MODE1), //enable peak currrent when sink request this fix!!!!!!
    PDO_PPS(3750, 3300, 21000, PDO_SPR_PPS_POWER_LIMITED),  //support constant power(max 60w)!!!!!!!!
};

const u32 pd65wCap[6] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),  //not support epr mode, not support dr swap, not support peak currrent!!!!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 3250, PDO_FIXED_PEAK_CURRENT_MODE1), //enable peak currrent when sink request this fix!!!!!!
    PDO_PPS(3250, 3300, 21000, PDO_SPR_PPS_POWER_LIMITED),  //support constant power(max 60w)!!!!!!!!
};


const u32 pd45wCap[6] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),  //not support epr mode, not support dr swap, not support peak currrent!!!!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 2250, PDO_FIXED_PEAK_CURRENT_MODE1), //enable peak currrent when sink request this fix!!!!!!
    PDO_PPS(2250, 3300, 21000, PDO_SPR_PPS_POWER_LIMITED),  //support constant power(max 60w)!!!!!!!!
};

const u32 pd30wCap[6] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),  //not support epr mode, not support dr swap, not support peak currrent!!!!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 2500, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(15000, 2000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(20000, 1500, PDO_FIXED_PEAK_CURRENT_MODE1), //enable peak currrent when sink request this fix!!!!!!
    //PDO_PPS(3000, 3300, 11000, PDO_SPR_PPS_POWER_LIMITED),  //support constant power(max 60w)!!!!!!!!
};

const u32 pd20wCap[4] = 
{
    PDO_FIXED_VSAFE5V(3000, PDO_FIXED_UNCONSTRAINED_POWER, PDO_FIXED_NO_PEAK_CURRENT),  //not support epr mode, not support dr swap, not support peak currrent!!!!!!!!!
    PDO_FIXED_OTHERS(9000, 3000, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_FIXED_OTHERS(12000, 2500, PDO_FIXED_NO_PEAK_CURRENT), //not support peak currrent!!!!!!
    PDO_PPS(2200, 3300, 11000, PDO_SPR_PPS_POWER_LIMITED),  //support constant power(max 60w)!!!!!!!!
};


pdo_cfg_t pdoSrcCap[8] = 
{
	[0].pdSrcCap = pd30wCap,
	[0].sprFixPdoNumber = 5,
	[0].sprPdoNumber	= 5,
	[0].eprFixPdoNumber = 0,
	[0].eprPdoNumber = 0,

	[1].pdSrcCap = pd45wCap,
	[1].sprFixPdoNumber = 5,
	[1].sprPdoNumber	= 5,
	[1].eprFixPdoNumber = 0,
	[1].eprPdoNumber = 0,

	[2].pdSrcCap = pd65wCap,
	[2].sprFixPdoNumber = 5,
	[2].sprPdoNumber	= 5,
	[2].eprFixPdoNumber = 0,
	[2].eprPdoNumber = 0,
	
	[3].pdSrcCap = pd75wCap,
	[3].sprFixPdoNumber = 5,
	[3].sprPdoNumber	= 6,
	[3].eprFixPdoNumber = 0,
	[3].eprPdoNumber = 0,

	[4].pdSrcCap = pd100wCap,
	[4].sprFixPdoNumber = 5,
	[4].sprPdoNumber	= 6,
	[4].eprFixPdoNumber = 0,
	[4].eprPdoNumber = 0,

	[5].pdSrcCap = pd140wCap,
	[5].sprFixPdoNumber = 5,
	[5].sprPdoNumber	= 5,
	[5].eprFixPdoNumber = 1,
	[5].eprPdoNumber = 1,
};

u8 Calc_Online_Num( port_device_t  *portChart)
{
	u8 i = 0;
	u8 tmpValue = 0;
	port_device_t  *tmpPortChart;
	
	tmpPortChart = portChart;
	
	for(i = 0; i < PORT_SUM; i++)
	{
		if(GET_PORT_STATE(tmpPortChart -> portPowerChart) > 0)
		{
			tmpValue += 1;
		}
		
		(void)*tmpPortChart ++;
	}	
	return tmpValue;
}

void Cfg_Sys_Pdo(u16 power)
{
	pdo_cfg_t *pdoCfgValue; 
	config_data_t* config;
	uint8_t pos = 0;
	
	switch(power)
	{
		case 300:
				pos = 0;
				break;
		case 450:
				pos = 1;
				break;
		case 650:
				pos = 2;
				break;
		case 750:
				pos = 3;
				break;
		case 1000:
				pos = 4;
				break;
		case 1400:
				pos = 5;
				break;
	}
	pdoCfgValue = &pdoSrcCap[pos];

	config = Config_Get();
	
	config -> pdMaxPower = power / 10;
	config -> pdSrcCap = pdoCfgValue -> pdSrcCap;
	config -> sprFixPdoNumber = pdoCfgValue -> sprFixPdoNumber;
	config -> sprPdoNumber =  pdoCfgValue -> sprPdoNumber;
	config -> eprFixPdoNumber = pdoCfgValue -> eprFixPdoNumber;
	config -> eprPdoNumber = pdoCfgValue -> eprPdoNumber;
}


/*info: 功率变换为对应的功率档位，用户自定义档位阈值
*/
uint8_t Power_To_Gear(uint16_t power)
{
 uint8_t  powerGear = 0;
 uint32_t powerMW  = power * 100; //功率单位以mw计算。

 if(powerMW <= POWER_GEAR_1)  	
 {
	 powerGear = 0;
 }		
 
 else if(powerMW <= POWER_GEAR_2)	
 {
	 powerGear = 1;
 }
 else if(powerMW <= POWER_GEAR_3)	
 {
	 powerGear = 2;
 }
 else if(powerMW <= POWER_GEAR_4)	
 {
	 powerGear = 3;
 }
 else if(powerMW <= POWER_GEAR_5)
 {
	 powerGear = 4;
 }
 else 
 {
   powerGear = 5;
 }
 
 return powerGear;
}


/*info: 根据电压获取当然所在的档位
具体电压档位，参考power_alloc.h
*/
uint8_t Get_Vol_Gear(uint16_t vol)
{
  uint8_t  volGear = 0;
	
	if(vol <= VOL_GEAR_1) //电压小于3V
	{
	 volGear = 0;
	}
	
	else if(vol <= VOL_GEAR_2) //电压小于5.3V
	{
	 volGear = 1;
	}
	
  else if(vol <= VOL_GEAR_3) //电压小于9.3V
	{
	 volGear = 2;
	}
	
  else if(vol <= VOL_GEAR_4)//电压小于12.3V
	{
	 volGear = 3;
	}
 
 
  else if(vol <= VOL_GEAR_5)//电压小于15.3V
	{
	 volGear = 4;
	}
 
 
  else if(vol <= VOL_GEAR_6)//电压小于20.1V
	{
	 volGear = 5;
	}
 
	else
	{
	 volGear = 6;
	}
 return volGear;
}

/******************** 简单处理 **************************/
static u32 Ask_Port1_Power(u8 onlineSum, port_device_t *portChart, u8 tmpertureState);
static u32 Ask_Port2_Power(u8 onlineSum, port_device_t *portChart, u8 tmpertureState);
static u32 Ask_Port3_Power(u8 onlineSum, port_device_t *portChart, u8 tmpertureState);

//端口规则变动，因为主机位于C3口，但是端口分配习惯1口为3566，所以调节下请求的顺序。
u32 Easy_Handle_Ask(u8 port, port_device_t *portChart, u8 tmpertureState)
{
	u32 tmpPower = 0;
	u8 onlineSum        =  Calc_Online_Num(portChart);
	
	switch(port)
	{
		case 1: 
				tmpPower = Ask_Port1_Power(onlineSum,  portChart, tmpertureState);
				break;
		case 2: 
				tmpPower = Ask_Port2_Power(onlineSum,  portChart, tmpertureState);
				break;
		case 3: 
				tmpPower = Ask_Port3_Power(onlineSum,  portChart, tmpertureState);
				break;
	}
	return tmpPower;
}

static u32 Ask_Port1_Power(u8 onlineSum, port_device_t *portChart, u8 tmpertureState)
{
	u32 tmpPower = 0;
	port_device_t *port0Chart;
	port_device_t *port1Chart;
	
	port0Chart = portChart;
	port1Chart = portChart + 1;
	
	switch(onlineSum)
	{
		case 1:
				if(tmpertureState == NORMAL_TEMP)
				{
					tmpPower = 1400;
				}
				else
				{
					tmpPower = 1000;			
				}
				break;
		case 2:
				if(tmpertureState == NORMAL_TEMP)
				{
					if(GET_PORT_STATE(port1Chart -> portPowerChart) > 0)
					{
						tmpPower = 650;
					}
					else
					{
						tmpPower = 1000;				
					}
				}
				else
				{
					if(GET_PORT_STATE(port1Chart -> portPowerChart) > 0)
					{
						tmpPower = 450;
					}
					else
					{
						tmpPower = 650;				
					}				
				}
				break;
		case 3:	
				if(tmpertureState == NORMAL_TEMP)
				{
					if(GET_PORT_STATE(port1Chart -> portPowerChart) > 0)
					{
						tmpPower = 450;
					}
					else
					{
						tmpPower = 1000;				
					}		
				}
				else
				{
					if(GET_PORT_STATE(port1Chart -> portPowerChart) > 0)
					{
						tmpPower = 300;
					}
					else
					{
						tmpPower = 650;				
					}					
				}
				break;
		case 4:
				if(tmpertureState == NORMAL_TEMP)
				{
						tmpPower = 450;					
				}
				else
				{
						tmpPower = 300;								
				}
				break;
	}
	return tmpPower;
}

static u32 Ask_Port2_Power(u8 onlineSum, port_device_t *portChart, u8 tmpertureState)
{
	u32 tmpPower = 0;
	port_device_t *port0Chart;
	port_device_t *port1Chart;
	
	port0Chart = portChart;
	port1Chart = portChart + 1;
	
	switch(onlineSum)
	{
		case 1:
				if(tmpertureState == NORMAL_TEMP)
				{
						tmpPower = 1400;					
				}
				else
				{
						tmpPower = 1000;								
				}
				break;
		case 2:
				if(tmpertureState == NORMAL_TEMP)
				{
					if(GET_PORT_STATE(port0Chart -> portPowerChart) > 0)
					{
						tmpPower = 650;
					}
					else
					{
						tmpPower = 1000;				
					}
				}
				else
				{
					if(GET_PORT_STATE(port0Chart -> portPowerChart) > 0)
					{
						tmpPower = 450;
					}
					else
					{
						tmpPower = 650;				
					}				
				}
				break;
		case 3:	
				if(tmpertureState == NORMAL_TEMP)
				{
					if(GET_PORT_STATE(port0Chart -> portPowerChart) > 0)
					{
						tmpPower = 650;
					}
					else
					{
						tmpPower = 1000;				
					}		
				}
				else
				{
					if(GET_PORT_STATE(port0Chart -> portPowerChart) > 0)
					{
						tmpPower = 300;
					}
					else
					{
						tmpPower = 650;				
					}					
				}
				break;
		case 4:
				if(tmpertureState == NORMAL_TEMP)
				{
						tmpPower = 650;					
				}
				else
				{
						tmpPower = 300;								
				}
				break;
	}
	return tmpPower;
}

static u32 Ask_Port3_Power(u8 onlineSum, port_device_t *portChart, u8 tmpertureState)
{
	port_device_t *port3Chart;
	u32 tmpPower = 300;
	
	port3Chart = portChart + 3;	
	
	if(GET_PORT_STATE(port3Chart -> portPowerChart) > 0)
	{
		tmpPower = 250;
	}
	
	return tmpPower;
}
