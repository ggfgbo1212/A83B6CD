#include "sw3566_i2c_lib.h"
#include "user_define.h"
#include "zr_systick.h"

enum i2c_group_num_e
{
	GROUP0 = 0,
	GROUP1,
	GROUP2,
	GROUP3,
};

typedef struct
{
 u32 scl_id;
 u32 sda_id;	
}i2c_gpio_id_t;

const i2c_gpio_id_t i2cGpioId[4] = 
{	
	SCL0,
	SDA0,
	SCL1,
	SDA1,	
	SCL2,
	SDA2,	
	SCL3,
	SDA3,	
};

static void Change_I2c_Group(u8 group);


static u8 i2cGroupNum = 0;
static u8 i2cNowAddr = 0; 

static u8 SZ_I2c_Master_Write_Data(u8* wr_data, u8 wrBytes);
static u8 SZ_I2c_Master_Read_Data(u8* wr_data, u8 wr_bytes, u8* rd_data, u8 rdBytes);

u32 i2cTime;
u32 (* i2cTimePass)(u32 tm);
u32 (* i2cTimeMark)();

usr_i2c_driver_t i2c_driver_buffer;

static void I2C_Driver_Init(uint8_t slaveAddr)
{
	i2cm_config_t i2cConfig;
	gpio_config_t gpioConfig;
	
	Rcc_Module_Clk_Enable(I2C_ID );
	
	i2cConfig . clockSpeed = 46;
	i2cConfig . devAddr     = slaveAddr;
	I2c_Master_Init(&i2cConfig);
	
}

//I2C通信采用轮循结构，解决主机的单I2C，读除3566外的多从机问题。
void I2c_Slave_Drive_Init(uint8_t slaveAddr)
{
	Change_I2c_Group(SLAVE_GPIO_GROUP); //请选择配置的第几组I2C引脚。
	
	I2c_Slave_Device_Addr_Cfg(slaveAddr);
	I2c_Slave_Force_SCL_Enable();
	I2c_Slave_TX_FIFO_Reset();
	I2c_Slave_Int_Enable(I2C_S_TX_DATA_INT_EN | I2C_S_RX_DATA_INT_EN);
	Nvic_Irq_Enable(I2CS_IRQn, PRIORITY_0);
	I2c_Slave_Enable();
	I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING | I2C_S_TX_DATA_PENGING | I2C_S_FINISH_PENGING
													| I2C_S_RX_OVERFLOW_PENGING | I2C_S_TX_UNDERFLOW_PENGING | I2C_S_RX_FIFO_PENGING
													| I2C_S_TX_FIFO_PENGING | I2C_S_BUS_ERROR_PENGING);			
}

//IIC中断
extern device_policy_t devicePolicy;

void I2CS_Handler()
{
	volatile uint8_t tmpRegAddr = 0;
	volatile uint8_t tmpBuf[5];
	volatile uint8_t tmpLen;
	static uint8_t finishEnFlag = 0;
	
	Low_Power_Not_Sleep();
	
	devicePolicy.notSleepFlag = 1; 
	devicePolicy.wakeFlag = 1;
	
	if (I2c_Slave_Get_Pending(I2C_S_TX_DATA_PENGING))
	{
		//I2c_Slave_Int_Enable(I2C_S_FINISH_INT_EN);
		I2c_Slave_Clear_Pending(I2C_S_TX_DATA_PENGING);
	}
	
	if(I2c_Slave_Get_Pending(I2C_S_RX_DATA_PENGING))
	{
		finishEnFlag = 1;
		I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
		I2c_Slave_Int_Enable(I2C_S_FINISH_INT_EN);
		I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING);
	}	

	if(I2c_Slave_Get_Pending(I2C_S_FINISH_PENGING))
	{
		if(finishEnFlag)
		{	
			tmpRegAddr = I2c_Slave_Get_Register_Address();
			tmpLen = I2c_Slave_Get_RX_Byte_Length();
			
			if(tmpRegAddr > 5)
			{
				I2c_Slave_Get_RX_Data(&i2c_driver_buffer.i2cBuf[tmpRegAddr], tmpLen);
				
				i2c_driver_buffer.revFlag = 1;
			}
			else
			{
				I2c_Slave_Get_RX_Data((u8 *)&tmpBuf[0], tmpLen);
				
				I2c_Slave_TX_FIFO_Reset();
				I2c_Slave_Write_TX_Data(&i2c_driver_buffer.i2cBuf[tmpBuf[0]], tmpBuf[1]);
			}
			finishEnFlag = 0;
		}
		I2c_Slave_Int_Disable(I2C_S_FINISH_INT_EN);
		I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
	}
}


