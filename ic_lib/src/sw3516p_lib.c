#include "sw3516p_func.h"
#include "user_define.h"

extern const struct sw_i2c_drive sw_port_i2c_drive[4];
extern const unsigned char icAddr[4];

static u32 SW3516p_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW3516p_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);
static u32 SW3516p_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 
static u32 SW3516p_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW3516p_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW3516p_On_Off_QC(u8 port,u8 version,u8 setState);
static u32 SW3516p_Set_FCP_XSCP(u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower);
static u32 SW3516p_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState);
static u32 SW3516p_Rebroadcast_PDO(u8 port, u8 way, u32 time);//
static u32 SW3516p_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW3516p_Set_PE(u8 port, u32 maxVol);
static u32 SW3516p_On_Off_PE(u8 port, u8 setState);
static u32 SW3516p_Set_Line_Com (u8 port, u8 setState,  u32 setValue);//
static u32 SW3516p_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW3516p_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW3516p_Set_NTC_Current(u8 port, u32 setValue);//
static u32 SW3516p_Set_DAC(u8 port, u32 setValue);//
static u32 SW3516p_Get_Vin(u8 port, u32 *vinVol);
static u32 SW3516p_Get_Vbus(u8 port, u32 *vbusVol);
static u32 SW3516p_Get_Current(u8 port, u8 num,u32 *current);
static u32 SW3516p_Get_NTC_Temperature(u8 port, int *temperature);//
static u32 SW3516p_Get_Port_State(u8 port, u8 num, u8  *portState);
static u32 SW3516p_Get_Sys_Power(u8 port,u32 *sysPower);
static u32 SW3516p_Get_Port_Protocol(u8 port,  u8 *protocol);
static u32 SW3516p_On_Off_Emarker(u8 port, u8 setState);
static u32 SW3516p_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW3516p_On_Off_Buck(u8 port, u8 setState);
static u32 SW3516p_I2C_Unlock(u8 port);
static u32 SW3516p_Unlock_Force (u8 port);
static u32 SW3516p_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW3516p_Set_Port_Mode(u8 port, u8 mode);

static u32 SW3516p_Init(u8 port);
static u32 SW3516p_Is_Exist(u8 port, u8 *existValue);
static u32 SW3516p_Set_System_Power(u8 port, u16 power);
static u32 SW3516p_User_Define(u8 port, u8 funcNo, u32 *dat);
const struct sw_func SW3516P_Func = 
{

	.SW_Init = SW3516p_Init,	
	.SW_Rebroadcast_PDO = SW3516p_Rebroadcast_PDO,

	.SW_Set_System_Power  = SW3516p_Set_System_Power,
	.SW_User_Define = SW3516p_User_Define,	
	.SW_Set_Line_Com = SW3516p_Set_Line_Com,
	.SW_Set_Current_Limit = SW3516p_Set_Current_Limit,
	.SW_Set_Unload_Current = SW3516p_Set_Unload_Current,
	.SW_Set_NTC_Current = SW3516p_Set_NTC_Current,
	.SW_Set_DAC = SW3516p_Set_DAC,
	
	.SW_Get_Vin = SW3516p_Get_Vin,
	.SW_Get_Vbus = SW3516p_Get_Vbus,
	.SW_Get_Current = SW3516p_Get_Current, 
	.SW_Get_NTC_Temperature = SW3516p_Get_NTC_Temperature,
	.SW_Get_Port_State = SW3516p_Get_Port_State,
	.SW_Get_Sys_Power = SW3516p_Get_Sys_Power,	
	.SW_Get_Port_Protocol = SW3516p_Get_Port_Protocol,
	
//	.SW_On_Off_Emarker = SW3516p_On_Off_Emarker,
//	.SW_Dpdm_Power_Same_PD = SW3516p_Dpdm_Power_Same_PD,
//	.SW_On_Off_Buck = SW3516p_On_Off_Buck,
//	.SW_I2C_Unlock = SW3516p_I2C_Unlock,
//	.SW_Unlock_Force = SW3516p_Unlock_Force,
//	.SW_Unlock_High_Reg = SW3516p_Unlock_High_Reg, 
//	.SW_Set_Port_Mode = SW3516p_Set_Port_Mode,
//	.SW_Set_PD_Gear = SW3516p_Set_PD_Gear, 
//	.SW_Set_PD_Vol_Cur = SW3516p_Set_PD_Vol_Cur,	
//   
//	.SW_Set_UFCS_Gear = SW3516p_Set_UFCS_Gear,	
	
//	.SW_Set_VID_PID = SW3516p_Set_VID_PID,
//	.SW_Set_QC = SW3516p_Set_QC,
//	.SW_On_Off_QC = SW3516p_On_Off_QC,
	
//	.SW_Set_FCP_XSCP = 	SW3516p_Set_FCP_XSCP,
//	.SW_On_Off_FCP_XSCP = SW3516p_On_Off_FCP_XSCP,
//	.SW_On_Off_Charge_Protocol = SW3516p_On_Off_Charge_Protocol,
//	.SW_Set_PE = SW3516p_Set_PE,
//	.SW_On_Off_PE = SW3516p_On_Off_PE,
//	.SW_isExist = SW3516p_Is_Exist,

};

