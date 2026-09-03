#include "sw2325_func.h"

static u32 SW2325_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW2325_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);

static u32 SW2325_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 

static u32 SW2325_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW2325_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW2325_On_Off_QC(u8 port,u8 version,u8 setState);

static u32 SW2325_Set_FCP_XSCP(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
static u32 SW2325_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState);

static u32 SW2325_Rebroadcast_PDO(u8 port, u8 way, u32 time);//
static u32 SW2325_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW2325_Set_PE(u8 port, u32 maxVol);
static u32 SW2325_On_Off_PE(u8 port, u8 setState);

static u32 SW2325_Set_Line_Com (u8 port, u8 setState,  u32 setValue);//
static u32 SW2325_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW2325_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW2325_Set_NTC_Current(u8 port, u32 setValue);//
static u32 SW2325_Set_DAC(u8 port, u32 setValue);//

static u32 SW2325_Get_Vin(u8 port, u32 *vinVol);
static u32 SW2325_Get_Vbus(u8 port, u32 *vbusVol);
static u32 SW2325_Get_Current(u8 port, u8 num,u32 *current);
static u32 SW2325_Get_NTC_Temperature(u8 port, int  *temperature);//
static u32 SW2325_Get_Port_State(u8 port, u8 num, u8  *portState);//
static u32 SW2325_Get_Sys_Power(u8 port,u32 *sysPower);
static u32 SW2325_Get_Port_Protocol(u8 port,  u8 *protocol);

static u32 SW2325_On_Off_Emarker(u8 port, u8 setState);
static u32 SW2325_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW2325_On_Off_Buck(u8 port, u8 setState);
static u32 SW2325_I2C_Unlock(u8 port);
static u32 SW2325_Unlock_Force (u8 port);
static u32 SW2325_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW2325_Set_Port_Mode(u8 port, u8 mode);

const struct sw_func SW2325_Func = 
{
  SW2325_Set_PD_Gear, 
  SW2325_Set_PD_Vol_Cur,	
	SW2325_Set_UFCS_Gear,
	SW2325_Set_VID_PID,
	
	SW2325_Set_QC,
	SW2325_On_Off_QC,
	SW2325_Set_FCP_XSCP,
	SW2325_On_Off_FCP_XSCP,
	SW2325_Rebroadcast_PDO,
	SW2325_On_Off_Charge_Protocol,
	SW2325_Set_PE,
	SW2325_On_Off_PE,
	SW2325_Set_Line_Com,
	SW2325_Set_Current_Limit,
	SW2325_Set_Unload_Current,
	SW2325_Set_NTC_Current,
	SW2325_Set_DAC,
	SW2325_Get_Vin,
	SW2325_Get_Vbus,
	SW2325_Get_Current,
	SW2325_Get_NTC_Temperature,
	SW2325_Get_Port_State,
	SW2325_Get_Sys_Power,
	
	SW2325_Get_Port_Protocol,
	SW2325_On_Off_Emarker,
	SW2325_Dpdm_Power_Same_PD,
	SW2325_On_Off_Buck,
	SW2325_I2C_Unlock,
	SW2325_Unlock_Force,
	SW2325_Unlock_High_Reg,
	SW2325_Set_Port_Mode,
};

volatile CUS_PARA_CFG *g_cus_para_cfg_p;


static u32 SW2325_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
    return 2;
}

static u32 SW2325_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{
	return 2;
}

//static u32 SW2325_Get_PD_PDO(PDO *pdoStruct,u32 *pdo )
//{


//}

///ufcs

static u32 SW2325_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
 return 2;
}

static u32 SW2325_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{
	 g_cus_para_cfg_p -> PdVID = setVID;
   g_cus_para_cfg_p -> PdPID = setPID;
	
	 return 1;
}

//afc/sfcp/pe/qc2.0 max voltage,最大电压为0：5V, 1：9V, 2:12V, 3:20V               
//afc/sfcp/pe/qc2.0 maxPower 最大功率 0：系统功率, 1:18W, 2：30W, 3:36W       
//qc3 and qc3p max power 0:18w 1:27W 2:40W 3:60W/45W; qc3 and qc3p max 

static u32 SW2325_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower)//QC
{
	u8 tmpVolGear = 0;
	u8 tmpPwrGear = 0;
	
	maxVol /= 1000;//单位化为V
	if(maxVol <= 5)
	{
		tmpVolGear = 0;
	}
	else if(maxVol <= 9)
	{
		tmpVolGear = 1;
	}
	else if(maxVol <= 12)
	{
		tmpVolGear = 2;
	}
	else 
	{
		tmpVolGear = 3;
	}
	
	maxPower /= 1000;
	if(maxPower <= 18)
	{
		tmpPwrGear = 0;
	}
	else if(maxPower <= 30)
	{
		tmpPwrGear = 1;
	}
	else if(maxPower <= 40)
	{
		tmpPwrGear = 2;
	}
	else
	{
		tmpPwrGear = 3;
	}
	
   switch(qcVersion)
	 {
		 case QC2_PROTOCOL_IC : 
												g_cus_para_cfg_p -> dpdmFixProMaxVol =  tmpVolGear;
		 
												if((maxPower > 0) && (tmpPwrGear < 3))
												{
													g_cus_para_cfg_p -> dpdmFixProMaxPower = tmpPwrGear + 1;
												}
												else
												{
													g_cus_para_cfg_p -> dpdmFixProMaxPower = 0;
												}
												break;
		 case QC3_PROTOCOL_IC : 
		 case QC3_P_PROTOCOL_IC : 
												g_cus_para_cfg_p -> qc3MaxVol  =  maxVol;
												g_cus_para_cfg_p ->qc3MaxPower =  tmpPwrGear;	 
												break;
	 }		
	
	 return 1;
}

