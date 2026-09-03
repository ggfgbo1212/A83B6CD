/*
***************************************************************************
* Copyright (C) 2022-2023 Ismartware Limited. All rights reserved. 
* File name: device_drive.c
* Description: functions about ic device to drive the hard device .
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date: 
* History: 
***************************************************************************
*/

#include "device_drive.h" 
#include "ic_lib_rely.h" 
#include "sw3566_i2c_lib.h"
#include "zr_atm.h"
#include "zr_nvic.h"
#include "api.h"

extern device_policy_t devicePolicy;
extern usr_i2c_driver_t i2c_driver_buffer;
static void Set_Get_State(device_policy_t  *tmpDevicePolicy);

void IC_Device_Init(device_policy_t  *tmpDevicePolicy)
{
	uint32_t temp = 0;
	//主机初始化
	SW_Func1 ->SW_Init(PORT1);	
	//从机初始化
	//SW_Func2 ->SW_Init(PORT2);
	SW_Func3 ->SW_Init(PORT3);
	SW_Func4 ->SW_Init(PORT4);
	//获取端口状态模式
	Set_Get_State(&devicePolicy);
}
////说明通过I2C连接其它3566设备，请务必设置模式为直接获取。
static void Set_Get_State(device_policy_t  *tmpDevicePolicy)
{
	#if GET_PORT1_STATE_MODE
	
	tmpDevicePolicy ->getPortMode[0].mode = GET_PORT1_STATE_MODE;
	tmpDevicePolicy ->getPortMode[0].loadCurrent =    P1_LOAD_CURRENT_MA;  //ma
	tmpDevicePolicy ->getPortMode[0].unloadCurrent =  P1_UNLOAD_CURRENT_MA; //ma
	
	#endif
	
	#if GET_PORT2_STATE_MODE
	
	tmpDevicePolicy ->getPortMode[1].mode = GET_PORT2_STATE_MODE;
	tmpDevicePolicy ->getPortMode[1].loadCurrent =    P2_LOAD_CURRENT_MA;  //ma
	tmpDevicePolicy ->getPortMode[1].unloadCurrent =  P2_UNLOAD_CURRENT_MA; //ma
	
	#endif 
	
	#if GET_PORT3_STATE_MODE
	
	tmpDevicePolicy ->getPortMode[2].mode = GET_PORT3_STATE_MODE;
	tmpDevicePolicy ->getPortMode[2].loadCurrent =    P3_LOAD_CURRENT_MA;  //ma
	tmpDevicePolicy ->getPortMode[2].unloadCurrent =  P3_UNLOAD_CURRENT_MA; //ma
	
	#endif 
	
	#if GET_PORT4_STATE_MODE
	
	tmpDevicePolicy ->getPortMode[3].mode = GET_PORT4_STATE_MODE;
	tmpDevicePolicy ->getPortMode[3].loadCurrent =    P4_LOAD_CURRENT_MA;  //ma
	tmpDevicePolicy ->getPortMode[3].unloadCurrent =  P4_UNLOAD_CURRENT_MA; //ma
	
	#endif 
	
	tmpDevicePolicy ->getPortMode[0].loadDebounce =   P1_LOAD_DEBUOUNCE_MS;//ms
	tmpDevicePolicy ->getPortMode[0].unLoadDebounce = P1_UNLOAD_DEBOUNCE_MS; //ms

	tmpDevicePolicy ->getPortMode[1].loadDebounce =   P2_LOAD_DEBUOUNCE_MS;//ms
	tmpDevicePolicy ->getPortMode[1].unLoadDebounce = P2_UNLOAD_DEBOUNCE_MS; //ms
	
	tmpDevicePolicy ->getPortMode[2].loadDebounce =   P3_LOAD_DEBUOUNCE_MS;//ms
	tmpDevicePolicy ->getPortMode[2].unLoadDebounce = P3_UNLOAD_DEBOUNCE_MS; //ms
	
	tmpDevicePolicy ->getPortMode[3].loadDebounce =   P4_LOAD_DEBUOUNCE_MS;//ms
	tmpDevicePolicy ->getPortMode[3].unLoadDebounce = P4_UNLOAD_DEBOUNCE_MS; //ms
}	



//求出最近的一位
uint8_t Bit_To_Num(uint8_t tmp)
{
	uint8_t i = 0;

	for(i = 0; i < 5; i++)
	{
		if((tmp & (1 << i)) > 0)
		{
			return i;
		}
	}
	
	return 0;
}

