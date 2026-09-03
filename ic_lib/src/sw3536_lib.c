#include "sw3536_func.h"
#include "user_define.h"

extern const struct sw_i2c_drive sw_port_i2c_drive[4];
extern const unsigned char icAddr[4];

static u32 SW3536_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW3536_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);

static u32 SW3536_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 

static u32 SW3536_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW3536_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW3536_On_Off_QC(u8 port,u8 version,u8 setState);

static u32 SW3536_Set_FCP_XSCP(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
static u32 SW3536_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState);

static u32 SW3536_Rebroadcast_PDO(u8 port, u8 way, u32 time);//
static u32 SW3536_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW3536_Set_PE(u8 port, u32 maxVol);
static u32 SW3536_On_Off_PE(u8 port, u8 setState);

static u32 SW3536_Set_Line_Com (u8 port, u8 setState,  u32 setValue);//
static u32 SW3536_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW3536_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW3536_Set_NTC_Current(u8 port, u32 setValue);//
static u32 SW3536_Set_DAC(u8 port, u32 setValue);//

static u32 SW3536_Get_Vin(u8 port, u32 *vinVol);
static u32 SW3536_Get_Vbus(u8 port, u32 *vbusVol);
static u32 SW3536_Get_Current(u8 port, u8 num,u32 *current);
static u32 SW3536_Get_NTC_Temperature(u8 port, int *temperature);//
static u32 SW3536_Get_Port_State(u8 port, u8 num, u8 *portState);//
static u32 SW3536_Get_Sys_Power(u8 port,u32 *sysPower);
static u32 SW3536_Get_Port_Protocol(u8 port,  u8 *protocol);

static u32 SW3536_On_Off_Emarker(u8 port, u8 setState);
static u32 SW3536_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW3536_On_Off_Buck(u8 port, u8 setState);
static u32 SW3536_I2C_Unlock(u8 port);
static u32 SW3536_Unlock_Force (u8 port);
static u32 SW3536_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW3536_Set_Port_Mode(u8 port, u8 mode);

static u32 SW3536_Init(u8 port);
static u32 SW3536_Is_Exist(u8 port, u8 *existValue);
static u32 SW3536_Set_System_Power(u8 port, u16 power);
static u32 SW3536_User_Define(u8 port, u8 funcNo, u32 *dat);

const struct sw_func SW3536_Func = 
{
	.SW_Init = SW3536_Init,	
	.SW_Rebroadcast_PDO = SW3536_Rebroadcast_PDO,

	.SW_Set_System_Power  = SW3536_Set_System_Power,
	.SW_User_Define = SW3536_User_Define,	
	.SW_Set_Line_Com = SW3536_Set_Line_Com,
	.SW_Set_Current_Limit = SW3536_Set_Current_Limit,
	.SW_Set_Unload_Current = SW3536_Set_Unload_Current,
	.SW_Set_NTC_Current = SW3536_Set_NTC_Current,
	.SW_Set_DAC = SW3536_Set_DAC,
	
	.SW_Get_Vin = SW3536_Get_Vin,
	.SW_Get_Vbus = SW3536_Get_Vbus,
	.SW_Get_Current = SW3536_Get_Current, 
	.SW_Get_NTC_Temperature = SW3536_Get_NTC_Temperature,
	.SW_Get_Port_State = SW3536_Get_Port_State,
	.SW_Get_Sys_Power = SW3536_Get_Sys_Power,	
	.SW_Get_Port_Protocol = SW3536_Get_Port_Protocol,
	
//	.SW_On_Off_Emarker = SW3536_On_Off_Emarker,
//	.SW_Dpdm_Power_Same_PD = SW3536_Dpdm_Power_Same_PD,
//	.SW_On_Off_Buck = SW3536_On_Off_Buck,
//	.SW_I2C_Unlock = SW3536_I2C_Unlock,
//	.SW_Unlock_Force = SW3536_Unlock_Force,
//	.SW_Unlock_High_Reg = SW3536_Unlock_High_Reg, 
//	.SW_Set_Port_Mode = SW3536_Set_Port_Mode,
//	.SW_Set_PD_Gear = SW3536_Set_PD_Gear, 
//	.SW_Set_PD_Vol_Cur = SW3536_Set_PD_Vol_Cur,	
//   
//	.SW_Set_UFCS_Gear = SW3536_Set_UFCS_Gear,	
	
//	.SW_Set_VID_PID = SW3536_Set_VID_PID,
//	.SW_Set_QC = SW3536_Set_QC,
//	.SW_On_Off_QC = SW3536_On_Off_QC,
	
//	.SW_Set_FCP_XSCP = 	SW3536_Set_FCP_XSCP,
//	.SW_On_Off_FCP_XSCP = SW3536_On_Off_FCP_XSCP,
//	.SW_On_Off_Charge_Protocol = SW3536_On_Off_Charge_Protocol,
//	.SW_Set_PE = SW3536_Set_PE,
//	.SW_On_Off_PE = SW3536_On_Off_PE,
//	.SW_isExist = SW3536_Is_Exist,
};

