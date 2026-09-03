#include "sw3526_func.h"
#include "user_define.h"

extern const struct sw_i2c_drive sw_port_i2c_drive[4];
extern const unsigned char icAddr[4];

static u32 SW3526_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState);
static u32 SW3526_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur);//

static u32 SW3526_Set_UFCS_Gear(u8 port, u8 *pwrAddr, u8 pos, u8 setState); 

static u32 SW3526_Set_VID_PID(u8 port,u32 setVID,u32 setPID);
static u32 SW3526_Set_QC(u8 port,u8 qcVersion,u32 maxVol,u32 maxPower);
static u32 SW3526_On_Off_QC(u8 port,u8 version,u8 setState);
static u32 SW3526_Set_FCP_XSCP(u8 port,u8 protVersion,u32 maxVol,u32 maxPower);
static u32 SW3526_On_Off_FCP_XSCP(u8 port,u8 protVersion,u8 setState);//fcp:3  scp:4  sscp:a
static u32 SW3526_Rebroadcast_PDO(u8 port, u8 way, u32 time);
static u32 SW3526_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState);
static u32 SW3526_Set_PE(u8 port,u32 maxVol);
static u32 SW3526_Set_On_Off_PE(u8 port, u8 setState);
static u32 SW3526_Set_Line_Com(u8 port,u8 setState,u32 setValue);
static u32 SW3526_Set_Current_Limit(u8 port, u8 num,u32 setValue);
static u32 SW3526_Set_Unload_Current(u8 port, u8 num, u32 setValue);//
static u32 SW3526_Set_NTC_Current(u8 port,u32 setValue);//
static u32 SW3526_Set_DAC(u8 port,u32 setValue);//
static u32 SW3526_Get_Vin(u8 port,u32 *vinVol);
static u32 SW3526_Get_Vbus(u8 port,u32 *vbusVol);
static u32 SW3526_Get_Current(u8 port, u8 num,u32 *current);
static u32 SW3526_Get_NTC_Temperature(u8 port, int *getTemperature);//
static u32 SW3526_Get_Port_State(u8 port, u8 num, u8  *portState);//
static u32 SW3526_Get_Sys_Power(u8 port,u32 *sysPower);
static u32 SW3526_Get_Port_Protocol(u8 port, u8 *protocol);
static u32 SW3526_On_Off_Emarker(u8 port, u8 setState);//

static u32 SW3526_Dpdm_Power_Same_PD(u8 port,u8 setState);//
static u32 SW3526_On_Off_Buck(u8 port, u8 setState);//
static u32 SW3526_I2C_Unlock(u8 port);
static u32 SW3526_Unlock_Force (u8 port);//
static u32 SW3526_Unlock_High_Reg(u8 port, u8 setState);//
static u32 SW3526_Set_Port_Mode(u8 port, u8 mode);//

static u32 SW3526_Init(u8 port);
static u32 SW3526_Is_Exist(u8 port, u8 *existValue);
static u32 SW3526_Set_System_Power(u8 port, u16 power);
static u32 SW3526_User_Define(u8 port, u8 funcNo, u32 *dat);