struct sw_func * PortToFunc(u8 port)
{
	struct sw_func *tmpFunc;
		
	switch(port)
	{
		case 0:
			tmpFunc =  (struct sw_func *)SW_Func1;
			break;
		case 1:
			tmpFunc =  (struct sw_func *)SW_Func2;
			break;
		case 2:
			tmpFunc =  (struct sw_func *)SW_Func3;
			break;
		case 3:
			tmpFunc =  (struct sw_func *)SW_Func4;
			break;
		case 4:
			tmpFunc =  (struct sw_func *)SW_Func5;
			break;
	}
	return tmpFunc;
}


static void Coey_Mode_About_Sleep(device_policy_t  *tmpDevicePolicy, u8 port,  get_port_state_mode *get_state_mode)
{ 
	get_state_mode-> mode = tmpDevicePolicy->getPortMode[port].mode;
	get_state_mode->loadCurrent = tmpDevicePolicy->getPortMode[port].loadCurrent;
	get_state_mode->unloadCurrent = tmpDevicePolicy->getPortMode[port].unloadCurrent;
	
	get_state_mode->loadDebounce = tmpDevicePolicy->getPortMode[port].loadDebounce;
	get_state_mode->unLoadDebounce = tmpDevicePolicy->getPortMode[port].unLoadDebounce;
}

