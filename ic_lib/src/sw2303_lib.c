#include "sw2303_func.h"
#include "user_define.h"

extern const struct sw_i2c_drive sw_port_i2c_drive[4];
extern const unsigned char icAddr[4];

static u32 SW2303_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW2303_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);

static u32 SW2303_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 

static u32 SW2303_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW2303_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW2303_On_Off_QC(u8 port,u8 version,u8 setState);

static u32 SW2303_Set_FCP_XSCP(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
static u32 SW2303_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState);

static u32 SW2303_Rebroadcast_PDO(u8 port, u8 way, u32 time);//
static u32 SW2303_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW2303_Set_PE(u8 port, u32 maxVol);
static u32 SW2303_On_Off_PE(u8 port, u8 setState);

static u32 SW2303_Set_Line_Com (u8 port, u8 setState,  u32 setValue);//
static u32 SW2303_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW2303_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW2303_Set_NTC_Current(u8 port, u32 setValue);//
static u32 SW2303_Set_DAC(u8 port, u32 setValue);//

static u32 SW2303_Get_Vin(u8 port, u32 *vinVol);
static u32 SW2303_Get_Vbus(u8 port, u32 *vbusVol);
static u32 SW2303_Get_Current(u8 port, u8 num,u32 *current);
static u32 SW2303_Get_NTC_Temperature(u8 port, int *temperature);//
static u32 SW2303_Get_Port_State(u8 port, u8 num, u8  *portState);//
static u32 SW2303_Get_Sys_Power(u8 port,u32 *sysPower);
static u32 SW2303_Get_Port_Protocol(u8 port,  u8 *protocol);

static u32 SW2303_On_Off_Emarker(u8 port, u8 setState);
static u32 SW2303_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW2303_On_Off_Buck(u8 port, u8 setState);
static u32 SW2303_I2C_Unlock(u8 port);
static u32 SW2303_Unlock_Force (u8 port);
static u32 SW2303_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW2303_Set_Port_Mode(u8 port, u8 mode);

const struct sw_func SW2303_Func = 
{
	SW2303_Set_PD_Gear, 
	SW2303_Set_PD_Vol_Cur,	
	SW2303_Set_UFCS_Gear,
	SW2303_Set_VID_PID,
	
	SW2303_Set_QC,
	SW2303_On_Off_QC,
	SW2303_Set_FCP_XSCP,
	SW2303_On_Off_FCP_XSCP,
	SW2303_Rebroadcast_PDO,
	SW2303_On_Off_Charge_Protocol,
	SW2303_Set_PE,
	SW2303_On_Off_PE,
	SW2303_Set_Line_Com,
	SW2303_Set_Current_Limit,
	SW2303_Set_Unload_Current,
	SW2303_Set_NTC_Current,
	SW2303_Set_DAC,
	SW2303_Get_Vin,
	SW2303_Get_Vbus,
	SW2303_Get_Current,
	SW2303_Get_NTC_Temperature,
	SW2303_Get_Port_State,
	SW2303_Get_Sys_Power,
	
	SW2303_Get_Port_Protocol,
	SW2303_On_Off_Emarker,
	SW2303_Dpdm_Power_Same_PD,
	SW2303_On_Off_Buck,
	SW2303_I2C_Unlock,
	SW2303_Unlock_Force,
	SW2303_Unlock_High_Reg,
	SW2303_Set_Port_Mode,
};



//读回-修改-写
#ifndef I2C_ONE_TIME

//0 - 8分别为5V, 9V,12V,15V,20V,PPS0,PPS1,PPS1,PPS3

static u32 SW2303_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
    u8 tmpValue = 0; 
	u8 tmpPos   =  volGear - 1;
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], PD_CONFIG2_RW_REG_B5, &tmpValue); 
	
	if(setState == 0)
	{
	  tmpValue &= (~(1 << tmpPos));
	}
	else
	{
	 tmpValue |= (1 << tmpPos);
	}
	
 	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], PD_CONFIG2_RW_REG_B5, tmpValue); 
 
   return 1;
}



static u32 SW2303_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{
	return 2;
}

//static u32 SW2303_Get_PD_PDO(PDO *pdoStruct,u32 *pdo )
//{


//}

///ufcs

static u32 SW2303_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
 return 2;
}