const struct sw_func SW3526_Func = {
 
	.SW_Init = SW3526_Init,	
	.SW_Rebroadcast_PDO = SW3526_Rebroadcast_PDO,

	.SW_Set_System_Power  = SW3526_Set_System_Power,
	.SW_User_Define = SW3526_User_Define,	
	.SW_Set_Line_Com = SW3526_Set_Line_Com,
	.SW_Set_Current_Limit = SW3526_Set_Current_Limit,
	.SW_Set_Unload_Current = SW3526_Set_Unload_Current,
	.SW_Set_NTC_Current = SW3526_Set_NTC_Current,
	.SW_Set_DAC = SW3526_Set_DAC,
	
	.SW_Get_Vin = SW3526_Get_Vin,
	.SW_Get_Vbus = SW3526_Get_Vbus,
	.SW_Get_Current = SW3526_Get_Current, 
	.SW_Get_NTC_Temperature = SW3526_Get_NTC_Temperature,
	.SW_Get_Port_State = SW3526_Get_Port_State,
	.SW_Get_Sys_Power = SW3526_Get_Sys_Power,	
	.SW_Get_Port_Protocol = SW3526_Get_Port_Protocol,
	
//	.SW_On_Off_Emarker = SW3526_On_Off_Emarker,
//	.SW_Dpdm_Power_Same_PD = SW3526_Dpdm_Power_Same_PD,
//	.SW_On_Off_Buck = SW3526_On_Off_Buck,
//	.SW_I2C_Unlock = SW3526_I2C_Unlock,
//	.SW_Unlock_Force = SW3526_Unlock_Force,
//	.SW_Unlock_High_Reg = SW3526_Unlock_High_Reg, 
//	.SW_Set_Port_Mode = SW3526_Set_Port_Mode,
//	.SW_Set_PD_Gear = SW3526_Set_PD_Gear, 
//	.SW_Set_PD_Vol_Cur = SW3526_Set_PD_Vol_Cur,	
//   
//	.SW_Set_UFCS_Gear = SW3526_Set_UFCS_Gear,	
	
//	.SW_Set_VID_PID = SW3526_Set_VID_PID,
//	.SW_Set_QC = SW3526_Set_QC,
//	.SW_On_Off_QC = SW3526_On_Off_QC,
	
//	.SW_Set_FCP_XSCP = 	SW3526_Set_FCP_XSCP,
//	.SW_On_Off_FCP_XSCP = SW3526_On_Off_FCP_XSCP,
//	.SW_On_Off_Charge_Protocol = SW3526_On_Off_Charge_Protocol,
//	.SW_Set_PE = SW3526_Set_PE,
//	.SW_On_Off_PE = SW3526_On_Off_PE,
//	.SW_isExist = SW3526_Is_Exist,
};

static u32 SW3526_Set_PD_Gear(u8 port,  u8 *pwrAddr, u8 volGear,  u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE1_REG_A9, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;
				break;

		case 1:
				if(setState)  
				{
					OPEN_PD_GEAR(tmpValue, (volGear + 1));
				}
				else
				{
					CLOSE_PD_GEAR(tmpValue, (volGear + 1));
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE1_REG_A9,  &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;		
				break;
	}
	return returnValue;
}

static u32 SW3526_Set_PD_Vol_Cur(u8 port,  u8 volGear,  u32 cur)
{

	return 1;
}

static u32 SW3526_Set_UFCS_Gear(u8 port,u8 *pwrAddr,u8 pos,u8 setState)
{
	return 2;
}
	


static u32 SW3526_Set_VID_PID(u8 port,u32 setVID,u32 setPID)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	switch(stepCnt)
	{

		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_VID_0_REG_AE,&tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;	 
				break;

		case 1:
				tmpValue = setVID & 0xff;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_VID_0_REG_AE, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;	 
				break;

		case 2:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_VID_1_REG_AF, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;	 
				break;

		case 3:
				tmpValue = (setPID >>8) & 0xff;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_VID_1_REG_AF, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;	 
				break;
	}
	return returnValue;
}

static u32 SW3526_Set_QC(u8 port, u8 qcVersion, u32 maxVol, u32 maxPower)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 tmpBit = 0;
	u8 tmpByte = 0;
	u8 returnValue = 0;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_AA, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;		
				break;

		case 1:
			   if(maxVol <= 9000)
			   {
			     tmpBit =  1;
			   }
			   
			   else if(maxVol <= 12000)
			   {
			     tmpBit = 2;
			   }
			   
			   else if(maxVol <= 20000)
			   {
			     tmpBit = 3;
			   }
			   
				tmpValue &= ~(MAX_OUT_VOL_EXCEPT_PD_MSK); 

				tmpValue |= (tmpBit << MAX_OUT_VOL_EXCEPT_PD_POS);

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_AA, &tmpValue, 1) == 0 )//set_maxVol
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;		 
				break;

		case 2:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_AB, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;		 
				break;

		case 3:
				tmpValue &= ~(POWER_SETTING_METHOD_MSK);

				tmpValue |= POWER_SETTING_METHOD_MSK;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_AB, &tmpValue, 1) == 0 )//enable
				{
					return 0;
				}
				stepCnt = 4;
				returnValue = 0;		 
				break;

		case 4:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_POWER_REG_A7, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 5;
				returnValue = 0;			
				break;

		case 5:
				tmpValue &= ~(SETTING_POWER_MSK);
		
		        if(maxPower >= 12 && maxPower <= 63)
				{
				 tmpByte = maxPower;
				}
				
				else if(maxPower >= 64 && maxPower <= 71)
				{
				 tmpByte = maxPower - 64;
				}

				tmpValue |= tmpByte;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_POWER_REG_A7, &tmpValue, 1) == 0 )//set_power
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;			
				break;
	}

	return returnValue;
}