static u32 SW3516p_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],  SET_PD_7_REG_B7, &tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;
		case 1: 
				volGear += 1;

				if(setState)  
				{
					OPEN_PD_GEAR(tmpValue, (volGear));
				}
				else
				{
					CLOSE_PD_GEAR(tmpValue, (volGear));
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],  SET_PD_7_REG_B7, &tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;

				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3516p_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	u8 regAddr = SET_PD_0_REG_B0 + volGear;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], regAddr, &tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;
		case 1:
				tmpValue &= 0x80;;

				tmpValue |=  (cur / 50);

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], regAddr, &tmpValue, 1) == 0 )  
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}



static u32 SW3516p_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState)
{
	return 2;
}


static u32 SW3516p_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{

	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:

				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_PDVID1_REG_BF,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;
		case 1:

				tmpValue = setVID & 0xff;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_PDVID1_REG_BF,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;
				break;
		case 2:

				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_PDVID0_REG_AF,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;
				break;
		case 3:

				tmpValue = (setVID >> 8) & 0xff;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_PDVID0_REG_AF,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3516p_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
	       u8 tmpBit = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:

				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_BA,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;
		case 1:
                if(maxVol <= 9000)
				{
				 tmpBit = 0;
				}
				else if(maxVol <= 12000)
				{
				 tmpBit = 1;
				}
				else if(maxVol <= 20000)
				{
				 tmpBit = 2;
				}
				tmpValue &= ~(MAX_OUT_VOL_MSK); 

				tmpValue |= (tmpBit << MAX_OUT_VOL_POS);

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_BA,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3516p_On_Off_QC(u8 port,u8 version,u8 setState)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	if(version == QC2_PROTOCOL_IC)
	{
		switch(stepCnt)
		{
			case 0:

					if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
					{
						return 0;
					}
					stepCnt = 1;
					returnValue = 0;
					break;
			case 1:

					if(setState)
					{
						tmpValue |= EN_QC_MSK;
					}

					else 
					{
						tmpValue &= ~(EN_QC_MSK);
					}

					if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
					{
						return 0;
					}
					stepCnt = 0;
					returnValue = 1;
					break;
		}
	}

	else if(version == QC3_PROTOCOL_IC)
	{

		switch(stepCnt)
		{
			case 0:
					if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_AA,&tmpValue, 1) == 0 ) 
					{
						return 0;
					}
					stepCnt = 1;
					returnValue = 0;
					break;
			case 1:				
					if(setState)
					{
						tmpValue |= EN_QC3_0_MSK ;
					}
					else
					{
						tmpValue &= ~(EN_QC3_0_MSK);
					}

					if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_AA,&tmpValue, 1) == 0 ) 
					{
						return 0;
					}
					stepCnt = 0;
					returnValue = 1;
					break;
		}
	}
	return returnValue;
}