static u32 SW2303_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{
   uint8_t tmpValue = 0;
    
   tmpValue = (setVID >> 8);
   sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_VID_REG_H_B6, tmpValue);

 	 tmpValue = (uint8_t)(setVID);
   sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_VID_REG_L_B7, tmpValue);
	
	 tmpValue = (setPID >> 8);
   sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PID_REG_H_BC, tmpValue);

 	 tmpValue = (uint8_t)(setPID);
   sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PID_REG_L_BD, tmpValue);
	 
	 return 1;
}

//此处QC2,QC3的电压只能选择是否关闭12V和20V，且如果关闭12V，则也必须关闭20V。且小于12V后，QC2/QC3/PE都会小于12V
//功率为总系统功率，一旦设置，整个系统的最大功率将固定。
static u32 SW2303_Set_QC(u8 port, u8 qcVersion, u32 maxVol, u32 maxPower)
{
	u8 tmpValue = 0;	  
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, &tmpValue);
	
	if(maxVol < 12000)
	 {
	   tmpValue |= 0x09;
	 }
    else if(maxVol < 20000)
	{
  	   switch(qcVersion)
		 {
			case QC2_PROTOCOL_IC :
								tmpValue &= (~NOT_PD_LOW_12_MSK);
								tmpValue &= (~QC2_LOW_20_MSK);
								break;
			case QC3_PROTOCOL_IC :
								tmpValue &= (~NOT_PD_LOW_12_MSK);
								tmpValue |= QC3_LOW_20_MSK;
								break;
		 }
	}
	else
	{	 
		 switch(qcVersion)
		  {
			case QC2_PROTOCOL_IC :
								tmpValue &= (~NOT_PD_LOW_12_MSK);
								tmpValue |= QC2_LOW_20_MSK;
								break;
			case QC3_PROTOCOL_IC :
								tmpValue &= (~NOT_PD_LOW_12_MSK);
								tmpValue &= (~QC3_LOW_20_MSK);
								break;
		  }
	}
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, tmpValue);
	
	return 1;
}


static u32 SW2303_On_Off_QC(u8 port,u8 version,u8 setState)
{
	u8 tmpValue = 0;  
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, &tmpValue);
	
	switch(version)
	{
			case QC2_PROTOCOL_IC :
								if(setState)
								{
				                   tmpValue &= (~EN_QC2_MSK);
								}
								else
								{
									tmpValue |= (EN_QC2_MSK | EN_QC3_MSK);
								}
			                    break;
			case QC3_PROTOCOL_IC :
				                if(setState)
								{
				                   tmpValue &= (~(EN_QC2_MSK | EN_QC3_MSK));
								}
								else
								{
									tmpValue |= EN_QC3_MSK;
								}
			                    break;
	}
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, tmpValue);

	return 1;
} 

//SCP可以设置4档电流，0：5A，1：4A，2：3A 不能设置电压
//FCP和AFC/SFCP一起设置，只能设置两档电流0：3.25，1：2.25。不能设置电压
//系统有个位置是设置最大功率，是整个系统共用的最大功率。
static u32 SW2303_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
	 return 2;
}

static u32 SW2303_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)//fcp:3  scp:4
{
	u8 tmpValue = 0;
	u8 tmpReg   = 0;
    u8 fcpVersionPos = 0;	  
	
	  if(fcpVersion == FCP_PROTOCOL_IC)
		{
			tmpReg =  QUICK_CHARGE_CONFIG_3_B1;
	    }
		else
		{
		 tmpReg =  QUICK_CHARGE_CONFIG_2_B0;
		}
		
		sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], tmpReg, &tmpValue);
		
		switch(fcpVersion)
		{
			case FCP_PROTOCOL_IC:
				                  fcpVersionPos = EN_FCP_POS;
				                  break;
			case H_SCP_PROTOCOL_IC:
				                  fcpVersionPos = EN_H_SCP_POS;
				                  break;
			case L_SCP_PROTOCOL_IC:
				                  fcpVersionPos = EN_L_SCP_POS;
				                  break;			
		}
		
		if(setState)
		{
		  tmpValue &= (~(1 << fcpVersionPos));
		}
		else
		{
		  tmpValue |= (1 << fcpVersionPos);
		} 
		
		sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], tmpReg, tmpValue);
	  
	  return 1;
}

static u32 SW2303_Rebroadcast_PDO(u8 port, u8 way, u32 time)//
{
	u8 tmpValue = 0;
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], 0x73, &tmpValue);
	
	tmpValue |= 0x80;
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], 0x73, tmpValue);
	return 1;
}

