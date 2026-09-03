/*
***************************************************************************
* Copyright (C) 2022-2023 Ismartware Limited. All rights reserved. 
* File name: device_policy.c
* Description: functions about ic device to handle different  situation.
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date: 
* History: 
***************************************************************************
*/
#include "power_alloc.h"
#include "device_policy.h" 
#include "device_drive.h" 

#include "sw3566_i2c_lib.h"
#include "power_alloc.h"
#include "zr_adc.h"
#include "device_com.h" 

device_policy_t devicePolicy;

const u8 portToBit[9] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

static u8 Port_State_Policy(device_policy_t *portDevice, uint8_t portNum);
//端口状态处理机制，根据新旧状态对比，来进行决策。
static u8 Port_State_Policy(device_policy_t *portDevice, uint8_t portNum)
{
	uint8_t newState  = GET_PORT_STATE(portDevice ->newPortDevice[portNum].portPowerChart);
	uint8_t lastState = GET_PORT_STATE(portDevice ->lastPortDevice[portNum].portPowerChart);
	
	if(newState != lastState)
	{
		if(newState == 0)
		{
			portDevice ->lastPortDevice[portNum].portStateChange = 2;//端口移除
			portDevice ->lastPortDevice[portNum].portPowerChart &= 0xFE;
			portDevice ->lastPortDevice[portNum].portMaxPower   = 0;
			portDevice -> cBuckVolInGear[0] = 0;
			
			portDevice ->dlyToUpdatePower = 1;//有设备拔出，则置位延时功率更新。
			portDevice ->portOutTime  =    My_GetSystemTimeMark();//记下此时的时间
		
			portDevice ->cBuckVolInGear[portNum] = 0;//设备拔出后，要清0输入电压档位。
			
			return 1;
		}
		else
		{
			portDevice ->lastPortDevice[portNum].portStateChange = 1;//新增端口
			portDevice ->lastPortDevice[portNum].portPowerChart |= 0x01;
			
			portDevice ->dlyToUpdatePower = 0;//有设备插入，则清零延时功率更新。
			
			return 2;
		}
	}
	else
	{
		portDevice ->lastPortDevice[portNum].portStateChange = 0;//未发生改变
	}
	
	return 0;
}

//把当前在线的设备状态，转变为字节，需要根据要求修改，请屏蔽掉网络设备数量。
static uint8_t toolStateToBytes( port_device_t  *portChart)
{
	u8 i = 0;
	u8 tmpValue = 0;
	const u8 port_ic[MODULE_NUM] = {PORT1_IC, PORT2_IC, PORT3_IC, PORT4_IC, PORT5_IC};
	u8  constWei =  (1 << (PORT_SUM - 1));
	port_device_t  *tmpPortChart;
	
	tmpPortChart = portChart;
	
	for(i = 0; i < PORT_SUM; i++)
	{
		tmpValue >>= 1;
		
		if(port_ic[i] != SW_NET_DEV) //网络设备不参与此数据采集。
		{
			if(GET_PORT_STATE(tmpPortChart -> portPowerChart) > 0)
			{
				tmpValue |= constWei;
			}
		}
		
		(void)*tmpPortChart ++;
	}	
	return tmpValue;
}