static 	u8 portRegAddr[5] = {0 , 1 , 2 , 3 , 4}; //此处设置此变量防止同器件不同端口，高低寄存器出现错误。
static  u8 nowRegHighOrLow[5] = {0 , 0 , 0 , 0 , 0};

#define REG_HIGH_LOW(n)	nowRegHighOrLow[n]


static u32 SW3536_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
	return 1;
}

static u32 SW3536_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{
	return 1;
}


static u32 SW3536_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
	return 2;//不支持 返回2
}

static u32 SW3536_Set_VID_PID(u8 port, u32 setVID, u32 setPID)
{
	return 1;
}

static u32 SW3536_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower)
{
	return 1;
}

static u32 SW3536_On_Off_QC(u8 port,u8 version,u8 setState)
{
	return 1;
}

static u32 SW3536_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
	return 1;		
}

static u32 SW3536_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)
{
	return 1;
}

static u32 SW3536_Rebroadcast_PDO(u8 port, u8 way, u32 time)//
{
     return 1;
}

static u32 SW3536_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
	return 1;
}

static u32 SW3536_Set_PE(u8 port, u32 maxVol)
{
	return 1;
}

static u32 SW3536_On_Off_PE(u8 port, u8 setState)
{
	return 1;
}

static u32 SW3536_Set_Line_Com(u8 port,u8 setState, u32 setValue)//setValue?
{
	return 1;
}

static u32 SW3536_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
	return 1;
}

static u32 SW3536_Set_Unload_Current(u8 port, u8 num, u32 setValue)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
	static u8 stepResult = 0;


	if(nowRegHighOrLow[portRegAddr[port]] == 0)
	{
		SW3536_Unlock_High_Reg(port, 1);
		nowRegHighOrLow[portRegAddr[port]] = 1;
	}	

	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], (u8)SET_NO_LOAD1_REG_110, &tmpValue, 1) == 0)
	{
		return 0;
	}
	if(num == PORT_NUM1)
	{
		tmpValue &= (~(PORT1_NO_LOAD_LEVEL_MSK));	//清零高4位
		
		tmpValue |=  (setValue << PORT1_NO_LOAD_LEVEL_POS);
	}
	else if(num == PORT_NUM2)
	{
		tmpValue &= (~(PORT2_NO_LOAD_LEVEL_MSK));	//清零低4位
		
		tmpValue |= (setValue << PORT2_NO_LOAD_LEVEL_POS);
	}

	if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], (u8)SET_NO_LOAD1_REG_110, &tmpValue, 1) == 0)
	{
		return 0;
	}
	return stepResult;
}

static u32 SW3536_Set_NTC_Current(u8 port, u32 setValue)
{
	return 1;	
}

static u32 SW3536_Set_DAC(u8 port, u32 setValue)
{
	return 1;
}

static u32 SW3536_Get_Vin(u8 port, u32 *vinVol)
{
	return 1;
}