static gpio_func_e Ask_Gpio_Sck(u32 gpioID, u32 sckOrSda)
{
	gpio_func_e tmpSck = 0;
	gpio_func_e tmpSda = 0;
	switch(gpioID)
	{
		case GPIO0:
				tmpSck = (gpio_func_e)GPIO0_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO0_FUNC_SEL_I2C_SDA;
				break;
		case GPIO1:
				tmpSck = (gpio_func_e)GPIO1_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO1_FUNC_SEL_I2C_SDA;
				break;	
		case GPIO2:
				tmpSck = (gpio_func_e)GPIO2_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO2_FUNC_SEL_I2C_SDA;
				break;	
		case GPIO3:
				tmpSck = (gpio_func_e)GPIO3_FUNC_SEL_I2C_SCK;	
				tmpSda = (gpio_func_e)GPIO3_FUNC_SEL_I2C_SDA;
				break;	
		case GPIO4:
				tmpSck = (gpio_func_e)GPIO4_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO4_FUNC_SEL_I2C_SDA;
				break;
		case GPIO5:
				tmpSck = (gpio_func_e)GPIO5_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO5_FUNC_SEL_I2C_SDA;		
				break;
		case GPIO6:
				tmpSck = (gpio_func_e)GPIO6_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO6_FUNC_SEL_I2C_SDA;
				break;
		case GPIO7:
				tmpSck = (gpio_func_e)GPIO7_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO7_FUNC_SEL_I2C_SDA;
				break;
		case GPIO10:
				tmpSck = (gpio_func_e)GPIO10_FUNC_SEL_I2C_SCK;	
				tmpSda = (gpio_func_e)GPIO10_FUNC_SEL_I2C_SDA;		
				break;
		case GPIO11:
				tmpSck = (gpio_func_e)GPIO11_FUNC_SEL_I2C_SCK;	
				tmpSda = (gpio_func_e)GPIO11_FUNC_SEL_I2C_SDA;		
				break;
		case GPIO14:
				tmpSck = (gpio_func_e)GPIO14_FUNC_SEL_I2C_SCK;
				tmpSda = (gpio_func_e)GPIO14_FUNC_SEL_I2C_SDA;
				break;
	}
	
	if(sckOrSda == 1)
		return tmpSda;
	
	return tmpSck;
}

static void Change_I2c_Group(u8 group)
{
	gpio_config_t gpioConfig;
	
	Rcc_Module_Clk_Enable(GPIO_ID);
	
	Gpio_Deinit(i2cGpioId[i2cGroupNum].scl_id);
	Gpio_Deinit(i2cGpioId[i2cGroupNum].sda_id);
	
	i2cGroupNum = group;
	
	Gpio_Deinit(i2cGpioId[i2cGroupNum].scl_id);
	gpioConfig . gpioFunc = Ask_Gpio_Sck(i2cGpioId[i2cGroupNum].scl_id, 0);
	gpioConfig . direction = GPIO_DIRECTION_OUTPUT;
	gpioConfig .pmosOd = 0;
	gpioConfig .nmosOd = 1;
	gpioConfig .pullup = PULLUP_EN;
	gpioConfig .pulldown = PULLDOWN_DIS;
	gpioConfig .res = PULLUP_RES_10K;
	Gpio_Init(i2cGpioId[i2cGroupNum].scl_id, &gpioConfig);
	
	Gpio_Set_Output_Value(i2cGpioId[i2cGroupNum].scl_id, 1);
	
	Gpio_Deinit(i2cGpioId[i2cGroupNum].sda_id);
	gpioConfig . gpioFunc = Ask_Gpio_Sck(i2cGpioId[i2cGroupNum].sda_id, 1);
	gpioConfig . direction = GPIO_DIRECTION_OUTPUT;
	gpioConfig .pmosOd = 0;
	gpioConfig .nmosOd = 1;
	gpioConfig .pullup = PULLUP_EN;
	gpioConfig .pulldown = PULLDOWN_DIS;
	gpioConfig .res = PULLUP_RES_10K;
	Gpio_Init(i2cGpioId[i2cGroupNum].sda_id, &gpioConfig);
	
	Gpio_Set_Output_Value(i2cGpioId[i2cGroupNum].sda_id, 1);
}