//设备策略，主进程，发布任务，检查任务，进程控制。
void Device_Policy(device_policy_t *portDevice)
{
	const u8 port_ic[MODULE_NUM] = {PORT1_IC, PORT2_IC, PORT3_IC, PORT4_IC, PORT5_IC};
	static uint8_t sysProcess = 0;
	static uint8_t portStateBuf[5];
	static uint8_t tempGear = 0;
	static uint8_t tempChangeFlag = 0;
	static uint32_t tempChangeTime = 0;
	static uint32_t dlyTime = 0;
		   uint8_t i = 0;
	
	switch(sysProcess)
	{
		case SYS_POLICY_INIT:
							portDevice -> portUpdate = portToBit[PORT_SUM];
							sysProcess = PORT_STATE_POLICY;
							portDevice->notSleepFlag = 1;
							break;
		case PORT_STATE_POLICY:		//状态检测
							if(portDevice -> portUpdate == 0) //完成一轮的检测
							{		
								//有设备在线时，进行更新。无设备时，只检测端口状态。
								if((Calc_Online_Num( portDevice -> lastPortDevice) > 0) || (Calc_Online_Num( portDevice -> newPortDevice) > 0)) //此处应为last的数据，因为新数据无设备后，无法进行清除老的状态。
								{
									portDevice -> tempUpdate = 1;//需要更新温度，则置位温度检测标志位。						
									sysProcess = PORT_TEMPER_POLICY; //更新完状态，根据需要是否需要更新温度
									portDevice->notSleepFlag = 1; //每次检测完端口，完成一次睡眠的设置。
									portDevice -> readyToSleep = 0; //准备睡眠标志清0
								}
								else
								{
									sysProcess = 0;
									portDevice -> notSleepFlag = 0; //无设备在线，则开始睡眠。
								}
							}		
							break;
		case PORT_VOL_POLICY:	//检测电压，根据需要来添加
							if(portDevice ->volUpdate == 0) //电压是否更新完成
							{
									portDevice ->curUpdate = toolStateToBytes( portDevice -> newPortDevice);
									sysProcess = PORT_CUR_POLICY;//准备更新电流
							}
							break;
		case PORT_TEMPER_POLICY: //检测温度，根据需要添加
							if(portDevice -> tempUpdate == 0)
							{
									sysProcess = SYS_POLICY_ANA;
							}
							break;
		case SYS_POLICY_ANA:		//PDO是否更新决策	
							//初始化更新PDO
							devicePolicy.isNeedPowerPolicy = 0;
							//根据端口来决定是否更新PDO
							for(i = 0; i < PORT_SUM; i++)
							{
								if(Port_State_Policy(portDevice, i) > 0)
								{
									//任何口的变化都会触发更新功率位。
									devicePolicy.isNeedPowerPolicy = 1;
								}
							}
							//根据温度来决定是否更新PDO
							NTC_To_Gear_Handle(portDevice ->devTemperture, &tempGear);
							if(portDevice ->powerGear != tempGear)
							{
								portDevice ->powerGear = tempGear; //5秒连续的温度改变才会更新温度档。
								devicePolicy.isNeedPowerPolicy = 1;//需要更新温度。
							}
							if(devicePolicy.isNeedPowerPolicy)
							{
								sysProcess = PORT_POWER_POLICY;//准备更新PDO
							}
							else
							{
								sysProcess = SYS_POLICY_INIT;//无需更新，重回检测
							}

							break;
		case PORT_POWER_POLICY://等待阶段，是否完成状态的判断和功率的分配。
							//完成则根据需要是否需要更新PDO，需要的话就置位设备的重置PDO。
							if(devicePolicy.isNeedPowerPolicy == 0) //等待功率策略层处理完成，
							{
								if(portDevice ->powerUpdate == 0) //处理完成后，等待硬件完成功率的更新
								{
									if(portDevice ->broadcastUpdate == 0)//等待广播完成后，完成一轮。
									{
										portDevice -> portUpdate = portToBit[PORT_SUM];
										sysProcess = SYS_POLICY_INIT;//重新进行状态检测												
									}
								}
							}
							break;
		case PORT_CUR_POLICY:	//检测电流，根据需要添加
							if(portDevice ->curUpdate == 0) //电流是否更新完成
							{
								sysProcess = SYS_POLICY_ANA;//主要数据采集完成，跳到进程3
							}
							break;//检测电流
		case PORT_PROTOCOL_POLICY://检测协议，根据需要添加
							if(portDevice -> protocolUpdate == 0) //协议检测是否为0
							{
								portDevice -> volUpdate = toolStateToBytes( portDevice -> newPortDevice);;
								sysProcess =  PORT_VOL_POLICY;
							}
							break;
	}
}

void Power_Policy(device_policy_t *portDevice)
{
	u8 i = 0;
	u32 tmpPower = 0;
	if(devicePolicy.isNeedPowerPolicy)
	{		
		if(portDevice -> devRole == MASTER_DEV) //主机的功率处理策略
		{
			for(i = 0; i < PORT_SUM; i++)
			{
				if(GET_PORT_STATE(portDevice ->newPortDevice[i].portPowerChart))
				{
					tmpPower = Easy_Handle_Ask(i + 1, portDevice -> newPortDevice, portDevice ->powerGear);
					
					portDevice -> newPortDevice[i].portMaxPower = tmpPower;
					
					if(tmpPower != (portDevice -> lastPortDevice[i].portMaxPower))
					{
						portDevice -> lastPortDevice[i].portMaxPower = tmpPower;
						portDevice -> powerUpdate |= (1 << i); 
					}
				}
			}
		}
		else  //从机的功率处理策略
		{
			if(GET_PORT_STATE(portDevice ->newPortDevice[portDevice ->devAddr].portPowerChart))
			{
				tmpPower = portDevice -> newPortDevice[portDevice ->devAddr].portMaxPower;
				
				if(tmpPower != (portDevice -> lastPortDevice[portDevice ->devAddr].portMaxPower))
				{
					portDevice -> lastPortDevice[portDevice ->devAddr].portMaxPower = tmpPower;
					SW_Func1 ->SW_Set_System_Power(PORT1,tmpPower);
					SW_Func1 ->SW_Rebroadcast_PDO(PORT1, 0, 0);
				}
			}
		}
		devicePolicy.isNeedPowerPolicy = 0;
	}
}

