#include "sw2325_i2c.h"
#include "ic_lib_rely.h"
#include "math.h"
#include "sw3566_i2c_lib.h"
#include "device_drive.h" 

typedef enum{
	IIC_BUSY,
	IIC_IDEL,
	IIC_ERROR
}_IIC_STATU;

typedef struct
{
	_IIC_STATU state;
	u8 data;
}_IIC_DATA;

static u32 Res_To_Temperture(float ntcBvalue, float ntcR25, float ntcRes);

const u8 portNumBuf[MODULE_NUM] = 
{
	PORT1_NUM,
	PORT2_NUM,
	PORT3_NUM,
	PORT4_NUM,
	PORT5_NUM,
};

const struct sw_i2c_drive sw_port_i2c_drive[MODULE_NUM] = 
{
	 PORT1_I2C_READ,
	 PORT1_I2C_WRITE,
	 PORT2_I2C_READ,	
	 PORT2_I2C_WRITE,	
	 PORT3_I2C_READ,	
	 PORT3_I2C_WRITE,
	 PORT4_I2C_READ,	
	 PORT4_I2C_WRITE,	
	 PORT5_I2C_READ,	
	 PORT5_I2C_WRITE,	
};

#ifdef _USE_SW2325_LIB
	extern const struct sw_func SW2325_Func;
#endif

#ifdef _USE_SW2303_LIB
	extern const struct sw_func SW2303_Func;
#endif

#ifdef _USE_SW3516P_LIB
	extern const struct sw_func SW3516P_Func;
#endif

#ifdef _USE_SW3516H_LIB
	extern const struct sw_func SW3516H_Func;
#endif

#ifdef _USE_SW3526_LIB
	extern const struct sw_func SW3526_Func;
#endif

#ifdef _USE_SW3536_LIB
	extern const struct sw_func SW3536_Func;
#endif

#ifdef _USE_SW3566_LIB
	extern const struct sw_func SW3566_Func;
#endif

#ifdef _USE_NET_LIB
	extern const struct sw_func SW_Net_Func;
#endif


const struct sw_func *SWXXXX_Func[IC_SUM] = 
{
 #ifdef _USE_SW2325_LIB
     &SW2325_Func,
 #else
     0,	
 #endif
	
 #ifdef _USE_SW2303_LIB	
     &SW2303_Func,
 #else
     0,	
 #endif

 #ifdef _USE_SW3516P_LIB		
		&SW3516P_Func,
 #else
     0,		
 #endif

 #ifdef _USE_SW3516H_LIB	
		&SW3516H_Func,
 #else
     0,	  
 #endif

 #ifdef _USE_SW3526_LIB	
		&SW3526_Func,
 #else
     0,	  
 #endif

 #ifdef _USE_SW3536_LIB	
		&SW3536_Func,
 #else
     0,	  
 #endif
 
 #ifdef _USE_SW3566_LIB	
		&SW3566_Func,
 #else
     0,	  
 #endif
 
 #ifdef _USE_NET_LIB	
		&SW_Net_Func,
 #else
     0,	  
 #endif
};

const unsigned char icAddr[4] = {PORT1_IC_ADDR, PORT2_IC_ADDR, PORT3_IC_ADDR, PORT4_IC_ADDR};

//SW3516P/SW3516H/SW3536必须传递相关的NTC电阻参数。
u32 SW_Get_Actual_Temp(u8 port, int *devTemp, float ntcBvalue, float ntcR25)
{
	u8 returnValue = 0;
	u8 port_ic[MODULE_NUM] = {PORT1_IC, PORT2_IC, PORT3_IC, PORT4_IC, PORT5_IC};
	
	switch(port_ic[port])
	{
		case SW_2325:
 		case SW_2303:
					returnValue = SWXXXX_Func[port_ic[port]]->SW_Get_NTC_Temperature(port, devTemp); 
					returnValue = 1;
					break;
 		case SW_3516P:	 
 		case SW_3516H:
		case SW_3536:
		case SW_3566:
					returnValue = SWXXXX_Func[port_ic[port]]->SW_Get_NTC_Temperature(port, devTemp); 
					if(returnValue == 1)
					{
						*devTemp = Res_To_Temperture(ntcBvalue, ntcR25, *devTemp);
					}
					break;
 		case SW_3526:
					returnValue = 2;
					break;	
	}
	return returnValue;
}