void I2C_Drive_Init(uint8_t slaveAddr)
{
	u32 i = 0;
	u8 r_data;	
	u8 iic_return = 0;
	
	I2c_Master_Deinit();
	Change_I2c_Group(GROUP0);
	
	for(; i < 160000; i++)
	{
	
	}
	
	I2C_Driver_Init(slaveAddr);	
}

//写数据
static u8 Hard_I2c_Wrtie(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen)
{
	u8 wr_data[7] = {0, 0, 1, 2, 3 ,4 ,5};	
	u8 iic_return = 0;
	u8 len = 1;
	
	if(devAddr != i2cNowAddr)
	{
		i2cNowAddr = devAddr;
		I2c_Master_Device_Addr_Set(devAddr);
	}
	
	wr_data[0] = writeAddr;
	
	while(len < (wLen + 1))
	{
	 wr_data[len] = *dataToWrite;
	 len ++;
	 (void)*dataToWrite ++;
	}	
		
	iic_return = SZ_I2c_Master_Write_Data(wr_data, wLen + 1);
	
	return iic_return;
}

//写数据
static u8 Hard_I2c_Read(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen)
{
	u8 r_data[1] = {0};	
	u8 iic_return = 0;
	
	if(devAddr != i2cNowAddr)
	{
		i2cNowAddr = devAddr;
		I2c_Master_Device_Addr_Set(devAddr);
	}
	
	r_data[0] = readAddr;
	iic_return = SZ_I2c_Master_Read_Data(r_data, 1, dataToRead, rLen);
	
	return iic_return;
}


/**************** I2C GROUP0 WRITE AND READ ******************************/
u8 I2c_Write_Gruop0(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen)
{
	if(GROUP0 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP0);
	}
	
	return(Hard_I2c_Wrtie(devAddr, writeAddr, dataToWrite, wLen));
}

u8 I2c_Read_Gruop0(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen)
{
	if(GROUP0 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP0);
	}

	return(Hard_I2c_Read(devAddr, readAddr, dataToRead, rLen));
}

/**************** I2C GROUP1 WRITE AND READ ******************************/
u8 I2c_Write_Gruop1(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen)
{
	if(GROUP1 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP1);
	}
	
	return(Hard_I2c_Wrtie(devAddr, writeAddr, dataToWrite, wLen));
}

u8 I2c_Read_Gruop1(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen)
{
	if(GROUP1 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP1);
	}

	return(Hard_I2c_Read(devAddr, readAddr, dataToRead, rLen));
}
/**************** I2C GROUP2 WRITE AND READ ******************************/
u8 I2c_Write_Gruop2(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen)
{
	if(GROUP2 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP2);
	}
	
	return(Hard_I2c_Wrtie(devAddr, writeAddr, dataToWrite, wLen));
}

u8 I2c_Read_Gruop2(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen)
{
	if(GROUP2 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP2);
	}

	return(Hard_I2c_Read(devAddr, readAddr, dataToRead, rLen));
}

/**************** I2C GROUP3 WRITE AND READ ******************************/
u8 I2c_Write_Gruop3(u8 devAddr, u8 writeAddr, u8 *dataToWrite, u8 wLen)
{
	if(GROUP3 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP3);
	}
	
	return(Hard_I2c_Wrtie(devAddr, writeAddr, dataToWrite, wLen));
}

u8 I2c_Read_Gruop3(u8 devAddr, u8 readAddr, u8 *dataToRead, u8 rLen)
{
	if(GROUP3 != i2cGroupNum)
	{
		Change_I2c_Group(GROUP3);
	}

	return(Hard_I2c_Read(devAddr, readAddr, dataToRead, rLen));
}

/**
 * @brief  I2C master write data to I2C slave 
 * @param  wr_data the write data(register address + data) to I2C slave 
 * @param  bytes the number byte of write data(register address + data)
 *         This parameter can be:             
 *         1~32 byte
 * @return Transfer error flag (SET or NOTSET)
 */