static u32 SW2303_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
    u8 tmpValue = 0;
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, &tmpValue);
	
	if(setState)
		{
		 tmpValue &= (~EN_QC_ALL_MSK);
		}
	else
		{
		 tmpValue |= EN_QC_ALL_MSK;
		}
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, tmpValue);
	 
    return 1;
}

static u32 SW2303_Set_PE(u8 port, u32 maxVol)
{
   u8 tmpValue = 0; 
	
   sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, &tmpValue);
	
	if(maxVol < 12000)
	 {
	   tmpValue |= 0x09;
	 }
    else if(maxVol < 20000)
	{
	   tmpValue &= (~NOT_PD_LOW_12_MSK);
	   tmpValue &= (~PE_LOW_20_MSK);	
	}
	else
	{
	   tmpValue |= PE_LOW_20_MSK;
	}
	
   sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, tmpValue);
	
   return 1;
}

static u32 SW2303_On_Off_PE(u8 port, u8 setState)
{
    
	 u8 tmpValue = 0;
	   
	 sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_3_B1, &tmpValue);
	  
	  if(setState)
		{
			tmpValue &= (~(EN_PE_MSK)); 
	    }
		else
		{
	        tmpValue |= EN_PE_MSK;
	    }
		sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_3_B1, tmpValue);

	 return 1;
}

static u32 SW2303_Set_Line_Com(u8 port,u8 setState, u32 setValue)//setValue?
{
	u8 tmpValue = 0;
	   
    sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], CONNECT_CTRL_REG_14, &tmpValue);
	
     if(setState)
	 {
	   tmpValue &= (~LINE_COMP_MSK);
	 }
	 else
	 {
	   tmpValue |= LINE_COMP_MSK;
	 }
	 
	 sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], CONNECT_CTRL_REG_14, tmpValue);
	 return 1;
}
//强制限流后，后期不使用，一定要手动关闭。
static u32 SW2303_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
	u8 tmpValue = 0; 
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], FORCE_CTRL_REG_16, &tmpValue);
	
	tmpValue |= FORCE_CURRENT_LIMMIT_MSK;
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], FORCE_CTRL_REG_16, tmpValue);
	
	tmpValue = (setValue - 1000) / 50;
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], CUR_LIMMIT_R_REG_05, tmpValue);
	
	return 1;
}
//在线电流只有4档，0：200ma,1:100ma,2:300ma,3:400mamt
static u32 SW2303_Set_Unload_Current(u8 port, u8 num, u32 setValue)
{
	 u8 tmpValue = 0;
	 u8 tmpRegValue = 0;
  
	 sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ONLIE_CONFIG_REG_AC, &tmpValue);
	
	 if(setValue <= 100)
	 {
	   tmpRegValue = 1;
	 }
	 else if(setValue <= 200)
	 {
	   tmpRegValue = 0;
	 }
	 else if(setValue <= 300)
	 {
	   tmpRegValue = 2;
	 }
	 else
	 {
	   tmpRegValue = 3;
	 }
	 
	 tmpValue &= (~ONLINE_CURRENT_MSK);
	 tmpValue |= tmpRegValue;
	 
	 sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], ONLIE_CONFIG_REG_AC, tmpValue);
   return 1;
}

static u32 SW2303_Set_NTC_Current(u8 port, u32 setValue)
{
  return 2;
}
//此处已假设强制功能已经开启
static u32 SW2303_Set_DAC(u8 port, u32 setValue)
{
	u8 tmpValue = 0; 
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], FORCE_CTRL_REG_16, &tmpValue);
	
	tmpValue |= FORCE_DAC_MSK;
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], FORCE_CTRL_REG_16, tmpValue);
	
	tmpValue = (u8)(setValue >> 4);
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], DAC_DATA_H8_R_REG_03, tmpValue);
	
	tmpValue = (u8)(setValue & 0x0F);
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], DAC_DATA_L4_R_REG_04, tmpValue);
	
	return 1;
}

static u32 SW2303_Get_Vin(u8 port, u32 *vinVol)
{
    u8 tmpValue = 0;
    u32 dataValue = 0;
    u8 dataH8 = 0; 
	u8 dataL4 = 0;
	
    sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
	
	tmpValue &= ~(ADC_DATA_SEL_MSK);
	
	tmpValue |= 1;
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
	
  	dataValue |= dataH8;
    dataValue <<= 4;
    dataValue |= dataL4;
	
	*vinVol = dataValue * 7.5; 
	
	return 1;
}