//睡眠策略
void Sleep_Policy(device_policy_t *portDevice)
{
	//从机深睡，主机定时唤醒
	if(portDevice -> devRole == SLAVE_DEV)
	{	
		if(portDevice ->slaveWakeFlag)
		{
			if(I2c_Wake_Up_Master() > 0)
			{
				portDevice ->slaveWakeFlag = 0;
			}
		}
		if(portDevice ->wakeFlag == 1)
		{
			portDevice ->wakeFlag = 0;
			portDevice -> wakeTime = My_GetSystemTimeMark();
			portDevice ->readyToSleep = 1;
		}
		if(portDevice ->readyToSleep > 0)
		{
			if(My_GetSystemTimePass(portDevice -> wakeTime) > 50)
			{
				portDevice ->readyToSleep = 0;
				portDevice ->notSleepFlag = 0;
			}
		}
	}
			
	//有定时任务不睡觉
	if(devicePolicy.notSleepFlag > 0) 
	{
		Low_Power_Not_Sleep(); 	
	}	
}

//获取SW3566主从机地址
static void Slave_Addr_Identify(device_policy_t *portDevice)
{
	//ADC数据缓存
	u32 tmpBuf[10];
	u32 tmpA = 0;
	u32 tmpValue = 0;
	for(int i = 0; i < 10; i ++)
	{
		//循环获取ADC采用数据
		tmpBuf[i] = Adc_Get_Channel_Data(ADC_CHL_3);
		for(tmpA = 0; tmpA < 10000;tmpA ++)
		{
			//模拟获取
		}
	}
	//冒泡排序
	for(int i = 0 ; i < 10 ; i++)
	{
		for(int j = 0 ; j <9 - i ; j++ )
		{
		  if(tmpBuf[j] > tmpBuf[j+1])
			{
				 tmpValue = tmpBuf[j];
				 tmpBuf[j] = tmpBuf[j+1];
				 tmpBuf[j+1] = tmpValue;                   
			}
		}
	}
	//换算成电阻值
	tmpValue = tmpBuf[5] * 50;
	//根据电阻值判断从机状态
	if(tmpValue < 30000)
	{
		portDevice->devAddr = 1;
	}	
	else if(tmpValue < 60000)
	{
		portDevice->devAddr = 2;
	}
	else if(tmpValue < 90000)
	{
		portDevice->devAddr = 3;
	}
	else if(tmpValue < 120000)
	{
		portDevice->devAddr = 4;
	}		
	else
	{
		portDevice->devAddr = 0;
	}	
	//使用完关闭，提升芯片效率
	Adc_Stop_Adc_Channel(ADC_CHL_3);
}

//初始化ADC，主从机区分初始化（VD脚初始化）
void SW3566_Master_Slave_Identify_ADCInit()
{
	Adc_Enabled();
	adc_config_t adcCfg;
	//Gpio_Function_Select(GPIO2, GPIO2_FUNC_SEL_ADC);
	Adc_Get_Config(&adcCfg);
	adcCfg.adc3Pullup = ADC_PULLUP_CURRENT_20UA;//100KΩ（≈1.9V-2V）
	adcCfg.adc3Source = ADC3_SIG_SRC_VD,//singal source should equal channel
	Adc_Set_Config(&adcCfg);
	Adc_Start_Adc_Channel(ADC_CHL_3);//channel should equal singal source		
}

//主机从机初始化
void MasterSlaveIdentify(device_policy_t *portDevice)
{
	u8 tmpValue = 0;
	u8 i2cAddr = 0;
	//初始化SW3566主从机区分ADC初始化
	SW3566_Master_Slave_Identify_ADCInit();		
	//从机需要确认地址	
	Slave_Addr_Identify(portDevice);
	//主从机分别初始化
	if(portDevice->devAddr == 0)
	{
		//确认SW3566主机身份
		portDevice ->devRole = MASTER_DEV;
		//主机IIC初始化
		I2C_Drive_Init(0x3C);
		//SW3566主机控制芯片初始化
		IC_Device_Init(portDevice);
		//设置默认功率
		Cfg_Sys_Pdo(1400);
		//GPIO功能初始化
		//Usr_Gpio_Init();
	}
	else
	{
		portDevice ->devRole = SLAVE_DEV;
		I2C_Drive_Init(I2C_ADDR_BASE); //从机做主机地址是主机的地址0x37

	}
	
	I2c_Slave_Drive_Init(icAddr[portDevice->devAddr]);
}

//GPIO相关操作
void Gpio_Policy(device_policy_t *portDevice)
{
	u32 tmpData = 0;
	u8  tmpGear = 0;
	
	tmpGear = portDevice -> cBuckVolInGear[0];
	
	if(portDevice -> cBuckVolInGear[1] >= portDevice -> cBuckVolInGear[0])
	{
		tmpGear = portDevice -> cBuckVolInGear[1];
	}
	
	if(portDevice -> cBuckVolInGear[2] > tmpGear)
	{
		tmpGear = portDevice -> cBuckVolInGear[2];
	}		
	 
	if(tmpGear != portDevice -> sysBuckVolInGear)
	{
		portDevice -> sysBuckVolInGear	= tmpGear;
		switch(portDevice -> sysBuckVolInGear)
		{
			case 0:
					ON_OFF1_0;
					break;
			case 1:
					ON_OFF1_1;
					break;

		}
	}	
}