static u32 SW2325_On_Off_QC(u8 port,u8 version,u8 setState)//QC version:1=QC2.0   2=QC3.0
{
    switch(version)
	 {
		 case QC2_PROTOCOL_IC : 
												g_cus_para_cfg_p ->qc2p0En = setState;
												break;
		 case QC3_PROTOCOL_IC : 
												g_cus_para_cfg_p ->qc3p0En = setState;
												break;
		 case QC3_P_PROTOCOL_IC : 
												g_cus_para_cfg_p ->qc3ppEn = setState;
												break;
	 }		
	
	 return 1;
}

static u32 SW2325_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
   g_cus_para_cfg_p -> dpdmFixProMaxVol =  maxVol;
	 g_cus_para_cfg_p -> dpdmFixProMaxPower = maxPower;
								
	 return 1;
}

static u32 SW2325_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)//fcp:3  scp:4
{
	 switch(fcpVersion)
	 {
		 case FCP_PROTOCOL_IC : 
												g_cus_para_cfg_p -> fcpEn = setState;
												break;
		 case H_SCP_PROTOCOL_IC : 
												g_cus_para_cfg_p -> hscpEn = setState;
												break;
		 case L_SCP_PROTOCOL_IC : 
												g_cus_para_cfg_p -> lscpEn = setState;
												break;
	 }		
	
	 return 1;
}


static u32 SW2325_Rebroadcast_PDO(u8 port, u8 way, u32 time)//
{
 
	 g_cus_para_cfg_p -> Ev_TxPdPDO = 1;	
	 return 1;
}

static u32 SW2325_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
	 return 2;
}

//afc/sfcp/pe/qc2.0 max voltage,最大电压为0：5V, 1：9V, 2:12V, 3:20V      
static u32 SW2325_Set_PE(u8 port, u32 maxVol)
{
	 g_cus_para_cfg_p -> dpdmFixProMaxVol =  maxVol;
	 return 1;
}

static u32 SW2325_On_Off_PE(u8 port, u8 setState)
{
   g_cus_para_cfg_p -> peEn  = setState;
	 return 1;
}
/*
    COMP50M = 0,  
    COMP0M,  
    COMP100M,
    COMP150M,
*/
static u32 SW2325_Set_Line_Com(u8 port,u8 setState, u32 setValue)//setValue?
{
   g_cus_para_cfg_p -> line_comp =  (LINE_COMPENSATION)setValue;  
	 return 1;
}
/*
current：检测输出电流
ADC_NTC_OUTPUT_0UA
ADC_NTC_OUTPUT_20UA
ADC_NTC_OUTPUT_40UA
ADC_NTC_OUTPUT_100UA （默认值）
*/
static u32 SW2325_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
   DAC_Set_Curr(setValue);
   return 1;
}

static u32 SW2325_Set_Unload_Current(u8 port, u8 num, u32 setValue)
{
   return 2;
}

static u32 SW2325_Set_NTC_Current(u8 port, u32 setValue)
{ 	
	
  ADC_Ntc_Output_Current_Cfg(setValue);	
	
  return 1;
}

static u32 SW2325_Set_DAC(u8 port, u32 setValue)
{
  return 2;
}

static u32 SW2325_Get_Vin(u8 port, u32 *vinVol)
{
  
  *vinVol = ADC_Get_Vin_Actual_Data();
  return 1;
}

static u32 SW2325_Get_Vbus(u8 port, u32 *vbusVol)
{
 
	*vbusVol = ADC_Get_Vbus_Actual_Data();
	return 1;
}
	
static u32 SW2325_Get_Current(u8 port, u8 num,u32 *current)
{
 
	*current = ADC_Get_Ich_Actual_Data();
	return 1;
}
	
static u32 SW2325_Get_NTC_Temperature(u8 port, int  *temperature)
{
   *temperature = ADC_Get_Ts_Actual_Data();
	return 1;
}

static u32 SW2325_Get_Port_State(u8 port,u8 num, u8  *portState)
{
	u32 *tmpState = 0;
	
	tmpState = (u32 *)portState;
	
	*tmpState  = (bool)(Cus_Get_TypeC_State(SRC_CONNECT) > 0);
	return 1;
}
	
static u32 SW2325_Get_Sys_Power(u8 port,u32 *sysPower)
{
	return 2;
}

static u32 SW2325_Get_Port_Protocol(u8 port,  u8 *protocol)
{
	return 2;
}

static u32 SW2325_On_Off_Emarker(u8 port, u8 setState)
{
	g_cus_para_cfg_p -> PdReadEmarker_En = setState;
	
	return 1;
}

//static u32 SW2325_NTC_Gear_Func_PDO(u8 port, u8 *temp_gear_t tempGear )
//{}
	
static u32 SW2325_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
	
	g_cus_para_cfg_p -> dpdmFixProMaxPower = setState;
	
 	return 1;
}
//buck-boost要为系统供电，此时只能开关gate
static u32 SW2325_On_Off_Buck(u8 port, u8 setState)
{
  if(setState)   
	{
	 DAC_Gate_Enable();
	}
	else
	{
	 DAC_Gate_Disable();
	}
	
	return 1;
}

static u32 SW2325_I2C_Unlock(u8 port)
{
	return 2;
}

static u32 SW2325_Unlock_Force (u8 port)
{
	return 2;
}

static u32 SW2325_Unlock_High_Reg(u8 port, u8 setState)
{
 	return 2;
}
	
static u32 SW2325_Set_Port_Mode(u8 port, u8 mode)
{
	  return 2;
}


void SW2325_Init(CUS_PARA_CFG *cusParaCfg)
{
  g_cus_para_cfg_p = cusParaCfg;
}