static u32 SW3516p_Set_FCP_XSCP (u8 port, u8 fcpVersion, u32 maxVol, u32 maxPower)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
	       u8 tmpBit = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{

		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_BA,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;
		case 1:
                if(maxVol <= 9000)
				{
				  tmpBit = 0;
				}
				
				else if(maxVol <= 12000)
				{
				  tmpBit = 1;
				}
				
				else if(maxVol <= 20000)
				{
				  tmpBit = 2;
				}
				tmpValue &= ~(MAX_OUT_VOL_MSK); 

				tmpValue |= (tmpBit << MAX_OUT_VOL_POS);

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_BA,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3516p_On_Off_FCP_XSCP(u8 port, u8 fcpVersion, u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;
		case 1:
				if(setState)
				{
					switch(fcpVersion)
					{

						case(FCP_PROTOCOL_IC): tmpValue |= (1 << EN_FCQ_POS); break;

						case(H_SCP_PROTOCOL_IC): tmpValue |= (1 << EN_SCP_POS); break;
					}
				}

				else 
				{
					switch(fcpVersion)
					{

						case(FCP_PROTOCOL_IC): tmpValue &= ~(0x01 << EN_FCQ_POS); break;

						case(H_SCP_PROTOCOL_IC): tmpValue &= ~(0x01 << EN_SCP_POS); break;
					}
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3516p_Rebroadcast_PDO(u8 port, u8 way, u32 time)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
	case 0:
			if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], PD_SRC_CAP_CMD_TX_REG_73, &tmpValue, 1) == 0 ) 
			{
				return 0;
			}
			tmpValue |= 0x80;

			stepCnt = 1;
			returnValue = 0;
			break;	
	case 1:

			if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], PD_SRC_CAP_CMD_TX_REG_73, &tmpValue, 1) == 0 ) 
			{
				return 0;
			}
			stepCnt = 0;

			returnValue = 1;
			break;	
	}
	return returnValue;	
}

static u32 SW3516p_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
  
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
	case 0:
			if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
			{
				return 0;
			}
			stepCnt = 1;
			returnValue = 0;
			break;	
	case 1:
			if(setState)
			{
				if(num == PORT_NUM1)
				{
					tmpValue |= (1 << EN_P1_QUICK_CHARGE_POS);
				}
				else if(num == PORT_NUM2)
				{
					tmpValue |= (1 << EN_P2_QUICK_CHARGE_POS);
				}
			}
			else
			{
				if(num == PORT_NUM1)
				{
					tmpValue &= ~(EN_P1_QUICK_CHARGE_MSK);	
				}
				else if(num == PORT_NUM2)	
				{
					tmpValue &= ~(EN_P2_QUICK_CHARGE_MSK);
				}
			}		
			if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
			{
				return 0;
			}
			stepCnt = 0;
			returnValue = 1;
			break;	
	}
	return returnValue;
}

static u32 SW3516p_Set_PE(u8 port, u32 maxVol)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	       u8 tmpBit = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_BA,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;	
		case 1:
			    if(maxVol <= 9000)
				{
				  tmpBit = 0;
				}
				
				else if(maxVol <= 12000)
				{
				  tmpBit = 1;
				}
				
				else if(maxVol <= 20000)
				{
				  tmpBit = 2;
				}
				
				tmpValue &= ~(MAX_OUT_VOL_MSK);

				tmpValue |= tmpBit << MAX_OUT_VOL_POS;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_BA,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;
				returnValue =  1;
				break;	
	}
	return returnValue;
}

static u32 SW3516p_On_Off_PE(u8 port, u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
			break;	
		case 1:
				if(setState)
				{
					tmpValue |= EN_PE_MSK;

				}
				else
				{
					tmpValue &= ~(EN_PE_MSK);
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_B9,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;					
				returnValue = 1;
							break;	
	}
	return returnValue;
}

static u32 SW3516p_Set_Line_Com(u8 port,u8 setState, u32 setValue)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{	   
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_PORT_MODE_REG_AB,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
			break;	
		case 1:
				if(setState)
				{
					tmpValue |= EN_LINE_COM_MSK; 
				}
				else
				{
					tmpValue &= ~(EN_LINE_COM_MSK);		
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_PORT_MODE_REG_AB,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
							break;	
	}
	return returnValue;
}