static u32 SW2303_Get_Vbus(u8 port, u32 *vbusVol)
{
    u8 tmpValue = 0;
	u32 dataValue = 0;
	u8 dataH8 = 0;
	u8 dataL4 = 0;
	
    sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
	
	tmpValue &= ~(ADC_DATA_SEL_MSK);
	
	tmpValue |= 2;
	
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
	
	dataValue |= dataH8;
    dataValue <<= 4;
    dataValue |= dataL4;
	
	*vbusVol = dataValue * 7.5; 
	
	return 1;
}
	
static u32 SW2303_Get_Current(u8 port, u8 num,u32 *current)
{
    u8 tmpValue = 0;
	u32 dataValue = 0;
	u8 dataH8 = 0;
	u8 dataL4 = 0;
	
    sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
	tmpValue &= ~(ADC_DATA_SEL_MSK);
	tmpValue |= 3;
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
	
	dataValue |= dataH8;
    dataValue <<= 4;
    dataValue |= dataL4;
	
	*current = dataValue * 25 / 8; 
	
	return 1;
}
	
static u32 SW2303_Get_NTC_Temperature(u8 port, int *temperature)
{
    u8 tmpValue = 0;
	u32 dataValue = 0;
	u8 dataH8 = 0;
	u8 dataL4 = 0;
	
    sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
	tmpValue &= ~(ADC_DATA_SEL_MSK);
	tmpValue |= 4;
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
	
	dataValue |= dataH8;
    dataValue <<= 4;
    dataValue |= dataL4;
	
	*temperature = (dataValue - 1848) *93 / 625; 
	
	return 1;
}

static u32 SW2303_Get_Port_State(u8 port, u8 num, u8  *portState)
{
	 u8 tmpValue = 0;
	
	 sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SYS_STATE_3_REG_0D ,&tmpValue);
	
	 *portState = (tmpValue & ONLINE_STATE_MSK);
	
	 return 1;
}
	
static u32 SW2303_Get_Sys_Power(u8 port,u32 *sysPower)
{
     u8 tmpValue = 0;
	
	 sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], POWER_CONFIG_AF ,&tmpValue);
	
	 *sysPower = tmpValue & 0x7F;
	  
	 return 1;
}

static u32 SW2303_Get_Port_Protocol(u8 port,  u8 *protocol)
{
    u8 tmpValue = 0;
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],QUICK_CHARGE_STA_REG_06,&tmpValue);
	
    *protocol = tmpValue & QUICK_CHARGE_PROTOCOL_MSK;

	return 1;
}

static u32 SW2303_On_Off_Emarker(u8 port, u8 setState)
{
   u8 tmpValue = 0;
	
   sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], PD_CONFIG_0_B3 ,&tmpValue);

   if(setState)
	{
	  tmpValue &= ~(EN_PD_READ_EMARKER_MSK); 
	}
	 
  else
   {
	 tmpValue |= EN_PD_READ_EMARKER_MSK;
	
   }
	 sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], PD_CONFIG_0_B3, tmpValue);   
	
   return 1;
}

//static u32 SW2303_NTC_Gear_Func_PDO(u8 port, u8 *temp_gear_t tempGear )
//{}
	
static u32 SW2303_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
 	return 2;
}
//此处只强制开关通路，不开关buck。且假设强制功能已经开启。
static u32 SW2303_On_Off_Buck(u8 port, u8 setState)
{
	u8 tmpValue = 0;
	
    sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], FORCE_CTRL_REG_16, &tmpValue);
	
	if(setState)
	{
		tmpValue &= 0xF3;
		tmpValue |= FORCE_OPEN_GATE_MSK;
	}
	else
	{
		tmpValue &= 0xF3;
		tmpValue |= FORCE_CLOSE_GATE_MSK;
	}
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], FORCE_CTRL_REG_16, tmpValue);
	  	 
	return 1;
}
#else


static u32 SW2303_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
	       u8 tmpPos  = 0;
		   u8 returnValue = 0;
		
	switch(stepCnt)
	{
		case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], PD_CONFIG2_RW_REG_B5, &tmpValue); 

				stepCnt = 1;
				returnValue =  0;
				break;				 
		case 1: 
				if(setState == 0)
				{
				tmpValue &= (~(1 << tmpPos));
				}
				else
				{
				tmpValue |= (1 << tmpPos);
				}

				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], PD_CONFIG2_RW_REG_B5, tmpValue); 

				stepCnt = 0; 
				returnValue = 1;
				break;
	 }
	return returnValue;
}