static u32 SW3526_On_Off_QC(u8 port,u8 version,u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	if(version == QC2_PROTOCOL_IC)
	{
		switch(stepCnt)
		{
			case 0:
					if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8,&tmpValue, 1) == 0 )
					{
						return 0;
					}
					stepCnt = 1;
					returnValue = 0;		
					break;

			case 1:
					if(setState)
					{
						tmpValue &= ~(EN_QC2_0_MSK);
					}

					else 
					{
						tmpValue |= EN_QC2_0_MSK;
					}

					if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8, &tmpValue, 1) == 0 )
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
					if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8, &tmpValue, 1) == 0 )
					{
						return 0;
					}
					stepCnt = 1;
					returnValue = 0;	
					break;

			case 1:
					if(setState)
					{
						tmpValue &= ~(EN_QC3_0_MSK) ;
					}
					else
					{
						tmpValue |= EN_QC3_0_MSK;
					}

					if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8, &tmpValue, 1) == 0 )
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

static u32 SW3526_Set_FCP_XSCP(u8 port,u8 protVersion,u32 maxVol, u32 maxPower)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 tmpBit = 0;
	u8 tmpByte = 0;
	u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_AA,&tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;	 
				break;

		case 1:		
                if(maxVol <= 9000)
				{
				  tmpBit = 1;
				}
       
                else if(maxVol <= 12000)
				{
				  tmpBit = 2;
				}			

                else if(maxVol <= 20000)
				{
				  tmpBit = 3;
				}					
				tmpValue &= ~(MAX_OUT_VOL_EXCEPT_PD_MSK); 

				tmpValue |= (tmpBit << MAX_OUT_VOL_EXCEPT_PD_POS);

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_AA, &tmpValue, 1) == 0 )//set_maxVol
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;			   
				break;

		case 2:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_AB, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 3;
				returnValue = 0;		 
				break;

		case 3:
				tmpValue &= ~(POWER_SETTING_METHOD_MSK);

				tmpValue |= POWER_SETTING_METHOD_MSK;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_3_REG_AB, &tmpValue, 1) == 0 )//enable
				{
					return 0;
				}
				stepCnt = 4;
				returnValue = 0;	 
				break;

		case 4:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_POWER_REG_A7,&tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 5;
				returnValue = 0;	 
				break;

		case 5:
			    if(maxPower >= 12 && maxPower<=63)
				{
				  tmpByte = maxPower;
				}
				
				else if(maxPower >=64 && maxPower <= 71)
				{
				  tmpByte = maxPower - 64;
				}
				tmpValue &= ~(SETTING_POWER_MSK);

				tmpValue |= tmpByte;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_POWER_REG_A7, &tmpValue, 1) == 0 )//set_power
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;	 
				break;
	}
	return returnValue;
}

static u32 SW3526_On_Off_FCP_XSCP(u8 port, u8 protVersion, u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;	 
				break;

		case 1:
				if(setState)
				{
					switch(protVersion)
					{

						case(FCP_PROTOCOL_IC): tmpValue &= ~(EN_FCP_MSK); break;

						case(H_SCP_PROTOCOL_IC): tmpValue &= ~(EN_SCP_MSK); break;

						case(SFCP_PROTOCOL_IC): tmpValue &= ~(EN_SFCP_MSK);break;
					}
				}

				else 
				{
					switch(protVersion)
					{

						case(FCP_PROTOCOL_IC): tmpValue |= EN_FCP_MSK; break;

						case(H_SCP_PROTOCOL_IC): tmpValue |= EN_SCP_MSK; break;

						case(SFCP_PROTOCOL_IC): tmpValue |= EN_SFCP_MSK;break;
					}
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;	
				break;
	}
	return returnValue;
}

