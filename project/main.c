/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file main.c
 * @brief Main program body
 * @author SD1 & SW team
 ****************************************************************************/
#include "usr_common.h"
#include "api.h"
#include "config.h"
#include "ufcs_caps.h"
#include "pdo.h"
#include "serial.h"
#include "zr_rcc.h"
#include "pd_message.h"
#include "zr_abn.h"

#include "iic_slave_policy.h"
#include "slave_policy_drive.h"
#include "i2c_ota_policy.h"
#include "zr_btm.h"

#include <stdio.h>

//双A口限流设置
#define A_A_NOLOAD_TRIGGER

//双A口自动限流
#define A_A_FIX_DCP



#ifdef A_A_FIX_DCP
u8 port1CurOldData = 0;
u8 port2CurOldData = 0;
u8 appleModleOldData = 0;
u8 fixDcpFlag = 0;
#endif


#ifdef A_A_NOLOAD_TRIGGER
u8 A_NoLoad_temp = 0;
u8 A_NoLoad_Start_Flag = 0;
#endif

extern u8 port1NoLoadCur;
extern u8 port2NoLoadCur;

extern u8 sceneConversionTime;

extern port_mode_e cur_port_mode;

extern u16 dac_vol;
extern u16 oldVol;
extern u16 oldPort1Cur;

extern u8 isIRQEnable;
extern u8 adjustVolCur;
extern u8 adjustVolCurIfExist;
extern u8 reg_new_data[0x100];

extern u8 avsOffsetGearPort1;
extern u8 ppsOffsetGearPort1;

extern u8 avsOffsetGearPort2;
extern u8 ppsOffsetGearPort2;