static u32 SW2303_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{	 
	return 1;
}

static u32 SW2303_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
	return 2;
}
//static u32 SW2303_Get_PD_PDO(PDO *pdoStruct,u32 *pdo )
//{


//}

///ufcs

static u32 SW2303_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{

	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
	  case 0:
			tmpValue = (setVID >> 8);
			sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_VID_REG_H_B6, tmpValue);	
		    stepCnt = 1;
			returnValue = 0;
		    break;				
		case 1:
			tmpValue = (uint8_t)(setVID);
			sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_VID_REG_L_B7, tmpValue);
		    stepCnt = 2;
			returnValue = 0;
		    break;				
		case 2:
			tmpValue = (setPID >> 8);
			sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PID_REG_H_BC, tmpValue);
				
			stepCnt = 3;
			returnValue = 0;
		    break;				
		case 3:
			tmpValue = (uint8_t)(setPID);
			sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PID_REG_L_BD, tmpValue);
				
			stepCnt = 0;
			returnValue = 1;
		    break;
    }
	return returnValue;
}

static u32 SW2303_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower)//QC
{
	static u8 tmpValue = 0;	 
	static u8 stepCnt = 0;
   		   u8 returnValue = 0;
	
	 switch(stepCnt)
	 {
	   case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, &tmpValue);
				stepCnt = 1;
				returnValue = 0;
				break;		 
	    case 1:
				if(maxVol < 12000)
					 {
					   tmpValue |= 0x09;
					 }
					else if(maxVol < 20000)
					{
					   switch(qcVersion)
						 {
							 case QC2_PROTOCOL_IC :
													 tmpValue &= (~NOT_PD_LOW_12_MSK);
													 tmpValue &= (~QC2_LOW_20_MSK);
																		 break;
						   case QC3_PROTOCOL_IC :
													 tmpValue &= (~NOT_PD_LOW_12_MSK);
													 tmpValue |= QC3_LOW_20_MSK;
																		 break;
						 }
					}
					else
					{	 
						 switch(qcVersion)
						  {
							case QC2_PROTOCOL_IC :
													 tmpValue &= (~NOT_PD_LOW_12_MSK);
													 tmpValue |= QC2_LOW_20_MSK;
																		 break;
						  case QC3_PROTOCOL_IC :
													 tmpValue &= (~NOT_PD_LOW_12_MSK);
													 tmpValue &= (~QC3_LOW_20_MSK);
																		 break;
						  }
					}
					
					sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, tmpValue);

					stepCnt = 0;
					returnValue = 1;
					break;
	 }
	return returnValue;
}

static u32 SW2303_On_Off_QC(u8 port,u8 version,u8 setState)//QC version:1=QC2.0   2=QC3.0
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 qcVersionPos = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, &tmpValue);
			
				stepCnt = 1;				
				returnValue = 0;
				break;
		case 1:
				switch(version)
				{
						case QC2_PROTOCOL_IC :
											if(setState)
											{
											   tmpValue &= (~EN_QC2_MSK);
											}
											else
											{
												tmpValue |= (EN_QC2_MSK | EN_QC3_MSK);
											}
											break;
						case QC3_PROTOCOL_IC :
											if(setState)
											{
											   tmpValue &= (~(EN_QC2_MSK | EN_QC3_MSK));
											}
											else
											{
												tmpValue |= EN_QC3_MSK;
											}
											break;
				}
				
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, tmpValue);
							
				stepCnt = 0; 
				returnValue = 1;
				break;
	}
	return returnValue;
}	
  

static u32 SW2303_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
	return 1;
}

