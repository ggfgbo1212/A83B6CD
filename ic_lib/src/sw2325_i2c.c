#include "i2c.h"
#include "include.h"
#include "user_define.h"

/********************* 软I2C代码区 *********************/

#define IIC_DELAY 2
#define IIC_DELAY_BYTE 4

typedef struct
{
 u32 scl_id;
 u32 sda_id;	
}i2c_gpio_id_t;

const i2c_gpio_id_t I2C_Gpio_Id[2] = 
{
 #ifdef SCL0	
 SCL0,
 #endif 
	
 #ifdef SDA1	
 SDA1,
 #endif
	
 #ifdef SCL2	
 SCL2,
 #endif 
	
 #ifdef SDA3	
 SDA3,
 #endif	
};

u32 Func_Gpio = 0;


void SOFT_IIC_Init(void)
{
	RCC_Module_Clk_Enable(GPIO_ID);
	//第一个3536的I2C的GPIO配置
	GPIO_Function_Sel(SCL0, FUNCTION_SEL0);
	GPIO_Function_Sel(SDA0, FUNCTION_SEL0);
	
	GPIO_Pulldown_Pullup_Cfg(SCL0,PULLDOWN_DIS,PULLUP_EN);
	GPIO_Pulldown_Pullup_Cfg(SDA0,PULLDOWN_DIS,PULLUP_EN);
	
	GPIO_Set_Output_Value1(SCL0);
	GPIO_Set_Output_Value1(SDA0);
	GPIO_Output_Enable(SCL0);
	GPIO_Output_Enable(SDA0);
  //第二个3536的I2C的GPIO配置
	GPIO_Function_Sel(SCL1, FUNCTION_SEL0);
	GPIO_Function_Sel(SDA1, FUNCTION_SEL0);
	
	GPIO_Pulldown_Pullup_Cfg(SCL1,PULLDOWN_DIS,PULLUP_EN);
	GPIO_Pulldown_Pullup_Cfg(SDA1,PULLDOWN_DIS,PULLUP_EN);
	
	GPIO_Set_Output_Value1(SCL1);
	GPIO_Set_Output_Value1(SDA1);
	GPIO_Output_Enable(SCL1);
	GPIO_Output_Enable(SDA1);
}

static void DelayUs(u8 Time)
{
	Time++;
}

static void SCLSet1(void)
{
		GPIO_Set_Output_Value1(I2C_Gpio_Id[Func_Gpio].scl_id);
}

static void SCLSet0(void)
{
		GPIO_Set_Output_Value0(I2C_Gpio_Id[Func_Gpio].scl_id);
}

static void SDASet1(void)
{
		GPIO_Set_Output_Value1(I2C_Gpio_Id[Func_Gpio].sda_id);
}

static void SDASet0(void)
{
		GPIO_Set_Output_Value0(I2C_Gpio_Id[Func_Gpio].sda_id);
}

static void SDADirectionOUT(void)
{
		GPIO_Output_Enable(I2C_Gpio_Id[Func_Gpio].sda_id);
}


static u8 ReadSDA(void)
{
		return GPIO_Get_Input_Value(I2C_Gpio_Id[Func_Gpio].sda_id);
}


static void IIC_Start(void)
{
	SDADirectionOUT();
	SDASet1();
	SCLSet1();
	DelayUs(IIC_DELAY);
	SDASet0();
//	DelayUs(IIC_DELAY);
	SCLSet0();
}

static void IIC_Stop(void)
{
	SDADirectionOUT();
	SCLSet0();
	SDASet0();
	DelayUs(IIC_DELAY);
	SCLSet1();
	SDASet1();
//	DelayUs(IIC_DELAY);
}


static u8 IIC_Wait_Ack(void)
{
	u16 ucErrTime=0;
	//SDA_IN
	SDASet1();//DelayUs(IIC_DELAY);
	SCLSet1();//DelayUs(IIC_DELAY);
	while(ReadSDA())
	{
		ucErrTime++;
		if(ucErrTime>300)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCLSet0();
	return 0;
}


static void IIC_Ack(void)
{
	SCLSet0();
	SDADirectionOUT();
	SDASet0();
//	DelayUs(IIC_DELAY);
	SCLSet1();
//	DelayUs(IIC_DELAY);
	SCLSet0();
}


static void IIC_NAck(void)
{
	SCLSet0();
	SDADirectionOUT();
	SDASet1();
	DelayUs(IIC_DELAY);
	SCLSet1();
//	DelayUs(IIC_DELAY);
	SCLSet0();
}

static void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDADirectionOUT();
	SCLSet0();
	for(t = 0;t<8;t++)
	{
		if((txd&0x80)>>7)
		{
			SDASet1();
		}else{
			SDASet0();
		}
		txd<<=1;
//		DelayUs(IIC_DELAY);
		SCLSet1();
//		DelayUs(IIC_DELAY);//////////////////////////////
		SCLSet0();
//		DelayUs(IIC_DELAY);
	}
}