static bool PowerAdjust(uint16_t* vol, uint16_t* port1Curr, uint16_t* port2Curr)
{
	//vol = *vol * 10/cur = *port1Curr * 25
	//模拟调流调压功能
	dac_vol = *vol;
	if((oldVol != *vol) || (oldPort1Cur != *port1Curr))
	{
		oldVol = *vol;
		oldPort1Cur = *port1Curr;
		if(isIRQEnable == 1 && adjustVolCur == 1 && adjustVolCurIfExist == 0)
		{
			INTERRUPGPIO_0;
			adjustVolCurIfExist = 1;
			WiterRegData(0x5A, (ReadRegData(0x5A) | 0x01));
			//reg_new_data[0x5A] |= 0x01;
		}
	}
	
	if(Device_Get_Protocol(1) == FC_PD_PPS)
	{
		switch(ppsOffsetGearPort1)
		{
			case 0:
				*port1Curr = *port1Curr;
				break;
			case 1:
				*port1Curr = *port1Curr + 4;//100ma 4*25mA
				break;
			case 2:
				*port1Curr = *port1Curr + 8;//200ma
				break;
			case 3:
				*port1Curr = *port1Curr + 12;//300ma
				break;
			case 4:
				*port1Curr = *port1Curr + 16;//400ma
				break;
			case 5:
				*port1Curr = *port1Curr + 20;//500ma
				break;
			default:
				*port1Curr = *port1Curr;
				break;
		}
		return true;
	}
	else if(Device_Get_Protocol(1) == FC_PD_AVS)
	{
		switch(avsOffsetGearPort1)
		{
			case 0:
				*port1Curr = *port1Curr;
				break;
			case 1:
				*port1Curr = *port1Curr + 4;//100ma
				break;
			case 2:
				*port1Curr = *port1Curr + 8;//200ma
				break;
			case 3:
				*port1Curr = *port1Curr + 12;//300ma
				break;
			case 4:
				*port1Curr = *port1Curr + 16;//400ma
				break;
			case 5:
				*port1Curr = *port1Curr + 20;//500ma
				break;
			default:
				*port1Curr = *port1Curr;
				break;
		}
		return true;		
	}
	
	if(Device_Get_Protocol(2) == FC_PD_PPS)
	{
		switch(ppsOffsetGearPort2)
		{
			case 0:
				*port2Curr = *port2Curr;
				break;
			case 1:
				*port2Curr = *port2Curr + 4;//100ma
				break;
			case 2:
				*port2Curr = *port2Curr + 8;//200ma
				break;
			case 3:
				*port2Curr = *port2Curr + 12;//300ma
				break;
			case 4:
				*port2Curr = *port2Curr + 16;//400ma
				break;
			case 5:
				*port2Curr = *port2Curr + 20;//500ma
				break;
			default:
				*port2Curr = *port2Curr;
				break;
		}
		return true;			
	}
	else if(Device_Get_Protocol(2) == FC_PD_AVS)
	{
		switch(avsOffsetGearPort2)
		{
			case 0:
				*port2Curr = *port2Curr;
				break;
			case 1:
				*port2Curr = *port2Curr + 4;//100ma
				break;
			case 2:
				*port2Curr = *port2Curr + 8;//200ma
				break;
			case 3:
				*port2Curr = *port2Curr + 12;//300ma
				break;
			case 4:
				*port2Curr = *port2Curr + 16;//400ma
				break;
			case 5:
				*port2Curr = *port2Curr + 20;//500ma
				break;
			default:
				*port2Curr = *port2Curr;
				break;
		}
		return true;			
	}


	
//	//LPS功能
//	abn_config_t abnCfg = {0};
//	Abn_Get_Config(&abnCfg);
//	//3.3V-15V(不包含15V)
//	if((*vol * 10) < 15000)
//	{
//		//LPS异常上管电流比较门限
//		abnCfg.bitFields.lpsHmosCurrTh = LPS_ADC_CMP_TH_3P96A_5P00A;
//		//LPS ADC比较门限
//		abnCfg.bitFields.lpsAdcCurrTh = LPS_ADC_CMP_TH_2P2A;
//		//OCP保护门限
//		Device_Set_Vr_Ocp(VR_OCP_15A);
//		//HSMOS电阻值
//		Device_Cfg_Mos_Resistance(MOS_RES_8MOHM);	
//		//abn设置
//		Abn_Set_Config(&abnCfg);
//	}
//	//15V-24V(不包含24V)
//	else if((*vol * 10) < 24000)
//	{
//		//LPS异常上管电流比较门限
//		abnCfg.bitFields.lpsHmosCurrTh = LPS_ADC_CMP_TH_4P52A_5P67A;
//		//LPS ADC比较门限
//		abnCfg.bitFields.lpsAdcCurrTh = LPS_ADC_CMP_TH_2P2A;	
//		//OCP保护门限
//		Device_Set_Vr_Ocp(VR_OCP_8A);
//		//HSMOS电阻值
//		Device_Cfg_Mos_Resistance(MOS_RES_10MOHM);
//		//abn设置
//		Abn_Set_Config(&abnCfg);		
//	}
//	//24V-28V
//	else
//	{
//		//LPS异常上管电流比较门限
//		abnCfg.bitFields.lpsHmosCurrTh = LPS_ADC_CMP_TH_4P52A_5P67A;
//		//LPS ADC比较门限
//		abnCfg.bitFields.lpsAdcCurrTh = LPS_ADC_CMP_TH_2P2A;	
//		//OCP保护门限
//		Device_Set_Vr_Ocp(VR_OCP_6A);
//		//HSMOS电阻值
//		Device_Cfg_Mos_Resistance(MOS_RES_13MOHM);	
//		//abn设置
//		Abn_Set_Config(&abnCfg);		
//	}
	return false;
}


#if defined(A_A_NOLOAD_TRIGGER)

static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad);

static void A_NoLoad_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline);

static void Start_Noload_Timer(void);
static void Stop_Noload_Timer(void);
static uint32_t gNoLoadCount = 0;
static bool gNoLoadChecking = false;
static bool gNoLoadSpecHandle = false;

typedef enum 
{
    NOLOAD_STEP_UNKNOWN,
    NOLOAD_STEP_A_A,
    NOLOAD_STEP_A_X,
} noload_step_e;
static noload_step_e gNoLoadStep = NOLOAD_STEP_UNKNOWN;

#endif

/**
 * @brief  it is a hook function, called only once at typec attach
           it mean when attach event happen,this function will be called, only once
 * @param portIndex,which port's typec attach, 0:port1  1:port2
 */
static void User_Typec_Attach_Hook(u8 portIndex)
{
/**
	modify code according to the actual situation
 */
}

/**
 * @brief  it is a hook function, called only once at typec detach
           it mean when detach event happen,this function will be called, only once
 * @param portIndex,which port's typec attach, 0:port1  1:port2
 */
static void User_Typec_Detach_Hook(u8 portIndex)
{
		config_data_t* config = Config_Get();
		config->isTypecVconnEnabled = true;
		Config_Apply();
		//Pd_Src_Cap_Ignore_Emarker(false);	
}

