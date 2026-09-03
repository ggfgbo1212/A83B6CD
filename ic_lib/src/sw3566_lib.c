#include "sw3566_func.h"
#include "user_define.h"
#include "power_alloc.h"

extern const struct sw_i2c_drive sw_port_i2c_drive[4];
extern const unsigned char icAddr[4];

static u32 SW3566_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW3566_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);
static u32 SW3566_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 
static u32 SW3566_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW3566_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW3566_On_Off_QC(u8 port,u8 version,u8 setState);
static u32 SW3566_Set_FCP_XSCP(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
static u32 SW3566_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState);
static u32 SW3566_Rebroadcast_PDO(u8 port, u8 way, u32 time);//
static u32 SW3566_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW3566_Set_PE(u8 port, u32 maxVol);
static u32 SW3566_On_Off_PE(u8 port, u8 setState);
static u32 SW3566_Set_Line_Com (u8 port, u8 setState,  u32 setValue);//
static u32 SW3566_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW3566_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW3566_Set_NTC_Current(u8 port, u32 setValue);//
static u32 SW3566_Set_DAC(u8 port, u32 setValue);//
static u32 SW3566_Get_Vin(u8 port, u32 *vinVol);
static u32 SW3566_Get_Vbus(u8 port, u32 *vbusVol);
static u32 SW3566_Get_Current(u8 port, u8 num,u32 *current);
static u32 SW3566_Get_NTC_Temperature(u8 port, int *temperature);//
static u32 SW3566_Get_Port_State(u8 port, u8 num, u8  *portState);
static u32 SW3566_Get_Sys_Power(u8 port,u32 *sysPower);
static u32 SW3566_Get_Port_Protocol(u8 port,  u8 *protocol);
static u32 SW3566_On_Off_Emarker(u8 port, u8 setState);
static u32 SW3566_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW3566_On_Off_Buck(u8 port, u8 setState);
static u32 SW3566_I2C_Unlock(u8 port);
static u32 SW3566_Unlock_Force (u8 port);
static u32 SW3566_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW3566_Set_Port_Mode(u8 port, u8 mode);

static u32 SW3566_Init(u8 port);
static u32 SW3566_isExist(u8 port, u8 *existValue);
static u32 SW3566_Set_System_Power(u8 port, u16 power);
static u32 SW3566_User_Define(u8 port, u8 funcNo, u32 *dat);

const struct sw_func SW3566_Func = 
{
	.SW_Init = SW3566_Init,	
	.SW_Rebroadcast_PDO = SW3566_Rebroadcast_PDO,

	.SW_Set_System_Power  = SW3566_Set_System_Power,
	.SW_User_Define = SW3566_User_Define,
	
//	.SW_Set_Line_Com = SW3566_Set_Line_Com,
//	.SW_Set_Current_Limit = SW3566_Set_Current_Limit,
//	.SW_Set_Unload_Current = SW3566_Set_Unload_Current,
//	.SW_Set_NTC_Current = SW3566_Set_NTC_Current,
//	.SW_Set_DAC = SW3566_Set_DAC,
	
	.SW_Get_Vin = SW3566_Get_Vin,
	.SW_Get_Vbus = SW3566_Get_Vbus,
	.SW_Get_Current = SW3566_Get_Current, 
	.SW_Get_NTC_Temperature = SW3566_Get_NTC_Temperature,
	.SW_Get_Port_State = SW3566_Get_Port_State,
	.SW_Get_Sys_Power = SW3566_Get_Sys_Power,	
	.SW_Get_Port_Protocol = SW3566_Get_Port_Protocol,
	
//	.SW_On_Off_Emarker = SW3566_On_Off_Emarker,
//	.SW_Dpdm_Power_Same_PD = SW3566_Dpdm_Power_Same_PD,
//	.SW_On_Off_Buck = SW3566_On_Off_Buck,
//	.SW_I2C_Unlock = SW3566_I2C_Unlock,
//	.SW_Unlock_Force = SW3566_Unlock_Force,
//	.SW_Unlock_High_Reg = SW3566_Unlock_High_Reg, 
//	.SW_Set_Port_Mode = SW3566_Set_Port_Mode,
//	.SW_Set_PD_Gear = SW3566_Set_PD_Gear, 
//	.SW_Set_PD_Vol_Cur = SW3566_Set_PD_Vol_Cur,	
//   
//	.SW_Set_UFCS_Gear = SW3566_Set_UFCS_Gear,	
	
//	.SW_Set_VID_PID = SW3566_Set_VID_PID,
//	.SW_Set_QC = SW3566_Set_QC,
//	.SW_On_Off_QC = SW3566_On_Off_QC,
	
//	.SW_Set_FCP_XSCP = 	SW3566_Set_FCP_XSCP,
//	.SW_On_Off_FCP_XSCP = SW3566_On_Off_FCP_XSCP,
//	.SW_On_Off_Charge_Protocol = SW3566_On_Off_Charge_Protocol,
//	.SW_Set_PE = SW3566_Set_PE,
//	.SW_On_Off_PE = SW3566_On_Off_PE,
//	.SW_isExist = SW3566_Is_Exist,
};