static u32 Res_To_Temperture(float ntcBvalue, float ntcR25, float ntcRes)
{
	u32 temperature = 0;
	
	if(ntcRes > 12300)
	{
		temperature = 70;
	}
	else if(ntcRes > 6500)
	{
		temperature = 90;		
	}
	else
	{
		temperature = 110;		
	}
	
	return (u32)(temperature);
}

//此函数的初次调用，请设置get_state_mode中端口对应的状态获取方式。
u32 SW_Get_Specify_State(u8 port, u8 num, get_port_state_mode *get_state_mode, u8 *state)
{
	u8 returnValue = 0;
	u8 tmpValue = 0;
	u32 tmpCurrent = 0;
	u8 port_ic[MODULE_NUM] = {PORT1_IC, PORT2_IC, PORT3_IC, PORT4_IC, PORT5_IC};

		   u8  tmpState = 0;	
	static u8  currentFlag[PORT_SUM][2] = {0, 0, 0, 0, 0, 0, 0, 0}; //当前状态标志
	static u32 changeFlagTime[PORT_SUM][2] = {0, 0, 0, 0, 0, 0, 0, 0}; //状态改变开始时间

	
	switch(get_state_mode -> mode)
	{
		case 0:
				returnValue = SWXXXX_Func[port_ic[port]] -> SW_Get_Port_State(port, num, &tmpState);
		
				if(returnValue  == 1)
				{
					if(tmpState == 0) //当前不在线
					{
						if(currentFlag[port][num] == 0)
						{
							if(My_GetSystemTimePass(changeFlagTime[port][num]) > get_state_mode -> unLoadDebounce)
							{
								*state = 0;
							}
						}
						else
						{
							currentFlag[port][num] = 0;
							changeFlagTime[port][num] = My_GetSystemTimeMark();
						}
					}
					else    //当前设备在线
					{
						if(currentFlag[port][num] > 0)
						{
							if(My_GetSystemTimePass(changeFlagTime[port][num]) > get_state_mode -> loadDebounce)
							{
								*state = 1;
							}
						}
						else
						{
							currentFlag[port][num] = 1;
							changeFlagTime[port][num] = My_GetSystemTimeMark();
						}					
					}
				}
				break;
		case 1:
				returnValue  = SWXXXX_Func[port_ic[port]] ->SW_Get_Current(port, num , &tmpCurrent);
				if(returnValue  == 1)
				{
					if(tmpCurrent < get_state_mode -> unloadCurrent) //小于空载电流进行空载处理
					{
						if(currentFlag[port][num] == 0)
						{
							if(My_GetSystemTimePass(changeFlagTime[port][num]) > get_state_mode -> unLoadDebounce)
							{
								*state = 0;
							}
						}
						else
						{
							currentFlag[port][num] = 0;
							changeFlagTime[port][num] = My_GetSystemTimeMark();
						}
					}
					else if(tmpCurrent > get_state_mode -> loadCurrent) //大于带载电流进行带载处理
					{
						if(currentFlag[port][num] > 0)
						{
							if(My_GetSystemTimePass(changeFlagTime[port][num]) > get_state_mode -> loadDebounce)
							{
								*state = 1;
							}
						}
						else
						{
							currentFlag[port][num] = 1;
							changeFlagTime[port][num] = My_GetSystemTimeMark();
						}					
					}
				}
				break;	
	}
	return returnValue;
}


u32 NTC_To_Gear_Handle(int tmperture, u8 * curGear)
{
	uint8_t curTemperGear = 0;
	uint8_t tmpValue = 0;
	static uint8_t 	oldTemperGear = 0;

	 //更新当前温度档	
	 if(tmperture <= TMPER_GEAR_1) 
	 {
		curTemperGear = 1;
	 }
	 else if(tmperture <= TMPER_GEAR_2) 
	 {
		curTemperGear = 2;
	 }
	 else
	 {
		curTemperGear = 3;	 
	 }
		  
	 //档位变位，更新功率档
	 if(curTemperGear != oldTemperGear)
	 {  
		switch(curTemperGear) 
		{		
			case 1:
					* curGear = 0;
					tmpValue = 1;
					break;
			case 2:	
					break;
			case 3:
					* curGear = 1;
					tmpValue = 1;
					
					break;
		}
		oldTemperGear =  curTemperGear; //更新当前档
	 }
	return tmpValue;
}	

