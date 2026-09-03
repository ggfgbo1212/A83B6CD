#include "sw_net_func.h"
#include "user_define.h"

static u32 SW_NET_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW_NET_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);
static u32 SW_NET_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 
static u32 SW_NET_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW_NET_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW_NET_On_Off_QC(u8 port,u8 version,u8 setState);
static u32 SW_NET_Set_FCP_XSCP(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
static u32 SW_NET_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState);
static u32 SW_NET_Rebroadcast_PDO(u8 port, u8 way, u32 time);//
static u32 SW_NET_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW_NET_Set_PE(u8 port, u32 maxVol);
static u32 SW_NET_On_Off_PE(u8 port, u8 setState);
static u32 SW_NET_Set_Line_Com (u8 port, u8 setState,  u32 setValue);//
static u32 SW_NET_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW_NET_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW_NET_Set_NTC_Current(u8 port, u32 setValue);//
static u32 SW_NET_Set_DAC(u8 port, u32 setValue);//
static u32 SW_NET_Get_Vin(u8 port, u32 *vinVol);
static u32 SW_NET_Get_Vbus(u8 port, u32 *vbusVol);
static u32 SW_NET_Get_Current(u8 port, u8 num,u32 *current);
static u32 SW_NET_Get_NTC_Temperature(u8 port, int *temperature);//
static u32 SW_NET_Get_Port_State(u8 port, u8 num, u8  *portState);
static u32 SW_NET_Get_Sys_Power(u8 port,u32 *sysPower);
static u32 SW_NET_Get_Port_Protocol(u8 port,  u8 *protocol);
static u32 SW_NET_On_Off_Emarker(u8 port, u8 setState);
static u32 SW_NET_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW_NET_On_Off_Buck(u8 port, u8 setState);
static u32 SW_NET_I2C_Unlock(u8 port);
static u32 SW_NET_Unlock_Force (u8 port);
static u32 SW_NET_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW_NET_Set_Port_Mode(u8 port, u8 mode);

static u32 SW_NET_Init(u8 port);
static u32 SW_NET_isExist(u8 port, u8 *existValue);
static u32 SW_NET_Set_System_Power(u8 port, u16 power);
static u32 SW_NET_User_Define(u8 port, u8 funcNo, u32 *dat);

const struct sw_func SW_Net_Func = 
{
	.SW_Get_Port_State = SW_NET_Get_Port_State,
	.SW_Set_System_Power  = SW_NET_Set_System_Power,
	
//	.SW_Set_PD_Gear = SW_NET_Set_PD_Gear, 
//	.SW_Set_PD_Vol_Cur = SW_NET_Set_PD_Vol_Cur,	
//   
//	.SW_Set_UFCS_Gear = SW_NET_Set_UFCS_Gear,	
	
//	.SW_Set_VID_PID = SW_NET_Set_VID_PID,
//	.SW_Set_QC = SW_NET_Set_QC,
//	.SW_On_Off_QC = SW_NET_On_Off_QC,
	
//	.SW_Set_FCP_XSCP = 	SW_NET_Set_FCP_XSCP,
//	.SW_On_Off_FCP_XSCP = SW_NET_On_Off_FCP_XSCP,
	
//	.SW_Rebroadcast_PDO = SW_NET_Rebroadcast_PDO,
//	.SW_On_Off_Charge_Protocol = SW_NET_On_Off_Charge_Protocol,
//	.SW_Set_PE = SW_NET_Set_PE,
//	.SW_On_Off_PE = SW_NET_On_Off_PE,
//	
//	.SW_Set_Line_Com = SW_NET_Set_Line_Com,
//	.SW_Set_Current_Limit = SW_NET_Set_Current_Limit,
//	.SW_Set_Unload_Current = SW_NET_Set_Unload_Current,
//	.SW_Set_NTC_Current = SW_NET_Set_NTC_Current,
//	.SW_Set_DAC = SW_NET_Set_DAC,
	
//	.SW_Get_Vin = SW_NET_Get_Vin,
//	.SW_Get_Vbus = SW_NET_Get_Vbus,
//	.SW_Get_Current = SW_NET_Get_Current, 
//	.SW_Get_NTC_Temperature = SW_NET_Get_NTC_Temperature,

//	.SW_Get_Sys_Power = SW_NET_Get_Sys_Power,	
//	.SW_Get_Port_Protocol = SW_NET_Get_Port_Protocol,
	
//	.SW_On_Off_Emarker = SW_NET_On_Off_Emarker,
//	.SW_Dpdm_Power_Same_PD = SW_NET_Dpdm_Power_Same_PD,
//	.SW_On_Off_Buck = SW_NET_On_Off_Buck,
//	.SW_I2C_Unlock = SW_NET_I2C_Unlock,
//	.SW_Unlock_Force = SW_NET_Unlock_Force,
//	.SW_Unlock_High_Reg = SW_NET_Unlock_High_Reg, 
//	.SW_Set_Port_Mode = SW_NET_Set_Port_Mode,

//	.SW_Init = SW_NET_Init,
//	.SW_isExist = SW_NET_Is_Exist,

//	.SW_User_Define = SW_NET_User_Define,
};