static u32 SW3536_Get_Vbus(u8 port, u32 *vbusVol)
{
	return 1;
}
	
static u32 SW3536_Get_Current(u8 port, u8 num,u32 *current)
{
	static u8 tmpValue = 0;
	static u8 dataH = 0;
	static u8 dataL = 0;
	u32 dataValue = 0;
	static u8 stepCnt = 0;
	static u8 stepResult = 0;

	switch(stepCnt)
	{
		default:break;
		case 0:
				if(nowRegHighOrLow[portRegAddr[port]] == 1)
				{
					if(SW3536_Unlock_High_Reg(port, 0) == 0)
					{
						return 0;
					}
					nowRegHighOrLow[portRegAddr[port]] = 0;
					
					return 0;
				}		
				//设置ADC Vout通道
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_CONFIG_REG_40,&tmpValue, 1) == 0)
				{
					return 0;
				}
				tmpValue &= (~(ADC_CH_SEL_MSK));
				if(num == PORT_NUM1)
				{
					tmpValue |= 0x01;//选择通道1，Iout1
				}
				else if(num == PORT_NUM2)
				{
					tmpValue |= 0x02;//选择通道2，Iout2
				}
		
				stepCnt = 1;
				stepResult = 0;
				break;
		case 1:
				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],ADC_CONFIG_REG_40,&tmpValue, 1) == 0)
				{
					return 0;
				}			
				stepCnt = 2;
				stepResult = 0;
				break;
		case 2:
				//获取ADC数据
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DAT_HIGH_REG_42,&dataH, 1) == 0)//获取高位
				{
					return 0;
				}				
				stepCnt = 3;
				stepResult = 0;
				break;
		case 3:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DAT_LOW_REG_41 ,&dataL, 1) == 0)//获取低位
				{
					return 0;
				}	
				dataH &= ADC_DAT_H_MSK;//清除最高bit
				dataValue = ((dataH << 8) | dataL);//组合数据
				*current = ((dataValue * 25) / 10);
			
				stepCnt = 0;
				stepResult = 1;
				break;
	}

	return stepResult;
}
	
static u32 SW3536_Get_NTC_Temperature(u8 port, int *temperature)
{
	return 1;
}

	
static u32 SW3536_Get_Sys_Power(u8 port, u32 *sysPower)
{
	return 1;   
}

static u32 SW3536_Get_Port_State(u8 port, u8 num, u8  *portState)
{
	u8 tmpValue = 0;
	
	if(nowRegHighOrLow[portRegAddr[port]] == 1)
	{
		if(SW3536_Unlock_High_Reg(port, 0) == 0)
		{
			return 0;
		}
		nowRegHighOrLow[portRegAddr[port]] = 0;
		
		return 0;
	}
	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SYSTEM_STA1_REG_0D, &tmpValue, 1) == 0)
	{
		return 0;	
	}
	
	if(num == PORT_NUM1)
	{
		tmpValue &= PORT1_DEVICE_EXIST_STA_MSK;
		*portState = (bool)(tmpValue > 0);
	}
	else if(num == PORT_NUM2)
	{
		tmpValue &= PORT2_DEVICE_EXIST_STA_MSK;
		*portState = (bool)(tmpValue > 0);
	}
	
	return 1;
}

static u32 SW3536_Get_Port_Protocol(u8 port,  u8 *protocol)
{
	u8 tmpValue = 0;

	if(nowRegHighOrLow[portRegAddr[port]] == 1)
	{
		if(SW3536_Unlock_High_Reg(port, 0) == 0)
		{
			return 0;
		}
		nowRegHighOrLow[portRegAddr[port]] = 0;
		
		return 0;
	}
	
	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], QUICK_CHARGE_STATE_REG_09, &tmpValue, 1) == 0)
	{
		return 0;
	}

	tmpValue &= QUICK_CHARGE_PROTOCOL_STA_MSK;

	*protocol = tmpValue;

	return 1;	 
}

static u32 SW3536_On_Off_Emarker(u8 port, u8 setState)
{
	return 1;
}
	