void IC_Device_Read(device_policy_t  *tmpDevicePolicy)
{
	const u8 port_ic[MODULE_NUM] = {PORT1_IC, PORT2_IC, PORT3_IC, PORT4_IC, PORT5_IC};
	static uint8_t sysBusyCnt = 0;//此处谁设置谁才有资格释放。
	static uint8_t checkPortNum;
		   uint8_t tmpState = 0;
		   uint32_t tmpCurrent = 0;
	struct sw_func *tmpFunc ;
			uint8_t tmpValue = 0;
			int tmpTemperture = 0;
	
	u32		tmpVolCurValue = 0;
	
	static uint8_t oneMoreCheckFlag = 0;
	
	get_port_state_mode tmpPortStateMode;	
	
	if(sysBusyCnt == 0) //当前系统不忙,依据优先级找出需要更新的
	{
		if(tmpDevicePolicy -> broadcastUpdate != 0)
		{
			checkPortNum = (Bit_To_Num(tmpDevicePolicy -> broadcastUpdate));
			sysBusyCnt = SET_DEVICE_REBROADCAST;
		}
		else if(tmpDevicePolicy -> powerUpdate != 0)
		{
			checkPortNum = (Bit_To_Num(tmpDevicePolicy -> powerUpdate));
			sysBusyCnt = SET_DEVICE_POWER;		
		}
		else if(tmpDevicePolicy -> tempUpdate != 0)
		{
			sysBusyCnt = READ_DEVICE_TEMPER;
		}
		else if(tmpDevicePolicy -> volUpdate != 0)
		{
			checkPortNum = (Bit_To_Num(tmpDevicePolicy -> volUpdate));
			sysBusyCnt = READ_DEVICE_VOL;
		}
		else if(tmpDevicePolicy -> curUpdate != 0)
		{
			checkPortNum = (Bit_To_Num(tmpDevicePolicy -> curUpdate));
			sysBusyCnt = READ_DEVICE_CUR;
		}
		else if(tmpDevicePolicy -> protocolUpdate != 0)
		{
			checkPortNum = (Bit_To_Num(tmpDevicePolicy -> protocolUpdate));
			sysBusyCnt = READ_DEVICE_PROTOCOL;
		}
		else if(tmpDevicePolicy ->portUpdate != 0)
		{
			checkPortNum = (Bit_To_Num(tmpDevicePolicy -> portUpdate));
			sysBusyCnt = READ_DEVICE_STATE;
		}
		else
		{
			return;
		}
	}
	tmpFunc = PortToFunc(checkPortNum);
	
	switch(sysBusyCnt)
	{
		case READ_DEVICE_STATE: //获取设备端口状态
				 tmpState = tmpDevicePolicy->newPortDevice[checkPortNum].portPowerChart & 0x01;	
				 Coey_Mode_About_Sleep(tmpDevicePolicy, checkPortNum,  &tmpPortStateMode);		
				 if(SW_Get_Specify_State(checkPortNum, portNumBuf[checkPortNum], &tmpPortStateMode,  &tmpState))
				 {
					 if(port_ic[checkPortNum] != SW_NET_DEV) //网络设备，不更新到本地，已通过网络更新 
					 { 
						 if(tmpState)
						 {
						 	 tmpDevicePolicy->newPortDevice[checkPortNum].portPowerChart |= 0x01;
						 }
						 else
						 {
							 tmpDevicePolicy->newPortDevice[checkPortNum].portPowerChart  = 0; //设备不在线，临时全部清0
						 }
					}	
					tmpDevicePolicy -> portUpdate  &= (~(1 << checkPortNum));//状态读取完成 
					sysBusyCnt = 0;//归还控制权
				 }
				break;
		case READ_DEVICE_VOL:	//获取设备电压
				if(tmpFunc -> SW_Get_Vbus(checkPortNum, &tmpVolCurValue))
				{					
					tmpDevicePolicy ->lastPortDevice[checkPortNum].portVoltage = tmpVolCurValue;
					tmpDevicePolicy -> volUpdate &= (~(1 << checkPortNum));
					sysBusyCnt = 0;//归还控制权
				}
				break;
		case SET_DEVICE_REBROADCAST://更新芯片PDO
				tmpValue = tmpFunc ->SW_Rebroadcast_PDO(checkPortNum, 1, 0); //此处广播方式选为1，3566忽略emarker广播。
				if(tmpValue > 0)
				{
					tmpDevicePolicy -> broadcastUpdate &= (~(1 << checkPortNum));//状态读取完成 
					sysBusyCnt = 0;//归还控制权
				}
				break;
		case SET_DEVICE_POWER://更新功率
				tmpValue = tmpFunc ->SW_Set_System_Power(checkPortNum, tmpDevicePolicy ->lastPortDevice[checkPortNum].portMaxPower);
				if(tmpValue > 0)
				{
					tmpDevicePolicy -> powerUpdate &= (~(1 << checkPortNum));//状态读取完成
					if(port_ic[checkPortNum] != SW_NET_DEV) //非网络设备再进行广播，网络设备在设置功率后设备会自动广播。
					{
						tmpDevicePolicy ->broadcastUpdate |=   (1 << checkPortNum);//功率设置完成，则重新广播
					}
					sysBusyCnt = 0;//归还控制权
				}
				break;
		case READ_DEVICE_TEMPER:	//更新温度				
				SW_Get_Actual_Temp(PORT1, &tmpTemperture, 3950, 100000);
				tmpDevicePolicy ->devTemperture = tmpTemperture;
				tmpDevicePolicy -> tempUpdate = 0;
				sysBusyCnt = 0;//归还控制权
				break;
		case READ_DEVICE_CUR ://更新电流
				if(tmpFunc -> SW_Get_Current(checkPortNum, 0, &tmpVolCurValue))
				{
					tmpDevicePolicy ->lastPortDevice[checkPortNum].portCurrent = tmpVolCurValue;
					tmpDevicePolicy -> curUpdate &= (~(1 << checkPortNum));
					sysBusyCnt = 0;//归还控制权
				}
				break;
		case READ_DEVICE_PROTOCOL ://更新协议
				if(tmpFunc -> SW_Get_Port_Protocol(checkPortNum, &tmpState))
				{
					if(tmpState > 0)
					{
						tmpDevicePolicy ->newPortDevice[checkPortNum].portPowerChart |= (1 << 1);//置位协议标志位
					}
					else
					{
						tmpDevicePolicy ->newPortDevice[checkPortNum].portPowerChart &= 0xFD; //清除协议标志位
					}
					tmpDevicePolicy ->protocolUpdate &= (~(1 << checkPortNum)); //清除对应标志位。
					sysBusyCnt = 0;//归还控制权
				}
				break;
	}
}