static u32 SW3526_Rebroadcast_PDO(u8 port, u8 way, u32 time)
{
   
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], PD_SRC_CHANGE_REG_73 , &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;		
				break;

		case 1:
				tmpValue |= PD_SRC_CHANGE_MSK;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], PD_SRC_CHANGE_REG_73, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;	
				break;
	}
	return returnValue;
}

static u32 SW3526_On_Off_Charge_Protocol(u8 port,u8 num,u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_4_REG_AC,&tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;		
				break;

		case 1:	
				if(setState)
				{
					tmpValue &= ~(EN_PORT_QUICK_CHARGE_MSK);			
				}

				else 
				{		
					tmpValue |= EN_PORT_QUICK_CHARGE_MSK;
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_4_REG_AC, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;			
				break;
	}

	return returnValue;
}

static u32 SW3526_Set_PE(u8 port, u32 maxVol)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 tmpBit = 0;
	u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_AA, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;	
				break;

		case 1:
			    if(maxVol <= 9000)
				{
				  tmpBit = 1;
				}
				
				else if(maxVol <= 12000)
				{
				  tmpBit = 2;
				}
				
				else if(maxVol <= 20000)
				{
				  tmpBit = 3;
				}
				tmpValue &= ~(MAX_OUT_VOL_EXCEPT_PD_MSK);

				tmpValue |= tmpBit << MAX_OUT_VOL_EXCEPT_PD_POS;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_2_REG_AA, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;		
				break;
	}

	return returnValue;
}

static u32 SW3526_Set_On_Off_PE(u8 port, u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;	
				break;

		case  1:
				tmpValue &= ~(EN_PE_MSK);

				if(setState)
				{
					tmpValue &= ~(EN_PE_MSK); 		
				}
				else 
				{
					tmpValue |= EN_PE_MSK; 
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_QUICK_CHARGE_0_REG_A8, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue =1;
				break;
	}
	return returnValue;
}

static u32 SW3526_Set_Line_Com(u8 port,u8 setState,u32 setValue)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;
	
	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],BUCK_FORCE_OFF_REG_13, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;	
				break;

		case 1:
				if(setState)
				{
					tmpValue &= ~(EN_LINE_COMOENSATION_MSK); 
				}
				else 
				{
					tmpValue |= EN_LINE_COMOENSATION_MSK;
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],BUCK_FORCE_OFF_REG_13, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 2;
				returnValue = 0;	 
				break;

		case 2:
				if(setState)
				{
					if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SET_LINE_COMPENSATION_REG_A6, &tmpValue, 1) == 0 )
					{
						return 0;
					}
					stepCnt = 3;
					returnValue = 0;	
				}
				else 
				{
					stepCnt = 0;
					returnValue = 1;
					
				}
				break;
		case 3:

				tmpValue &= ~(LINE_DROP_COMPENSATION_SET_MSK);

				tmpValue |= setValue << LINE_DROP_COMPENSATION_SET_POS;	

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_LINE_COMPENSATION_REG_A6, &tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;		
				break;
	}
	return returnValue;
}

static u32 SW3526_Set_Current_Limit(u8 port, u8 num, u32 setValue)
{
	return 2;
}

static u32 SW3526_Set_Unload_Current(u8 port,u8 num, u32 setValue)//
{
	return 2;
}
      
static u32 SW3526_Set_NTC_Current(u8 port,u32 setValue)
{
	return 2;
}

static u32 SW3526_Set_DAC(u8 port,u32 setValue)
{
	return 2;
}