static u32 SW3516p_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{

	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
		   u8 returnValue = 0;
	
	u8 regAddr[2] = {SET_LIMIT_CURRENT_0_REG_BD, SET_LIMIT_CURRENT_1_REG_C4};

	if(num == PORT_NUM1)
	{
		switch(stepCnt)
		{
			case 0:
					if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],regAddr[num],&tmpValue, 1) == 0 ) 
					{
						return 0;
					}
					stepCnt = 1;
					returnValue = 0;
					break;	
			case 1:
					tmpValue &= ~(P1_LITCUR_VAL_2P_EN_MSK);

					tmpValue |= (setValue << P1_LITCUR_VAL_2P_EN_POS) ;	

					if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],regAddr[num],&tmpValue, 1) == 0 ) 	
					{
						return 0;
					}
					stepCnt = 0;
					returnValue = 1;
						break;	
		}
	}

	else if(num == PORT_NUM2)
	{
		switch(stepCnt)
		{
			case 0:	

					if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],regAddr[num],&tmpValue, 1) == 0 ) 
					{
						return 0;
					}
					stepCnt = 1;
					returnValue = 0;
					break;	
			case 1:
					tmpValue &=  ~(EN_P2_LIT_CUR_2P_ONLINE_MSK);

					tmpValue &= ~(P2_LITCUR_VAL_MSK);

					tmpValue |= (setValue << P2_LITCUR_VAL_POS);

					if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],regAddr[num],&tmpValue, 1) == 0 ) 	
					{
						return 0;
					}
					stepCnt = 0;
					returnValue = 1;
					break;
		}
	}
	return returnValue;
}

static u32 SW3516p_Set_Unload_Current(u8 port, u8 num, u32 setValue)
{
	return 2;
}

static u32 SW3516p_Set_NTC_Current(u8 port, u32 setValue)
{
	return 2;
}

static u32 SW3516p_Set_DAC(u8 port, u32 setValue)
{
	return 2;
}

static u32 SW3516p_Get_Vin(u8 port, u32 *vinVol)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;

	static u8 dataH8 = 0;

	u32 dataValue = 0;

	u8 dataL4 = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],EN_ADC_VIN_REG_13,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
					break;
		case 1:
				tmpValue |= EN_VIN_ADC_MSK;	

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],EN_ADC_VIN_REG_13,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;
				break;
		case 2:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_ADC_REG_3A,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;
				break;
		case 3:
				tmpValue &= ~(ADC_DATA_SEL_MSK);

				tmpValue |= 1;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_ADC_REG_3A,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 4;
				returnValue = 0;
				break;
		case 4:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_H8_REG_3B,&dataH8, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 5;
				returnValue = 0;
				break;
		case 5:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_L4_REG_3C,&dataL4, 1) == 0 ) 
				{
					return 0;
				}
				dataValue |= dataH8;

				dataValue <<= 4;

				dataValue |= dataL4;

				*vinVol = dataValue*10;

				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3516p_Get_Vbus(u8 port, u32 *vbusVol)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;

	static u8 dataH8 = 0;

	u32 dataValue = 0;

	u8 dataL4 = 0;
		   u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_ADC_REG_3A,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
					break;
		case 1:
				tmpValue &= ~(ADC_DATA_SEL_MSK);

				tmpValue |= 2;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_ADC_REG_3A,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;
					break;
		case 2:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_H8_REG_3B,&dataH8, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;
					break;
		case 3:

				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_L4_REG_3C,&dataL4, 1) == 0 ) 
				{
					return 0;
				}
				dataValue |= dataH8;

				dataValue <<= 4;

				dataValue |= dataL4;

				*vbusVol = dataValue*6;

				stepCnt = 0;

				returnValue = 1;
				break;
	}
	return returnValue;
}
	
static u32 SW3516p_Get_Current(u8 port, u8 num,u32 *current)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;

	static u8 dataH8 = 0;

	u32 dataValue = 0;

	u8 dataL4 = 0;
	
	u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_ADC_REG_3A ,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 1;

				returnValue = 0;
				break;
		case 1:
				tmpValue &= ~(ADC_DATA_SEL_MSK);

				if(num == PORT_NUM1)
				{
				tmpValue |= 3;
				}

				else if(num == PORT_NUM2)
				{
				tmpValue |= 4;
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_ADC_REG_3A ,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;
				break;
		case 2:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_H8_REG_3B,&dataH8, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;
				break;
		case 3:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_L4_REG_3C,&dataL4, 1) == 0 ) 
				{
					return 0;
				}
				dataValue |= dataH8;

				dataValue <<= 4;

				dataValue |= dataL4;

				*current = dataValue*2500;
		        *current /= 1000;		
		
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}
	