static u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,recevie=0;
	//SDA_IN
	for(i=0;i<8;i++)
	{
		SCLSet0();
//		DelayUs(IIC_DELAY);//000000000000000000
		SCLSet1();
		recevie <<=1;
		if(ReadSDA())
		{
			recevie++;
		}
//		DelayUs(IIC_DELAY);
	}
	if(!ack)
	{
		IIC_NAck();
	}else{
		IIC_Ack();
	}
	return recevie;
}

_IIC_DATA SOFT_I2cRead(u8 DevAddr,u8 ReadAddr)
{
	static _IIC_DATA iic_data;

			IIC_Start();
			IIC_Send_Byte(DevAddr<<1);
			if(IIC_Wait_Ack())
			{
				iic_data.state = IIC_ERROR;
				return iic_data;
			}
	
			IIC_Send_Byte(ReadAddr&0xFF);
			if(IIC_Wait_Ack())
			{
				iic_data.state = IIC_ERROR;
				return iic_data;
			}
		
				IIC_Start();
				IIC_Send_Byte((DevAddr<<1)|1);
				if(IIC_Wait_Ack())
				{
					iic_data.state = IIC_ERROR;
					return iic_data;
				}
		
				iic_data.data = IIC_Read_Byte(0);
				IIC_Stop();
				
				iic_data.state = IIC_IDEL;
		
	return iic_data;
}

_IIC_DATA SOFT_I2cWrite(u8 Devaddr,u8 WriteAddr,u8 DataToWrite)
{
	static _IIC_DATA iic_data;
	

				iic_data.state = IIC_BUSY;
				IIC_Start();
				IIC_Send_Byte(Devaddr<<1);
				if(IIC_Wait_Ack())
				{
				
					iic_data.state = IIC_ERROR;
					return iic_data;
				}
	
				IIC_Send_Byte(WriteAddr&0xFF);
				if(IIC_Wait_Ack())
				{
					iic_data.state = IIC_ERROR;
					return iic_data;
				}
			
				IIC_Send_Byte(DataToWrite);
				if(IIC_Wait_Ack())
				{
					iic_data.state = IIC_ERROR;
					return iic_data;
				}
				IIC_Stop();
				iic_data.state = IIC_IDEL;

	return iic_data;
}

/*****************************************************************/
//以下为修改的模拟iic读写 针对SW2325使用
u8 soft_iic_wrtie_1(u8 Devaddr,u8 WriteAddr,u8 DataToWrite)
{
	_IIC_DATA temp;
	
	Func_Gpio = 0;
	
	temp = SOFT_I2cWrite(Devaddr,WriteAddr,DataToWrite);
	
	return temp.state;
}

u8 soft_iic_read_1(u8 Devaddr, u8 WriteAddr, u8 *readData)
{
	_IIC_DATA temp;
	
	Func_Gpio = 0;
	
	temp = SOFT_I2cRead(Devaddr, WriteAddr);
	*readData = temp.data;
	
	return temp.state;
}


u8 soft_iic_wrtie_2(u8 Devaddr, u8 WriteAddr, u8 DataToWrite)
{
	 _IIC_DATA temp;
	
	Func_Gpio = 1;
	
	temp = SOFT_I2cWrite(Devaddr, WriteAddr, DataToWrite);
	
	return temp.state;
}

u8 soft_iic_read_2(u8 Devaddr, u8 WriteAddr, u8 *readData)
{
	_IIC_DATA temp;
	
	Func_Gpio = 1;
	
	temp = SOFT_I2cRead(Devaddr, WriteAddr);
	*readData = temp.data;
	
	return temp.state;
}

/********************* 硬I2C代码区 *********************/

void I2CMConfig(u8 device_addr, u32 freq)
{
	I2CM_Device_Addr_Cfg(device_addr);
	I2CM_Freq_Set(freq);
}

void iI2CM_Init(void)
{	
	/* pinmux */
    GPIO_Function_Sel(GPIO7,FUNCTION_SEL1);  //SDA
    GPIO_Function_Sel(GPIO6,FUNCTION_SEL1);  //SCL
	GPIO_Mos_Opendrain_Cfg(GPIO7,PMOS_OPDRAIN_DIS,NMOS_OPDRAIN_EN);
	GPIO_Mos_Opendrain_Cfg(GPIO6,PMOS_OPDRAIN_DIS,NMOS_OPDRAIN_EN);
	I2CMConfig(0X3C,I2CM_FREQ_400KHZ);
}


//读数据
u8 hard_iic_read(u8 Devaddr, u8 ReadAddr, u8 *readData)
{
	 u8 iic_return = 0;	
	
	 iic_return = I2CM_Read_Data(&ReadAddr, 1, readData, 1);
	
	 return iic_return;
}

//写数据
u8 hard_iic_wrtie(u8 devAddr, u8 writeAddr, u8 dataToWrite)
{
	u8 wr_data[2] = {0, 0};	
	u8 iic_return = 0;
	
	wr_data[0] = devAddr;
	wr_data[1] = dataToWrite;
	iic_return = I2CM_Write_Data(wr_data, 2);

	return iic_return;
}