void Slave_Update_Chart(device_policy_t  *tmpDevicePolicy)
{
	u8 tmpValue = 0;
	u32 tmpVol  = 0;
	u32 tmpPower = 0;
	u8 tmpVolGear = 0;
	u8 tmpPowerGear = 0;
	
	SW_Func1 -> SW_Get_Port_State(PORT1, 0, &tmpValue);
	
	if(tmpValue != GET_PORT_STATE(tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart))
	{	
		if(tmpValue)
		{
			tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart |= 0x01;
			//tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr + NET_ADDR_OFFSET].portMaxPower = 450;	
			//广播一个小的。
		}
		else
		{
			tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart &= 0xFE;
			tmpDevicePolicy -> cBuckVolInGear[0] = 0;
			//tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr + NET_ADDR_OFFSET].portMaxPower = 0;
		}
	}

	if(GET_PORT_STATE(tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart))
	{
		SW_Func1 -> SW_Get_Vbus(PORT1, &tmpVol);
		tmpVolGear = Get_Vol_Gear(tmpVol);
		
		if(tmpVolGear != GET_VOL_GEAR(tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr ].portPowerChart))
		{
			SET_VOL_GEAR(tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart, tmpVolGear);
		}
		
		SW_Func1 -> SW_Get_Current(PORT1, 0,  &tmpPower);
		
		tmpPower = tmpPower * tmpVol;//电压 * 电流得到实际功率。	
		
		tmpPower /= 100000;//得到功率值的10倍
		
		tmpPowerGear = Power_To_Gear(tmpPower); //此处要求传输实际功率档
		
		if(tmpPowerGear != GET_POWER_GEAR(tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart))
		{
			SET_POWER_GEAR(tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart, tmpPowerGear);
		}
	}
	
	if(tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart != tmpDevicePolicy->lastPortDevice[tmpDevicePolicy->devAddr].portPowerChart)
	{
		tmpDevicePolicy->lastPortDevice[tmpDevicePolicy->devAddr].portPowerChart = tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart;
		//I2c_Slave_Write_TX_Data(&i2c_driver_buffer.sendBuf[1], 0x01);
	}
	
	i2c_driver_buffer.i2cBuf[PORT1_STATE_REG] = tmpDevicePolicy->newPortDevice[tmpDevicePolicy->devAddr].portPowerChart; //更新数据到i2c发送buffer
	i2c_driver_buffer.i2cBuf[REQ_VOL_REG] = tmpDevicePolicy -> cBuckVolInGear[0]; //更新数据到i2c发送buffer
}

#define TIMEMARKMASK 0X7FFFFFFF
volatile u32 SystemTimeCounter = 0;

void ATM_Init_Normal(void)
{
	Rcc_Module_Clk_Enable(ATM_ID);
	Rcc_Module_Rst_Release(ATM_ID);
	//Atm0_Set_Reload_Value(50000);//50000*80=4ms
	//Atm0_Set_Reload_Value(12500);//12500*80ns=1ms
	Atm0_Set_Reload_Value(25000);//25000*80ns=2ms
	Atm0_Int_Enable();
	Nvic_Irq_Enable(ATM0_IRQn, PRIORITY_3);
	Atm0_Enable();	
}

void ATM0_Handler(void)
{
	 Atm0_Clear_Int_Pending();
	 //根据定时器初始化修改累加值
	 SystemTimeCounter += 2;
}

//时间获取函数
u32 My_GetSystemTimePass(u32 tm)
{
	u32 otm = (TIMEMARKMASK & SystemTimeCounter);
	if(otm > tm)
		return (otm-tm);
	else
		return (TIMEMARKMASK&((TIMEMARKMASK & ~tm) + 1 + otm));
}

u32 My_GetSystemTimeMark(void)
{
	return (TIMEMARKMASK & SystemTimeCounter);
}

void My_Time_Delay(u32 tim)
{
	u32 tmpTime = My_GetSystemTimeMark();
	while(My_GetSystemTimePass(tmpTime) < tim)
	{
		//死延时
	}
}	

//GPIO初始化
void Usr_Gpio_Init(void)
{
	gpio_config_t gpioConfig;
	u8 tmpValue = 0;
	
 	Rcc_Module_Clk_Enable(GPIO_ID);
	
	////PFC引脚功能设置
	Gpio_Deinit(ON_OFF1_GPIO);
	gpioConfig .direction = GPIO_DIRECTION_OUTPUT;
	gpioConfig .gpioFunc = (gpio_func_e)ON_OFF1_FUN_SEL;
	gpioConfig .pmosOd   = PMOS_OPDRAIN_DIS;
	gpioConfig .nmosOd   = NMOS_OPDRAIN_DIS;
	gpioConfig .pullup   = PULLUP_EN;
	gpioConfig .pulldown = PULLDOWN_DIS;
	gpioConfig .res = PULLUP_RES_10K;

	Gpio_Init(ON_OFF1_GPIO, &gpioConfig);
	ON_OFF1_0;
}