static u32 SW3526_Get_Vin(u8 port,u32 *vinVol)
{

	static u8 tmpValue = 0;

	static u8 stepCnt = 0;

	static u8 dataH8 = 0;
	u8 returnValue = 0;
	u8  dataL4 = 0;	

	u32 dataValue = 0;

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
				tmpValue &= ~(ADC_DATA_TYPE_MSK);

				tmpValue |= 1;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_ADC_REG_3A, &tmpValue, 1) == 0 )
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
				dataValue |=  dataH8;

				dataValue <<= 4;

				dataValue |= dataL4;

				*vinVol = dataValue*16*10;

				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3526_Get_Vbus(u8 port,u32 *vbusVol)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	static u8 dataH8 = 0;

	u8  dataL4 = 0;	

	u32 dataValue = 0;

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
				tmpValue &= ~(ADC_DATA_TYPE_MSK);

				tmpValue |= 2;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_ADC_REG_3A, &tmpValue, 1) == 0 )
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
				dataValue |=  dataH8;

				dataValue <<= 4;

				dataValue |= dataL4;

				*vbusVol = dataValue*16*6;

				stepCnt = 0;
				returnValue = 1;	
				break;
	}
	return returnValue;
}

static u32 SW3526_Get_Current(u8 port, u8 num,u32 *current)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	static u8 dataH8 = 0;

	u8  dataL4 = 0;	

	u32 dataValue = 0;

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
				tmpValue &= ~(ADC_DATA_TYPE_MSK);

				tmpValue |= 3;

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],SET_ADC_REG_3A, &tmpValue, 1) == 0 )
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
				dataValue |=  dataH8;

				dataValue <<= 4;

				dataValue |= dataL4;

				*current = dataValue*2.5;

				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3526_Get_NTC_Temperature(u8 port, int *getTemperature)
{
	return 2;
}

u32 SW3526_Get_Port_State(u8 port, u8 num, u8 *portState)
{
	u8 tmpValue = 0;
	static u8 stepCnt    = 0;
	static u8 onLineCnt  = 0;
	static u8 offLineCnt = 0;
	static u8 sumCnt     = 0;
	u32 cur = 0;
	
	if(num == 0)
	{
		if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],SYS_STA_REG_07,&tmpValue, 1) == 0 )
		{
			return 0;
		}
		tmpValue &= PORT_STA_MSK;

		*portState = tmpValue >> 1;	
		
		return 1;
	}
	else
	{
		if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], ADC_IOUT_DATA_REG_33, &tmpValue, 1) == 0 )
		{
			return 0;
		}
		cur = tmpValue * 40;
		
		sumCnt    += 1;
		
		if(cur > 80)
		{
			onLineCnt ++;
			offLineCnt = 0;
		}
		else if(cur < 40)
		{
			offLineCnt ++;
			onLineCnt = 0;					
		}
		
		if(onLineCnt > 2)
		{
			onLineCnt = 0;
			offLineCnt = 0;
			sumCnt    = 0;
			
			*portState = 1;
			return 1;
		}
		else if(offLineCnt > 2)
		{
			onLineCnt = 0;
			offLineCnt = 0;
			sumCnt    = 0;
			
			*portState = 0;
			return 1;
		}
		
		if(sumCnt > 3)
		{
			sumCnt    = 0;
			onLineCnt = 0;
			offLineCnt = 0;

			return 1;
		}
	}
	return 0;
}

static u32 SW3526_Get_Sys_Power(u8 port,u32 *sysPower)
{
	u8 tmpValue = 0;

	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],POWER_STA_REG_68,&tmpValue, 1) == 0 )
	{
		return 0;
	}
	tmpValue &= POWER_STA_MSK;

	*sysPower = tmpValue*1000;
	
	return 1;
}

static u32 SW3526_Get_Port_Protocol(u8 port, u8 *protocol)
{
	u8 tmpValue = 0;

	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],PROTOCOL_INDICATION_REG_06,&tmpValue, 1) == 0 )
	{
		return 0;
	}
	*protocol = tmpValue & PROTOCOL_INDICATION_MSK; 

	return 1;
}

static u32 SW3526_On_Off_Emarker(u8 port, u8 setState)
{
	return 2;
}