static u32 SW3516p_Get_NTC_Temperature(u8 port, int *temperature)
{
	static u8 tmpValue = 0;

	static u8 dataH8 = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	u8 dataL4 = 0;

	u32 dataValue = 0;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_ADC_REG_3A ,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}				
				stepCnt = 1;
				returnValue = 0;
				break;

		case 1:
				tmpValue &= ~(ADC_DATA_SEL_MSK);

				tmpValue |= 6;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_ADC_REG_3A ,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;
				break;

		case 2:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_H8_REG_3B,&dataH8, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;
				break;

		case 3:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],ADC_DATA_L4_REG_3C,&dataL4, 1) == 0 ) 
				{
					return 0;
				}
				dataValue |= dataH8;
				dataValue <<= 4;
				dataValue |= dataL4;
				dataValue = (dataValue*0.5/100)-2;//kohm(ntc)
				
				dataValue *= 1000;//实际电阻值
				
				*temperature = dataValue;
		
				stepCnt = 0;
				returnValue = 1;
				break;
	}

	return returnValue;
}

static u32 SW3516p_Get_Port_State(u8 port, u8 num, u8  *portState)
{
	u8 tmpValue = 0;
	
	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SYS1_STA_REG_08 ,&tmpValue, 1) == 0 ) 
	{
		return 0;
	}
	tmpValue &=  PORT_SINK_STA_MSK;

	tmpValue  >>= PORT_SINK_STA_POS;
	
	if(num ==  PORT_NUM1)
	{
		if(tmpValue == 1 || tmpValue == 3 || tmpValue == 5|| tmpValue == 7 ||	tmpValue == 9 || tmpValue == 10)
		{
			*portState = 0;
		}
		else if(tmpValue == 2 || tmpValue == 4 || tmpValue == 6 || tmpValue == 8 || tmpValue  == 11)
		{
			*portState = 1;
		}
	}		

	else if(num == PORT_NUM2)
	{
		if(tmpValue == 1 || tmpValue == 2 || tmpValue == 5 || tmpValue == 6  || tmpValue == 10 || tmpValue == 11 )
		{
			*portState = 0;
		}
		else if(tmpValue == 3 || tmpValue ==4 || tmpValue == 7 || tmpValue == 8 || tmpValue == 9)
		{
			*portState = 1;  
		}
	}	
	return 1;
}
	
static u32 SW3516p_Get_Sys_Power(u8 port,u32 *sysPower)
{
	u8 tmpValue = 0;

	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_PD_4_REG_B4 ,&tmpValue, 1) == 0 ) 
	{
		return 0;
	}
	tmpValue &= FIX_20V_PDO_CUR_MSK;

	*sysPower = tmpValue*50*20; 
	
	return 1;	
}

static u32 SW3516p_Get_Port_Protocol(u8 port,  u8 *protocol)
{
	u8 tmpValue = 0;
	
	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],QUICK_CHARGE_STA_REG_06 ,&tmpValue, 1) == 0 )
	{
		return 0;
	}		
	*protocol = tmpValue & QUICK_CHARGE_PROTOCOL_MSK; 	
	return 1;	
}

static u32 SW3516p_On_Off_Emarker(u8 port, u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
		   u8 returnValue = 0;

	switch(stepCnt)
	{ 
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_PD_7_REG_B7 ,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
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
				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_PD_7_REG_B7 ,&tmpValue, 1) == 0 ) 
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
					break;
	}
	return returnValue;
}
	
static u32 SW3516p_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
	return 2;
}

static u32 SW3516p_On_Off_Buck(u8 port, u8 setState)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],PWR_FORCE0_REG_16 ,&tmpValue, 1) == 0)
				{
					return 0;
				}

				stepCnt = 1;
				returnValue = 0;
					break;
		case 1:
				if(setState)
				{
					tmpValue |= FORCE_OPEN_BUCK_MSK; 
				}
				else 
				{
					tmpValue |= FORCE_CLOSE_BUCK_MSK;
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],PWR_FORCE0_REG_16 ,&tmpValue, 1) == 0) 
				{
					return 0;
				}
				stepCnt = 0;

				returnValue = 1;
					break;				
	}
	return returnValue;
}

static u32 SW3516p_I2C_Unlock(u8 port)
{
	u8 tmpValue = 0;
	
	tmpValue = 0x20;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_CTRL_REG_12 ,&tmpValue, 1) == 0);
	tmpValue = 0x40;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_CTRL_REG_12 ,&tmpValue, 1) == 0);
	tmpValue = 0x80;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_CTRL_REG_12 ,&tmpValue, 1) == 0);

	return 1;
}