static u32 SW2303_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)//fcp:3  scp:4
{
    static u8 tmpValue = 0;
	static u8 stepCnt = 0;
	static u8 tmpReg = 0;
		   u8 fcpVersionPos = 0;
		   u8 returnValue = 0;
	 
	switch(stepCnt)
	{
		case 0:
			  if(fcpVersion == FCP_PROTOCOL_IC)
				{
				  tmpReg =  QUICK_CHARGE_CONFIG_3_B1;
				}
				else
				{
				 tmpReg =  QUICK_CHARGE_CONFIG_2_B0;
				}
				
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], tmpReg, &tmpValue);  
	           
			    stepCnt = 1;
				returnValue = 0;
				break;	
	     case 1:
				switch(fcpVersion)
				{
					case FCP_PROTOCOL_IC:
										  fcpVersionPos = EN_FCP_POS;
										  break;
					case H_SCP_PROTOCOL_IC:
										  fcpVersionPos = EN_H_SCP_POS;
										  break;
					case L_SCP_PROTOCOL_IC:
										  fcpVersionPos = EN_L_SCP_POS;
										  break;			
				}
				
				if(setState)
				{
				  tmpValue &= (~(1 << fcpVersionPos));
				}
				else
				{
				  tmpValue |= (1 << fcpVersionPos);
				} 
				
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], tmpReg, tmpValue);
					
                stepCnt = 0;					
				returnValue = 1;
				break;
	  }
	return returnValue;
}

static u32 SW2303_Rebroadcast_PDO(u8 port, u8 way, u32 time)//
{
     static u8 tmpValue = 0;
	 static u8 stepCnt = 0;
		   u8 returnValue = 0;
		 
	 switch(stepCnt)
	 {
		case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], 0x73, &tmpValue);
			
				stepCnt = 1;
				returnValue = 0;
				break;
		case 1:
				tmpValue |= 0x80;
			
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], 0x73, tmpValue);
	 
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW2303_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
  
	static u8 tmpValue = 0;	 
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
		
	switch(stepCnt)
	{
	
	   case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, &tmpValue);
				 
				stepCnt = 1;
				returnValue = 0;
				 
		 case 1:
				if(setState)
					{
					 tmpValue &= (~EN_QC_ALL_MSK);
					}
				else
					{
					 tmpValue |= EN_QC_ALL_MSK;
					}
				
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_2_B0, tmpValue);
				 
				stepCnt = 0;
				returnValue = 1;
	}
	return returnValue;
}

static u32 SW2303_Set_PE(u8 port, u32 maxVol)
{
    static u8 tmpValue = 0; 
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
    switch(stepCnt)
	{
		  
			case 0:
					sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, &tmpValue);
					
					stepCnt = 1;
					returnValue = 0;
			case 1:
	
					if(maxVol < 12000)
					 {
					   tmpValue |= 0x09;
					 }
					else if(maxVol < 20000)
					{
					   tmpValue &= (~NOT_PD_LOW_12_MSK);
					   tmpValue &= (~PE_LOW_20_MSK);	
					}
					else
					{
					   tmpValue |= PE_LOW_20_MSK;
					}
					
				   sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_1_AE, tmpValue);
					
				   stepCnt = 0;
				   returnValue = 1;
	}
	return returnValue;
}

static u32 SW2303_On_Off_PE(u8 port, u8 setState)
{
    static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
			 
	switch(stepCnt)
	{
	
		case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_CONFIG_3_B1, &tmpValue);

				stepCnt = 1;
				returnValue = 0; 
		case 1:
				if(setState)
				{
					tmpValue &= (~(EN_PE_MSK)); 
				}
				else
				{
					tmpValue |= EN_PE_MSK;
				}
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], QUICK_CHARGE_CONFIG_3_B1, tmpValue);

				stepCnt = 0;
				returnValue = 1;
	}
	return returnValue;
}

static u32 SW2303_Set_Line_Com(u8 port,u8 setState, u32 setValue)//setValue?
{
    static u8 tmpValue = 0;	 
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
		
	switch(stepCnt)
	{	   
		case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], CONNECT_CTRL_REG_14, &tmpValue);
				
				stepCnt = 1;
				returnValue = 0;			 
		case 1:
				 if(setState)
				 {
				   tmpValue &= (~LINE_COMP_MSK);
				 }
				 else
				 {
				   tmpValue |= LINE_COMP_MSK;
				 }
				 
				 sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], CONNECT_CTRL_REG_14, tmpValue);
				
				 stepCnt = 0;
				 returnValue = 1;
	}
	return returnValue;
}

static u32 SW2303_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
  
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
   switch(stepCnt)
   {
	  case 0:
			sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], FORCE_CTRL_REG_16, &tmpValue);
			tmpValue |= FORCE_CURRENT_LIMMIT_MSK;
			
			stepCnt = 1;
			returnValue = 0;

	  case 1:
			sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], FORCE_CTRL_REG_16, tmpValue);
			
			stepCnt = 2;
			returnValue = 0;
	  case 2:
			tmpValue = (setValue - 1000) / 50;
			
			sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], CUR_LIMMIT_R_REG_05, tmpValue);	
			
			stepCnt = 0;
			returnValue = 1;
	}
	return returnValue;
}