static u32 SW3536_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
 	return 1;
}


static u32 SW3536_On_Off_Buck(u8 port, u8 setState)
{
     return 2;
}

static u32 SW3536_Set_Port_Mode(u8 port, u8 mode)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
	static u8 stepResult = 0;

	if(nowRegHighOrLow[portRegAddr[port]]== 0)
	{
		while(SW3536_Unlock_High_Reg(port, 1) == 0);
		nowRegHighOrLow[portRegAddr[port]] = 1;
	}
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], (u8)SET_DPDM_CUR_LIMIT_REG_114, &tmpValue, 1) == 0);
	tmpValue &= (~(SET_PORT_MODE_MSK));
	tmpValue |= (mode << SET_PORT_MODE_POS);
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], (u8)SET_DPDM_CUR_LIMIT_REG_114, &tmpValue, 1) == 0);

	return stepResult;
}
///初始化时调用的代码，不用分段进行。
static u32 SW3536_I2C_Unlock(u8 port)
{
	u8 tmpValue = 0;
	
	if(nowRegHighOrLow[portRegAddr[port]]== 1)
	{
		while(SW3536_Unlock_High_Reg(port, 0) == 0);
		nowRegHighOrLow[portRegAddr[port]] = 0;
	}
	tmpValue = 0x20;	
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_IIC_REG_10, &tmpValue, 1) == 0);
	tmpValue = 0x40;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_IIC_REG_10, &tmpValue, 1) == 0);
	tmpValue = 0x80;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_IIC_REG_10, &tmpValue, 1) == 0);

	return 1;
}
///初始化时调用的代码，不用分段进行。
static u32 SW3536_Unlock_Force (u8 port)
{
	u8 tmpValue = 0;
	
	if(nowRegHighOrLow[portRegAddr[port]]== 1)
	{
		while(SW3536_Unlock_High_Reg(port, 0) == 0);
		nowRegHighOrLow[portRegAddr[port]] = 0;
	}
	tmpValue = 0x20;	
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_FORCE_OPERATION_REG_15, &tmpValue, 1) == 0);
	tmpValue = 0x40;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_FORCE_OPERATION_REG_15, &tmpValue, 1) == 0);
	tmpValue = 0x80;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_FORCE_OPERATION_REG_15, &tmpValue, 1) == 0);

	return 1;
}

/*
	setState：0-从高地址退回低地址 / 1-从低地址跳到高地址
	注意：使用该函数前，需先使能IIC
*/
static u32 SW3536_Unlock_High_Reg(u8 port, u8 setState)
{
	u8 tmpValue = 0;
	
	if(setState == 0)//跳至低地址
	{
		if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], (u8)SET_REG_LOW_REG_180, &tmpValue, 1) == 0)
		{
			return 0;
		}
	}
	else//跳至高地址
	{
		tmpValue = 0x81;
		if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_IIC_REG_10, &tmpValue, 1) == 0)
		{
			return 0;
		}
	}

 	return 1;
}
	
static u32 SW3536_Init(u8 port)
{
	u8 tmpValue = 0;
	u32 i = 0;
	
	SW3536_I2C_Unlock(port);
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], EN_IIC_REG_10 ,&tmpValue, 1) == 0);
	
	SW3536_Set_Port_Mode(port, 3);//设置单A模式
	
	SW3536_Set_Unload_Current(port, 0, 2);//空载电流第0档，3ma
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], (u8)SET_NO_LOAD0_REG_10F ,&tmpValue, 1) == 0);
	tmpValue &= 0xFC;//最低两位清0
	tmpValue |= 0x03;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], (u8)SET_NO_LOAD0_REG_10F ,&tmpValue, 1) == 0);
	
	return 1;
}

static u32 SW3536_Is_Exist(u8 port, u8 *existValue)
{
	return 1;
}

static u32 SW3536_Set_System_Power(u8 port, u16 power)
{
	return 1;
}

static u32 SW3536_User_Define(u8 port, u8 funcNo, u32 *dat)
{
	return 1;
}