static u32 SW3516p_Unlock_Force (u8 port)
{
	u8 tmpValue = 0;
	
	tmpValue = 0x20;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_PWR_FORCE_REG_15 ,&tmpValue, 1) == 0);
	tmpValue = 0x40;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_PWR_FORCE_REG_15 ,&tmpValue, 1) == 0);
	tmpValue = 0x80;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_PWR_FORCE_REG_15 ,&tmpValue, 1) == 0);

	return 1;
}

static u32 SW3516p_Unlock_High_Reg(u8 port, u8 setState)
{
	return 2;
}

static u32 SW3516p_Set_Port_Mode(u8 port, u8 mode)
{
	u8 tmpValue = 0;

	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_PORT_MODE_REG_AB ,&tmpValue, 1) == 0);

	tmpValue &= ~(SET_CHIP_PORT_MSK);

	tmpValue |= (mode << SET_CHIP_PORT_POS);

	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_PORT_MODE_REG_AB ,&tmpValue, 1) == 0);

	return 1;
}



static u32 SW3516p_Init(u8 port)
{
	u8 tmpValue = 0;
	u32 i = 0;
	
	SW3516p_I2C_Unlock(port);
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], EN_I2C_CTRL_REG  ,&tmpValue, 1) == 0);
	
	tmpValue = 0x01;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_LINK_REG_76  ,&tmpValue, 1) == 0); //断开CC
	
	tmpValue = 0x9E;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PD_4_REG_B4 ,&tmpValue, 1) == 0); //修改功率30W
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_PD_7_REG_B7  ,&tmpValue, 1) == 0); //设置功率由寄存器设定，禁止PPS1,PPS0
	tmpValue &= 0x3D;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PD_7_REG_B7 ,&tmpValue, 1) == 0);
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_LIMIT_CURRENT_1_REG_C4  ,&tmpValue, 1)== 0); //读取限流值设置寄存器
	tmpValue &= (~SET_LITCUR_FIX_OFFSET_MSK); //设置限流offset为150ma
	tmpValue |=  (EN_P2_LIT_CUR_2P_ONLINE_MSK ); //设置一个2口的限流值同1的限流值一样。
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_LIMIT_CURRENT_1_REG_C4  ,&tmpValue, 1)== 0); //修改限流值为450ma
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_LIMIT_CURRENT_0_REG_BD  ,&tmpValue, 1) == 0); //设置限流值都是2.6A
	tmpValue &= 0xCF;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_LIMIT_CURRENT_0_REG_BD ,&tmpValue, 1) == 0);

	tmpValue = 0x00;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_LINK_REG_76  ,&tmpValue, 1) == 0); //打开CC
	return 1;
}

static u32 SW3516p_Is_Exist(u8 port, u8 *existValue)
{
	u8 tmpValue = 0;
	
	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], EN_I2C_CTRL_REG_12 ,&tmpValue, 1) == 0)
	{
		return 0;
	}
	
	if(tmpValue == 0x80)
	{
		*existValue = 1;
	}
	else
	{
		*existValue = 0;
	}
	
	return 1;
}

static u32 SW3516p_Set_System_Power(u8 port, u16 power)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 0;
		   u32 current = power * 5;//功率是10倍传进去的。

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_PD_4_REG_B4 ,&tmpValue, 1) == 0)
				{
					return 0;
				}

				stepCnt = 1;
				returnValue = 0;
				break;
		case 1:
				tmpValue &= 0x80;
					
				tmpValue |=  (current / 50);

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PD_4_REG_B4 ,&tmpValue, 1) == 0)
				{
					return 0;
				}


				stepCnt = 2;
				returnValue = 0;
				break;
		case 2:
				tmpValue &= 0x7F;
				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_PD_5_REG_B5 ,&tmpValue, 1)== 0) //写PPS0电流
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3516p_User_Define(u8 port, u8 funcNo, u32 *dat)
{
	static u8 tmpValue = 0;
	static u8 stepCnt = 0;
		   u8 returnValue = 1;

	if(funcNo == 1) //修改最高电压为21V
	{
		while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_LINK_REG_76 ,&tmpValue, 1) == 0);//打开CC
		returnValue = 1;
	}
 
	return returnValue;
}