/**
 * @brief  rx pd request message information.it is a hook function, called only once at rx request message
           it mean as long as rx valid request message,this function will be called, only once
 * @param objIndex,which object data in src cap
 * @param volInMv,request vol in mv
 * @param currInMa,request current in ma
 */
extern u16 pd_request_vol;
extern u16 pd_request_cur;

static void User_Pd_Request_Info_Hook(u8 objIndex, u16 volInMv, u16 currInMa)
{
		if(pd_request_vol != volInMv || pd_request_cur != currInMa)
		{
				pd_request_vol = volInMv;
				pd_request_cur = currInMa;
		}
	
/**
	modify code according to the actual situation
 */
}

int main(void)
{
    /// device init shall be called firstly
    Device_Init();
		//从机初始化
		Slave_Config_Init();

	Typec_Init_Hook_Func(User_Typec_Attach_Hook, User_Typec_Detach_Hook);
	Pd_Init_Hook_Func(User_Pd_Request_Info_Hook, User_Pd_Policy_Hook);
#if defined(A_A_NOLOAD_TRIGGER)
    Register_Noload_Trigger_Callback(A_C_Noload_Trigger_Callback);

    Register_Noload_State_Customize_Callback(A_NoLoad_Customize_Callback);
#endif	


		//功率最终值
		Device_Register_Power_Adjust_Hook(PowerAdjust);
	
		//0x81-bit0寄存器buck强制关闭
		Abn_Force_Ctrl(true);//close buck
		//0x17-bit3-0不驱动端口CC
		Port_Force_Disconnect(0x00);//Port1
		Port_Force_Disconnect(0x01);//Port2	
		
    while (1)
    {
        // the following sequence shall NOT change
        Device_Run();

				#if defined(A_A_NOLOAD_TRIGGER)
						if(cur_port_mode == DUO_AA_MODE)
						{
								uint8_t tempPortState = Device_Get_Port_State();
								if (tempPortState == 0 && gNoLoadSpecHandle)
								{
										gNoLoadChecking = false;
										gNoLoadCount = 0;
										gNoLoadSpecHandle = false;
										Stop_Noload_Timer();
										printf("stop no load checking\n");
								}							
						}
				#endif
												
				#ifdef A_A_FIX_DCP
						//判断当前处于双A口模式
						if(cur_port_mode == DUO_AA_MODE)
						{
							//判断系统功率是否小于等于15W
							if(ReadRegData(0x02) <= 15)
							{
								//当前标志是否置位
								if(fixDcpFlag == 0)
								{
									fixDcpFlag = 1;
									u8 regData0x42 = ReadRegData(0x42);
									u8 regData0x43 = ReadRegData(0x43); 
									u8 regData0x48 = ReadRegData(0x48);
									//获取Port1电流值
									port1CurOldData = regData0x42 & 0x03;
									//获取Port2电流值
									port2CurOldData = regData0x43 & 0x30;
									//当前苹果模式
									appleModleOldData = regData0x48 & 0x40;
									regData0x42 = ((regData0x42 & 0xFC) | 0x00);
									regData0x43 = ((regData0x43 & 0xCF) | 0x00);
									regData0x48 = ((regData0x43 & 0xBF) | 0x00);
									//修改寄存器的值
									WiterRegData(0x42,regData0x42);
									WiterRegData(0x43,regData0x43);
									WiterRegData(0x48,regData0x48);
									//测试实现功能
									Write_Fifo(0x42);
									Write_Fifo(0x43);
									Write_Fifo(0x48);
								}
							}
							else
							{
								//大于15W
								if(fixDcpFlag == 1)
								{
									fixDcpFlag = 0;
									u8 regData0x42 = ReadRegData(0x42);
									u8 regData0x43 = ReadRegData(0x43); 
									u8 regData0x48 = ReadRegData(0x48);
									regData0x42 = ((regData0x42 & 0xFC) | port1CurOldData);
									regData0x43 = ((regData0x43 & 0xCF) | port2CurOldData);
									regData0x48 = ((regData0x43 & 0xBF) | appleModleOldData);		
									//修改寄存器的值
									WiterRegData(0x42,regData0x42);
									WiterRegData(0x43,regData0x43);
									WiterRegData(0x48,regData0x48);
									//测试实现功能
									Write_Fifo(0x42);
									Write_Fifo(0x43);
									Write_Fifo(0x48);									
								}								
							}
						}
				#endif
						

						
				
				CheckAbnSCPOrUVP();
				Slave_Policy();
				//OTA: honours a served OTA ACK by resetting into the vendor
				//bootloader. Returns immediately unless one was requested.
				I2c_Ota_Policy_Run();
				// device policy process, secondary developmen
				Sys_Sleep_Policy();
    }
}