static u32 SW3566_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
	return 1;
}

static u32 SW3566_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{
	return 1;
}


static u32 SW3566_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
	return 2;
}

static u32 SW3566_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{
	return 1;
}

static u32 SW3566_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower)//QC
{
	return 1;
}

static u32 SW3566_On_Off_QC(u8 port,u8 version,u8 setState)//QC version:1=QC2.0   2=QC3.0
{	
	return 1;
}

static u32 SW3566_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
	return 1;
}

static u32 SW3566_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)//fcp:3  scp:4
{
	return 1;
}

static u32 SW3566_Rebroadcast_PDO(u8 port, u8 way, u32 time)//
{
	//是否开启emark检测
	Pd_Src_Cap_Ignore_Emarker(way);
	//PD重新广播
	Pd_Rebroadcast_Src_Cap();
	return 1;
}

static u32 SW3566_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
	return 2;
}

static u32 SW3566_Set_PE(u8 port, u32 maxVol)
{
	return 1;
}

static u32 SW3566_On_Off_PE(u8 port, u8 setState)
{
	return 1;
}

static u32 SW3566_Set_Line_Com(u8 port,u8 setState, u32 setValue)
{
	return 1;
}

static u32 SW3566_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
	return 1;
}

static u32 SW3566_Set_Unload_Current(u8 port, u8 num, u32 setValue)
{
	return 2;
}

static u32 SW3566_Set_NTC_Current(u8 port, u32 setValue)
{
	
	return 2;
}

static u32 SW3566_Set_DAC(u8 port, u32 setValue)
{
	return 2;
}

static u32 SW3566_Get_Vin(u8 port, u32 *vinVol)
{
	*vinVol = Adc_Vin_Get();
	return 1;
}

static u32 SW3566_Get_Vbus(u8 port, u32 *vbusVol)
{
	*vbusVol = Adc_Vout_Get();
	return 1;
}
	
static u32 SW3566_Get_Current(u8 port, u8 num,u32 *current)
{
	*current = Adc_Iout_Get(num);
	return 1;
}

//此处返回值为电阻值	
static u32 SW3566_Get_NTC_Temperature(u8 port, int *temperature)
{
	u32 tmpData = 0;
	tmpData = Adc_Get_Channel_Data(NTC_ADC_CHANNEL);//user get channel ADC data
	tmpData *= 50;
	*temperature = tmpData;
	return 1;
}

static u32 SW3566_Get_Port_State(u8 port, u8 num, u8  *portState)
{
	u8 tmpValue = 0;
	tmpValue = Device_Get_Port_State();
	*portState = tmpValue;
	return 1;
}
	
static u32 SW3566_Get_Sys_Power(u8 port,u32 *sysPower)
{
	return 1;
}

static u32 SW3566_Get_Port_Protocol(u8 port,  u8 *protocol)
{
	return 1;
}

static u32 SW3566_On_Off_Emarker(u8 port, u8 setState)
{
	return 1;
}

	
static u32 SW3566_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
	return 2;
}

static u32 SW3566_On_Off_Buck(u8 port, u8 setState)
{
	return 1;
}

static u32 SW3566_I2C_Unlock(u8 port)
{
	return 2;
}

static u32 SW3566_Unlock_Force (u8 port)
{
	return 2;
}

static u32 SW3566_Unlock_High_Reg(u8 port, u8 setState)
{
	return 2;
}
	
static u32 SW3566_Set_Port_Mode(u8 port, u8 mode)
{
	return 1;
}

static u32 SW3566_Init(u8 port)
{
	//NTC初始化
//	Adc_Enabled();
//	adc_config_t adcCfg;
//	//Gpio_Function_Select(GPIO2, GPIO2_FUNC_SEL_ADC);
//	Adc_Get_Config(&adcCfg);
//	adcCfg.adc3Pullup = ADC_PULLUP_CURRENT_20UA;//100KΩ（≈1.9V-2V）
//	adcCfg.adc3Source = ADC3_SIG_SRC_CC12,//singal source should equal channel
//	Adc_Set_Config(&adcCfg);
//	Adc_Start_Adc_Channel(ADC_CHL_3);//channel should equal singal source	
	return 1;
}

static u32 SW3566_isExist(u8 port, u8 *existValue)
{
	*existValue = 1;
	return 2;
}

static u32 SW3566_Set_System_Power(u8 port, u16 power)
{
	Cfg_Sys_Pdo(power);
	return 1;
}

static u32 SW3566_User_Define(u8 port, u8 funcNo, u32 *dat)
{	
	return 1;
}

