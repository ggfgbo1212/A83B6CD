/*
***************************************************************************
* Copyright (C) 2022-2023 Ismartware Limited. All rights reserved. 
* File name: iic_slave_policy.h
* Description: functions about ic device to drive the hard device .
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date: 
* History: 
***************************************************************************
*/

#include "api.h"
#include "config.h"
#include "ufcs_caps.h"
#include "pdo.h"

#include "zr_rcc.h"
#include "pd_message.h"

#include "zr_i2c.h"
#include "zr_rcc.h"
#include "zr_gpio.h"
#include "zr_nvic.h"
#include "zr_adc.h"
#include "zr_atm.h"

//IIC初始化
#define I2C_SDA_M_FUNC (GPIO1_FUNC_SEL_I2C_SDA)
#define I2C_SCK_M_FUNC (GPIO0_FUNC_SEL_I2C_SCK)

#define I2C_SDA_GPIO (GPIO1) 
#define I2C_SCK_GPIO (GPIO0)



//中断
#define INTERRUPGPIO	GPIO2
#define INTERRUPGPIO_FUNC_SEL_GPIO 	GPIO2_FUNC_SEL_GPIO
#define INTERRUPGPIO_1	Gpio_Set_Output_Value(INTERRUPGPIO, 1);
#define INTERRUPGPIO_0	Gpio_Set_Output_Value(INTERRUPGPIO, 0);



/*********************************UFCS默认设置************************************/
#define UFCS_PPS0_DEFAULT_MIN_VOL	5500
#define UFCS_PPS0_DEFAULT_MAX_VOL 	11000
#define UFCS_PPS0_DEFAULT_MAX_CUR	3000
#define UFCS_PPS0_DEFAULT_MIN_CUR	1000
#define UFCS_PPS0_DEFAULT_VOL_STEP	UFCS_VOL_MIN_STEP_10MV
#define UFCS_PPS0_DEFAULT_CUR_STEP	UFCS_CURR_STEP_50MA

#define UFCS_PPS1_DEFAULT_MIN_VOL 	11000
#define UFCS_PPS1_DEFAULT_MAX_VOL 	21000
#define UFCS_PPS1_DEFAULT_MAX_CUR	3000
#define UFCS_PPS1_DEFAULT_MIN_CUR	1000
#define UFCS_PPS1_DEFAULT_VOL_STEP	UFCS_VOL_MIN_STEP_10MV
#define UFCS_PPS1_DEFAULT_CUR_STEP	UFCS_CURR_STEP_50MA

#define UFCS_PPS2_DEFAULT_MIN_VOL 	11000
#define UFCS_PPS2_DEFAULT_MAX_VOL 	21000
#define UFCS_PPS2_DEFAULT_MAX_CUR	3000
#define UFCS_PPS2_DEFAULT_MIN_CUR	1000
#define UFCS_PPS2_DEFAULT_VOL_STEP	UFCS_VOL_MIN_STEP_10MV
#define UFCS_PPS2_DEFAULT_CUR_STEP	UFCS_CURR_STEP_50MA

#define UFCS_VOL_CUR_LOW(vol_cur)	(u8)(vol_cur / 10)
#define UFCS_VOL_CUR_HI(vol_cur)	(u8)((vol_cur / 10) >> 8)	

#define UFCS_PPS0_DEFAULT_ENABLE	1
#define UFCS_PPS1_DEFAULT_ENABLE	1
#define UFCS_PPS2_DEFAULT_ENABLE	0

extern u8 reg_new_data[0x100];

void PD_PDO_Update(u8 *fix_sum, u8 *spr_sum, u8 *epr_fix, u8 *epr_sum);

//从机配置初始化
void Slave_Config_Init(void);

//从机执行策略
void Slave_Policy(void);

u8 ReadRegData(u8 addr);

void WiterRegData(u8 addr,u8 data);

void CheckAbnSCPOrUVP(void);