#if defined(A_A_NOLOAD_TRIGGER)
static void A_C_Noload_Trigger_Callback(bool* isDisableNoLoad)
{
    if((cur_port_mode == DUO_AA_MODE) && (3 == Device_Get_Port_State()))
    {
        *isDisableNoLoad = true;
        //printf("no load: disable auto checking, manual checking start, port state = %d\n", Device_Get_Port_State());
        gNoLoadChecking = true;
        gNoLoadCount = 0;
        Start_Noload_Timer();
        gNoLoadStep = NOLOAD_STEP_A_A;
    }
}

#define NOLOAD_CHECKING_INTERVAL (200)
//#define NOLOAD_MAX_COUNT (5*60*1000/NOLOAD_CHECKING_INTERVAL)
//#define IOUT_NOLOAD_TH (15)

static void A_NoLoad_Customize_Callback(bool* isPort1Offline, bool* isPort2Offline)
{
	uint32_t noloadMaxCount = (sceneConversionTime * 60 * 1000 / NOLOAD_CHECKING_INTERVAL);
	if(cur_port_mode == DUO_AA_MODE)
	{
		if(gNoLoadChecking)
		{
			if (gNoLoadStep == NOLOAD_STEP_A_A)
			{
				if (gNoLoadCount >= noloadMaxCount)
				{
					Stop_Noload_Timer();
					if(A_NoLoad_temp == 0)
					{
						*isPort1Offline = true;
					}
					else 
					{
						*isPort2Offline = true;
					}
					A_NoLoad_Start_Flag = 0;
					gNoLoadChecking = false;
					gNoLoadSpecHandle = true;
					gNoLoadCount = 0;
					gNoLoadStep = NOLOAD_STEP_A_X;
				}
			}
		}
		else
		{
			if(A_NoLoad_temp == 0)
			{
				if (2 == Device_Get_Port_State() && gNoLoadStep == NOLOAD_STEP_A_X)
				{
					*isPort1Offline = true ;
				}
			}
			else 
			{
				if (1 == Device_Get_Port_State() && gNoLoadStep == NOLOAD_STEP_A_X)
				{
					*isPort2Offline = true ;
				}
			}
		}		
	}
}

void BTM4_Handler(void)
{
	Low_Power_Not_Sleep();
	if(Btm4_Get_Int_Pending())
	{
		Btm4_Clear_Int_Pending();
		if(cur_port_mode == DUO_AA_MODE)
		{
			if(A_NoLoad_Start_Flag == 0)
			{
				if(Adc_Iout_Get(0) < port1NoLoadCur)
				{
					A_NoLoad_temp = 0;//Port1
					A_NoLoad_Start_Flag = 1;
				}
				else if(Adc_Iout_Get(1) < port2NoLoadCur)
				{
					A_NoLoad_temp = 1;//Port2
					A_NoLoad_Start_Flag = 1;
				}
			}
			uint16_t temp = Adc_Iout_Get(A_NoLoad_temp);
			
			uint8_t curTemp = port1NoLoadCur;
			if(A_NoLoad_temp == 0)
			{
				curTemp = port1NoLoadCur;
			}
			else
			{
				curTemp = port2NoLoadCur;
			}
			
			if (temp < curTemp)
			{
				gNoLoadCount++;
				//printf("iout = %d, count:%d\n", temp, gNoLoadCount);
			}
			else
			{
				gNoLoadCount = 0;
				A_NoLoad_Start_Flag = 0;
			}
		}
	}
}

static void Start_Noload_Timer(void)
{
	if(cur_port_mode == DUO_AA_MODE)
	{
		Btm4_Set_Reload_Value(NOLOAD_CHECKING_INTERVAL*1000/8 - 1);
		Nvic_Irq_Enable(BTM4_IRQn,PRIORITY_2);
		Btm4_Int_Enable();
		Btm4_Enable();   		
	}
}

static void Stop_Noload_Timer(void)
{
	if(cur_port_mode == DUO_AA_MODE)
	{	
		Nvic_Irq_Disable(BTM4_IRQn);
		Btm4_Int_Disable();
		Btm4_Disable();  
	}		
}

#endif