static u8 SZ_I2c_Master_Write_Data(u8* wr_data, u8 bytes)
{
    u32 value = 0;
	u8 status = 0;
    volatile static u8 currentState = 0;
	
	switch(currentState)
	{
		case 0:
				value = ZR_I2C->M_CTRL;
				value &= (uint32_t)(~((0x1f << 4) | (0xff << 16)));
				ZR_I2C->M_CTRL = value | (uint32_t)((bytes - 1) << 4 ) | (0x47 << 16);   
		
				if(I2c_Get_Bus_Busy_Status() == SET)
				{
					status = 2; //总线忙，不可写
					currentState = 0;
				}
				else
				{
					I2c_Master_Start();
				
					if(I2c_Master_Get_WR_FIFO_Left() > bytes)
					{
						I2c_Master_Async_Write(wr_data, bytes);
						
						status = 0; //正常，转而下一个过程。 
						currentState = 1; //转而等待传送 完成
						
						i2cTime = (* i2cTimeMark)(); //记下状态切换时间
					}
					else
					{
						status = 3; //fifo空间不够
						currentState = 0;
					}
				}
				break;
		case 1:
				if(I2c_Master_Busy_Flag() == NOTSET)
				{
					status = I2c_Master_Get_Pending(I2C_M_NACK_PENGING);//发送完成
					
					if(status == 0)
					{
						status = 1;	
					}
					else
					{
						status = 7; //不应答ACK;
					}
					currentState = 0;
				}
				else if((* i2cTimePass)(i2cTime) > 20) //20ms超时中断
				{
					status = 6; //超时异常，退出
					currentState = 0;
				}
				break;
	}	
	return status;
}

/**
 * @brief  I2C master read data from I2C slave 
 * @param  wr_data the write data(register address + data) to I2C slave 
 * @param  wr_bytes the number byte of write data(register address + data)
 *         This parameter can be:             
 *         1~32 byte
 * @param  rd_data read data from I2C slave
 * @param  rd_bytes the number byte of read data
 *         This parameter can be:
 *         1~32 byte
 * @return Transfer error flag (SET or NOTSET)
 */
volatile u8 currentReadState = 0;

static u8 SZ_I2c_Master_Read_Data(u8* wr_data, u8 wr_bytes, u8* rd_data, u8 rd_bytes)
{
    u32 value;
    
	u8 status = 0;

	
	switch(currentReadState)
	{
		case 0:
				value = ZR_I2C->M_CTRL;
				value &= (uint32_t)~((0x1f << 4) | (0xff << 16) | (0x1f << 11));
				ZR_I2C->M_CTRL = value | (uint32_t)((wr_bytes - 1) << 4 ) | (uint32_t)((rd_bytes - 1) << 11 ) | (0x7f << 16);
		
				if(I2c_Get_Bus_Busy_Status() == SET)
				{
					status = 2; //总线忙，不可写
					currentReadState = 0;
				}
				else
				{
					I2c_Master_Start();
					
					if(I2c_Master_Get_WR_FIFO_Left() > wr_bytes)
					{
						I2c_Master_Async_Write(wr_data, wr_bytes);
						
						status = 0; //正常，转而下一个过程。 
						currentReadState = 1; //转而等待传送 完成
						i2cTime = (* i2cTimeMark)(); //记下状态切换时间
					}
					else
					{
						status = 3; //fifo空间不够
						currentReadState = 0;
					}				
				}
				break;		
		case 1:
				if(I2c_Master_Get_RD_FIFO_Left() >= rd_bytes)
				{
					status = 0; //正常，转而下一个过程。 
					currentReadState = 2; //转而等待传送 完成	
					i2cTime = (* i2cTimeMark)(); //记下状态切换时间
				}
				else if(I2c_Master_Get_Pending(I2C_M_BUS_LOST_PENGING) == SET) //是否总线出现异常
				{
					status = 5; //总线丢失，通信异常
					currentReadState = 0;
				}
				else if((* i2cTimePass)(i2cTime) > 20) //20ms超时中断
				{
					status = 6; //超时异常，退出
					currentReadState = 0;
				}
				break;
		case 2:
				if(I2c_Master_Busy_Flag() == NOTSET)
				{
					I2c_Master_Async_Read(rd_data, rd_bytes);
					
					status = I2c_Master_Get_Pending(I2C_M_NACK_PENGING);//发送完成

					if(status == 0)
					{
						status = 1;	
					}
					else
					{
						status = 7; //不应答ACK;
					}
					currentReadState = 0;
				}
				else if((* i2cTimePass)(i2cTime) > 20) //20ms超时中断
				{
					status = 6; //超时异常，退出
					currentReadState = 0;
				}
				break;
	}

    return status;
}

void i2c_Register_Time(u32 (* tmpTimePass)(u32 tm), u32 (* tmpTimeMark)())
{
	if((* tmpTimePass) > 0)
	{
		i2cTimePass = tmpTimePass;
	}
    if((* tmpTimeMark) > 0)
	{
		i2cTimeMark = tmpTimeMark;
	}
}