static u32 SW3526_Dpdm_Power_Same_PD(u8 port,u8 setState)
{
	return 2;
}

static u32 SW3526_On_Off_Buck(u8 port, u8 setState)
{
	static u8 tmpValue = 0;

	static u8 stepCnt = 0;
	u8 returnValue = 0;

	switch(stepCnt)
	{
		case 0:
				if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],BUCK_FORCE_OFF_REG_13,&tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 1;
				returnValue = 0;		
				break;

		case 1:
				tmpValue &= ~(BUCK_FORCE_TURNOFF_1S_MSK);

				if(setState == 0)
				{
					tmpValue |= BUCK_FORCE_TURNOFF_1S_MSK; 
				}

				if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port],BUCK_FORCE_OFF_REG_13,&tmpValue, 1) == 0 )
				{
					return 0;
				}
				stepCnt = 0;
				returnValue = 1;
				break;
	}
	return returnValue;
}

static u32 SW3526_I2C_Unlock(u8 port)
{
	u8 tmpValue = 0;
	
	tmpValue = 0x20;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_REG_12 ,&tmpValue, 1) == 0);
	tmpValue = 0x40;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_REG_12 ,&tmpValue, 1) == 0);
	tmpValue = 0x80;
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], EN_I2C_REG_12 ,&tmpValue, 1) == 0);

	return 1;
}


static u32 SW3526_Unlock_Force (u8 port)
{
	return 2;
}

static u32 SW3526_Unlock_High_Reg(u8 port, u8 setState)
{
	return  2;
}

static u32 SW3526_Set_Port_Mode(u8 port, u8 mode)
{
	return 2;
}

static u32 SW3526_Init(u8 port)
{
	u8 tmpValue = 0;
	
	SW3526_I2C_Unlock(port);

	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], EN_I2C_REG ,&tmpValue, 1) == 0);
	
	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_QUICK_CHARGE_3_REG_AB, &tmpValue, 1) == 0);
	tmpValue |= (1 << 2);//置位第2位，使能寄存器设置功率	
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_QUICK_CHARGE_3_REG_AB , &tmpValue, 1) == 0);


	while(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], SET_QUICK_CHARGE_6_REG_A2, &tmpValue, 1) == 0);
	tmpValue &= (~(1 << 6));//清零第6位，使能QC2/QC3的线补和offset
	while(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_QUICK_CHARGE_6_REG_A2 , &tmpValue, 1) == 0);
	
	//根据不同的端口设置不同的功率
	if(port == 3)
	{
		while(SW3526_Set_System_Power(port, 450) == 0);
	}
	else if(port == 4)
	{
		while(SW3526_Set_System_Power(port, 240) == 0);
	}
	
	return 1;
}

static u32 SW3526_Is_Exist(u8 port, u8 *existValue)
{
	u8 tmpValue = 0;
	
	if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port], EN_I2C_REG ,&tmpValue, 1) == 0 )
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

static u32 SW3526_Set_System_Power(u8 port, u16 power)
{
	u8 tmpValue = 0;
	
	if(power > 710) 
	{
		return 3;
	}

	power /= 10;

	
	if(power >= 64)
	{
		power -=  64; 
	}
	
	tmpValue = power;
	if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], SET_POWER_REG_A7, &tmpValue, 1) == 0 ) 
	{
		return 0;
	}	
	return 1;
}

static u32 SW3526_User_Define(u8 port, u8 funcNo, u32 *dat)
{
//	static u8 i = 12;
//	static u8 tmpValue = 0;
//	static u8 stepCnt = 0;
// 
//	if(stepCnt == 0)
//	{
//		if(sw_port_i2c_drive[port].i2c_write_byte(icAddr[port], 0x01, i) == 0)
//		{
//			return 0;
//		}
//		stepCnt = 1;
//	}
//	else if(stepCnt == 1)	
//	{
//		if(sw_port_i2c_drive[port].i2c_read_byte(icAddr[port],  0x01, &tmpValue) == 0 )
//		{
//			return 0;
//		}
//		i +=1;
//		stepCnt = 0;
//	}
// 
	
	return 1;
}