static u32 SW2303_Set_Unload_Current(u8 port, u8 num,  u32 setValue)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;  
		   u8 tmpRegValue = 0;
		   u8 returnValue = 0;
		
   switch(stepCnt)
   {
	  case 0:	
			sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ONLIE_CONFIG_REG_AC, &tmpValue); 

			stepCnt = 1;
			returnValue = 0;
	  case 1:
			if(setValue <= 100)
			{
				tmpRegValue = 1;
			}
			else if(setValue <= 200)
			{
				tmpRegValue = 0;
			}
			else if(setValue <= 300)
			{
				tmpRegValue = 2;
			}
			else
			{
				tmpRegValue = 3;
			}

			tmpValue &= (~ONLINE_CURRENT_MSK);
			tmpValue |= tmpRegValue;

			sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], ONLIE_CONFIG_REG_AC, tmpValue);

			stepCnt = 0;
			returnValue = 1;
	}
	return returnValue;
}

static u32 SW2303_Set_NTC_Current(u8 port, u32 setValue)
{
	return 2;
}

static u32 SW2303_Set_DAC(u8 port, u32 setValue)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0; 
		   u8 returnValue = 0;
	
   switch(stepCnt)
   {
		case 0:	
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], FORCE_CTRL_REG_16, &tmpValue);
				
				stepCnt = 1;
				returnValue = 0;
		case 1:
				tmpValue |= FORCE_DAC_MSK;
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], FORCE_CTRL_REG_16, tmpValue);	
				
				stepCnt = 2;
				returnValue = 0;			
		case 2:
				tmpValue = (u8)(setValue >> 4);
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], DAC_DATA_H8_R_REG_03, tmpValue);	
				
				stepCnt = 3;
				returnValue = 0;		  
		case 3:
				tmpValue = (u8)(setValue & 0x0F);
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], DAC_DATA_L4_R_REG_04, tmpValue);
				
				stepCnt = 0;
				returnValue = 1;		
  }
	return returnValue;   
}

static u32 SW2303_Get_Vin(u8 port, u32 *vinVol)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0; 
	static u8 dataH8 = 0;
		   u8 dataL4 = 0;
	       u32 dataValue = 0;
		   u8 returnValue = 0;
		
   switch(stepCnt)
   {
		case 0:	
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
				
				stepCnt = 1;
				returnValue = 0;
		case 1:
				tmpValue &= ~(ADC_DATA_SEL_MSK);
				tmpValue |= 1;
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
				
				stepCnt = 2;
				returnValue = 0;			
		case 2:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
				
				stepCnt = 3;
				returnValue = 0;		  
		case 3:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
				
				dataValue |= dataH8;
				dataValue <<= 4;
				dataValue |= dataL4;
				
				*vinVol = dataValue * 7.5; 
				
				stepCnt = 0;
				returnValue = 1;		
  }
	return returnValue;  
}

static u32 SW2303_Get_Vbus(u8 port, u32 *vbusVol)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0; 
	static u8 dataH8 = 0;
		   u8 dataL4 = 0;
	       u32 dataValue = 0;		
		   u8 returnValue = 0;
	
   switch(stepCnt)
   {
		case 0:	
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
				
				stepCnt = 1;
				returnValue = 0;
		case 1:
				tmpValue &= ~(ADC_DATA_SEL_MSK);
				tmpValue |= 2;
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
				
				stepCnt = 2;
				returnValue = 0;			
		case 2:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
				
				stepCnt = 3;
				returnValue = 0;		  
		case 3:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
				
				dataValue |= dataH8;
				dataValue <<= 4;
				dataValue |= dataL4;
				
				*vbusVol = dataValue * 7.5; 
				
				stepCnt = 0;
				returnValue = 1;		
	}
	return returnValue;
}
	
static u32 SW2303_Get_Current(u8 port, u8 num,u32 *current)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0; 
	static u8 dataH8 = 0;
		   u8 dataL4 = 0;
	       u32 dataValue = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:	
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
				
				stepCnt = 1;
				returnValue = 0;
		case 1:
				tmpValue &= ~(ADC_DATA_SEL_MSK);
				tmpValue |= 3;
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
				
				stepCnt = 2;
				returnValue = 0;			
		case 2:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
				
				stepCnt = 3;
				returnValue = 0;		  
		case 3:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
				
				dataValue |= dataH8;
				dataValue <<= 4;
				dataValue |= dataL4;
				
				*current = dataValue * 25 / 8; 
				
				stepCnt = 0;
				returnValue = 1;		
	}
		return returnValue ;
}
	