static u32 SW_NET_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
	return 1;
}

static u32 SW_NET_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{
	return 1;
}

static u32 SW_NET_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
	return 2;
}

static u32 SW_NET_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{
	return 1;
}

static u32 SW_NET_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower)
{
	return 1;
}

static u32 SW_NET_On_Off_QC(u8 port,u8 version,u8 setState)
{
	return 1;
}

static u32 SW_NET_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
	return 1;
}

static u32 SW_NET_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)
{
	return 1;
}

static u32 SW_NET_Rebroadcast_PDO(u8 port, u8 way, u32 time)
{
	return 1;
}

static u32 SW_NET_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
	return 2;
}

static u32 SW_NET_Set_PE(u8 port, u32 maxVol)
{
	return 1;
}

static u32 SW_NET_On_Off_PE(u8 port, u8 setState)
{
	return 1;
}

static u32 SW_NET_Set_Line_Com(u8 port,u8 setState, u32 setValue)
{
	return 1;
}

static u32 SW_NET_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
	return 1;
}

static u32 SW_NET_Set_Unload_Current(u8 port, u8 num, u32 setValue)
{
	return 2;
}

static u32 SW_NET_Set_NTC_Current(u8 port, u32 setValue)
{
	
	return 2;
}

static u32 SW_NET_Set_DAC(u8 port, u32 setValue)
{
	return 2;
}

static u32 SW_NET_Get_Vin(u8 port, u32 *vinVol)
{
	return 1;
}

static u32 SW_NET_Get_Vbus(u8 port, u32 *vbusVol)
{
	return 1;
}
	
static u32 SW_NET_Get_Current(u8 port, u8 num,u32 *current)
{
	return 1;
}

static u32 SW_NET_Get_NTC_Temperature(u8 port, int *temperature)
{
	return 1;
}

static u32 SW_NET_Get_Port_State(u8 port, u8 num, u8  *portState)
{
	u8 tmpValue = 0;
	
	tmpValue = Get_Net_Dev_State(port, portState);
		
	return tmpValue;
}
	
static u32 SW_NET_Get_Sys_Power(u8 port,u32 *sysPower)
{
	return 1;
}

static u32 SW_NET_Get_Port_Protocol(u8 port,  u8 *protocol)
{
	return 1;
}

static u32 SW_NET_On_Off_Emarker(u8 port, u8 setState)
{
	return 1;
}
	
static u32 SW_NET_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
	return 2;
}

static u32 SW_NET_On_Off_Buck(u8 port, u8 setState)
{
	return 1;
}

static u32 SW_NET_I2C_Unlock(u8 port)
{
	return 2;
}

static u32 SW_NET_Unlock_Force (u8 port)
{
	return 2;
}

static u32 SW_NET_Unlock_High_Reg(u8 port, u8 setState)
{
	return 2;
}
	
static u32 SW_NET_Set_Port_Mode(u8 port, u8 mode)
{
	return 1;
}

static u32 SW_NET_Init(u8 port)
{
	return 1;
}

static u32 SW_NET_isExist(u8 port, u8 *existValue)
{
	return 2;
}

static u32 SW_NET_Set_System_Power(u8 port, u16 power)
{
	u8 tmpValue = 0;
	
	tmpValue = Set_Net_Dev_Power(port);
	
	return tmpValue ;
}

static u32 SW_NET_User_Define(u8 port, u8 funcNo, u32 *dat)
{
	return 1 ;
}	