static u32 SW2303_Get_NTC_Temperature(u8 port, int *temperature)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0; 
	static u8 dataH8 = 0;
		   u8 dataL4 = 0;
	       u32 dataValue = 0;		   
		   u8 returnValue = 0;
	
   switch(stepCnt)
   {
		case 0:	
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_ADC_REG_3B, &tmpValue);
				
				stepCnt = 1;
				return 0;
		case 1:
				tmpValue &= ~(ADC_DATA_SEL_MSK);
				tmpValue |= 4;
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_ADC_REG_3B, tmpValue);
				
				stepCnt = 2;
				returnValue = 0;
				break;		
		case 2:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_H8_REG_3C, &dataH8);
				
				stepCnt = 3;
				returnValue = 0;
				break;
		case 3:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_DATA_L4_REG_3D, &dataL4);
				
				dataValue |= dataH8;
				dataValue <<= 4;
				dataValue |= dataL4;
				
				*temperature = (dataValue - 1848) *93 / 625; 
				
				stepCnt = 0;
				returnValue = 1;
				break;		
  }  
	return returnValue;
}

static u32 SW2303_Get_Port_State(u8 port, u8 num, u8  *portState)
{
	 u8 tmpValue = 0;
	
	 sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SYS_STATE_3_REG_0D ,&tmpValue);
	
	 *portState = (tmpValue & ONLINE_STATE_MSK);
	
	 return 1;
}
	
static u32 SW2303_Get_Sys_Power(u8 port,u32 *sysPower)
{
     u8 tmpValue = 0;
	
	 sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], POWER_CONFIG_AF ,&tmpValue);
	
	 *sysPower = tmpValue & 0x7F;
	  
	 return 1;
}

static u32 SW2303_Get_Port_Protocol(u8 port,  u8 *protocol)
{
    u8 tmpValue = 0;
	
	sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],QUICK_CHARGE_STA_REG_06,&tmpValue);
	
    *protocol = tmpValue & QUICK_CHARGE_PROTOCOL_MSK;

	return 1;
}

static u32 SW2303_On_Off_Emarker(u8 port, u8 setState)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;	 
	
	switch(stepCnt)
	{ 
		case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], PD_CONFIG_0_B3 ,&tmpValue);
					 
				stepCnt = 1;
				returnValue = 0;
				break;				
		 case 1:
				if(setState)
				{
					tmpValue &= ~(EN_PD_READ_EMARKER_MSK); 
				}
				 
				else
				{
					tmpValue |= EN_PD_READ_EMARKER_MSK;
				}
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], PD_CONFIG_0_B3, tmpValue);  
			 
				stepCnt = 0;	
				returnValue = 1;
				break;
	 }
	return returnValue;
}
	
static u32 SW2303_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
 	return 2;
}

static u32 SW2303_On_Off_Buck(u8 port, u8 setState)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
	
	   case 0:
				sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], FORCE_CTRL_REG_16, &tmpValue);
				
				stepCnt = 1;
				returnValue = 0;
				break;	   
		 case 1:
				if(setState)
				{
					tmpValue &= 0xF3;
					tmpValue |= FORCE_OPEN_GATE_MSK;
				}
				else
				{
					tmpValue &= 0xF3;
					tmpValue |= FORCE_CLOSE_GATE_MSK;
				}
				sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], FORCE_CTRL_REG_16, tmpValue);
				 
				stepCnt = 0;
				returnValue = 1;
				break;				
	 }
	return returnValue;
}

#endif

static u32 SW2303_I2C_Unlock(u8 port)
{
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_CTRL_REG_12, 0x20);

	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_CTRL_REG_12, 0x40);

	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_CTRL_REG_12, 0x80);

	return 1;
}

static u32 SW2303_Unlock_Force (u8 port)
{
	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_PWR_FORCE_REG_15, 0x20);

	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_PWR_FORCE_REG_15, 0x40);

	sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_PWR_FORCE_REG_15, 0x80);

	return 1;
}

static u32 SW2303_Unlock_High_Reg(u8 port, u8 setState)
{
 	return 2;
}

static u32 SW2303_Set_Port_Mode(u8 port, u8 mode)
{
	return 2;
}
