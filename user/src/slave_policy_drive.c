#include "iic_slave_policy.h"
#include "iic_slave_reg.h"
#include "slave_policy_drive.h"
#include "i2c_ota_policy.h"

extern u8 set_reg_addr;
extern u8 reg_new_data[0x100];
extern u8 reg_0x8F_data[8];
extern u8 reg_0x90_data[8];

#define TIMEMARKMASK 0X7FFFFFFF
volatile u32 SystemTimeCounter = 0;

static uint8_t atmState = 0;
uint8_t wakeFlag = 0;
uint8_t notSleepFlag = 0;

static u8 writePoint;
static u8 readPoint;
static u8 addrBuf[32];

void ATM_Init_Normal(void)
{
	Rcc_Module_Clk_Enable(ATM_ID);
	Rcc_Module_Rst_Release(ATM_ID);
	
	Atm0_Set_Reload_Value(50000);//12500*80ns=1ms
	
	Atm0_Int_Enable();
	Nvic_Irq_Enable(ATM0_IRQn, PRIORITY_3);
	
	Atm0_Enable();
	atmState = 1;	
}

void ATM0_Handler(void)
{
 Atm0_Clear_Int_Pending();	
 SystemTimeCounter += 4;
}

//时间获取函数
u32 My_GetSystemTimePass(u32 tm)
{
	u32 otm = (TIMEMARKMASK & SystemTimeCounter);
	if(otm > tm)
		return (otm-tm);
	else
		return (TIMEMARKMASK&((TIMEMARKMASK & ~tm) + 1 + otm));
}

u32 My_GetSystemTimeMark(void)
{
	return (TIMEMARKMASK & SystemTimeCounter);
}

//此为死延时，只可在初始化的时候调用。
u32 My_DieDleay(u32 time)
{
	volatile  u32 tmpTime = 0;
	
	tmpTime = My_GetSystemTimeMark();
	
	while(My_GetSystemTimePass(tmpTime) < time)
	{
	}
	return 1;
}

void ATM_Init_Enable(void)
{
	if(atmState == 0)
	{
		Atm0_Clear_Int_Pending();	
		Atm0_Int_Enable();
		Atm0_Enable();
		
		atmState = 1;
	}
}

void ATM_Init_Disable(void)
{
	if(atmState > 0)
	{
		Atm0_Clear_Int_Pending();	
		Atm0_Int_Disable();
		Atm0_Disable();	
		atmState = 0;
	}
}



//ADC配置初始化
void ADC_Config_Init(void)
{
	Adc_Enabled();
	adc_config_t adcCfg;
	//Gpio_Function_Select(GPIO0, GPIO0_FUNC_SEL_ADC);
	Adc_Get_Config(&adcCfg);
	//ADC VD Init
	adcCfg.adc3Pullup = ADC_PULLUP_CURRENT_20UA;//100KΩ（≈1.9V-2V）
	adcCfg.adc3Source = ADC3_SIG_SRC_VD,//singal source should equal channel
	//ADC GPIO0 Init
	//adcCfg.adc0Pullup = ADC_PULLUP_CURRENT_20UA;//100KΩ（≈1.9V-2V）
  adcCfg.adc2Pullup = ADC_PULLUP_CURRENT_20UA;
	adcCfg.adc2Source = ADC2_SIG_SRC_CC12;
	
	Adc_Set_Config(&adcCfg);
	
	Adc_Start_Adc_Channel(ADC_CHL_2);//channel should equal singal source
	Adc_Start_Adc_Channel(ADC_CHL_3);//channel should equal singal source	
	
	//防止前端电压不稳导致IIC地址识别错误	
	bool isCheckVinSucess = false;
	u8 checkVinNum = 0;	
	
	while(!isCheckVinSucess)
	{
			if(Adc_Vin_Get() > 5000)
			{
				checkVinNum ++;
				if(checkVinNum > 5)
				{
					isCheckVinSucess = true;
				}
			}
			else
			{
				checkVinNum = 0;
			}
			My_DieDleay(3);
	}
	
	u8 i = 0;
	u8 j = 0;
	u32 tmpBuf[10];
	u32 tmpA = 0;
	u32 tmpValue = 0;
	for(i = 0; i < 10; i ++)
	{
		tmpBuf[i] = Adc_Get_Channel_Data(ADC_CHL_3);//user get channel ADC data
		My_DieDleay(5);
	}
	
	//冒泡排序
	for(int i = 0 ; i < 10 ; i++)
	{
		for(int j = 0 ; j <9 - i ; j++ )
		{
				if(tmpBuf[j] > tmpBuf[j+1])
				{

					 tmpValue = tmpBuf[j];

					 tmpBuf[j] = tmpBuf[j+1];

					 tmpBuf[j+1] = tmpValue;                   
				}
		  }
	}	
	
	tmpValue = tmpBuf[5] * 50;//换算成电阻

	set_reg_addr = tmpValue / 20000;
	
	set_reg_addr += 0x3D;
}



//GPIO配置初始化
void GPIO_Conifg_Init(void)
{
		//中断引脚初始化
		gpio_config_t gpioConfig;
		Rcc_Module_Clk_Enable(GPIO_ID);
	
		Gpio_Deinit(INTERRUPGPIO);
		gpioConfig .direction = GPIO_DIRECTION_OUTPUT;
		gpioConfig .gpioFunc = (gpio_func_e)INTERRUPGPIO_FUNC_SEL_GPIO;
		gpioConfig .pmosOd   = PMOS_OPDRAIN_DIS;
		gpioConfig .nmosOd   = NMOS_OPDRAIN_DIS;
		gpioConfig .pullup   = PULLUP_EN;
		gpioConfig .pulldown = PULLDOWN_DIS;
		gpioConfig .res = PULLUP_RES_10K;

		Gpio_Init(INTERRUPGPIO, &gpioConfig);
		Gpio_Set_Output_Value(INTERRUPGPIO, 1);	
}


//从机3566初始化
void I2C_Drive_Init(void)
{
	I2c_Master_Disable();
	Rcc_Module_Rst(I2C_ID);
    Rcc_Module_Clk_Enable(GPIO_ID | I2C_ID);
    Rcc_Module_Rst_Release(I2C_ID);
    Gpio_Function_Select(I2C_SDA_GPIO, I2C_SDA_M_FUNC);
    Gpio_Function_Select(I2C_SCK_GPIO, I2C_SCK_M_FUNC);

    Gpio_Mos_Opendrain_Cfg(I2C_SDA_GPIO, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Mos_Opendrain_Cfg(I2C_SCK_GPIO, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_SDA_GPIO, PULLDOWN_DIS, PULLUP_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_SCK_GPIO, PULLDOWN_DIS, PULLUP_EN);
	
    I2c_Slave_Device_Addr_Cfg(set_reg_addr);
//		I2c_Slave_Force_SCL_Enable();
		I2c_Slave_TX_FIFO_Reset();
		I2c_Slave_Int_Enable(I2C_S_TX_DATA_INT_EN | I2C_S_RX_DATA_INT_EN);
    Nvic_Irq_Enable(I2CS_IRQn, PRIORITY_0);
    I2c_Slave_Enable();
    I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING | I2C_S_TX_DATA_PENGING | I2C_S_FINISH_PENGING
                            | I2C_S_RX_OVERFLOW_PENGING | I2C_S_TX_UNDERFLOW_PENGING | I2C_S_RX_FIFO_PENGING
                            | I2C_S_TX_FIFO_PENGING | I2C_S_BUS_ERROR_PENGING);
}



//写入要修改寄存器的值的功能
u8 Write_Fifo(u8 tmpAddr)
{
	u8 tmpPoint = writePoint + 1;
	if(tmpPoint > 31)
	{
		tmpPoint = 0; 
	}
		
	if(tmpPoint == readPoint)
	{
		return 1;
	}
	else
	{
		addrBuf[tmpPoint] = tmpAddr;
		writePoint = tmpPoint;
	}
	
	return 0;
}

//读取需要实现功能的寄存器
u8 Read_Fifo(u8 *tmpAddr)
{
	if(readPoint == writePoint)
	{
		return 1;
	}
	else
	{	
		readPoint += 1;
		if(readPoint > 31)
		{
			readPoint = 0;
		}
		
		*tmpAddr = addrBuf[readPoint];
	}
	
	return 0;
}




//IIC中断
void I2CS_Handler()
{
	volatile uint8_t regAddr = 0;
	volatile uint8_t regData = 0;
	volatile uint8_t tmpBuf[8];
	volatile static uint8_t finishEnFlag = 0;
	
	Low_Power_Not_Sleep();
	wakeFlag = 1;
	notSleepFlag = 1;
	
		//读会进入
	if(I2c_Slave_Get_Pending(I2C_S_TX_DATA_PENGING))
	{
			regAddr = I2c_Slave_Get_Register_Address();
			//OTA: register 0xAA carries the vendor OTA frame instead of a normal
			//3-byte register reply. Bypassed here so every other register keeps
			//its original behaviour.
			if(regAddr == I2C_OTA_REG_ADDR)
			{
				I2c_Ota_On_Tx_Request();
			}
			else if(regAddr == 0x8F)
			{
                for (uint8_t i =0 ; i < 8; i++)
                {
                    ZR_I2C->S_TX_DATA = reg_0x8F_data[i];
                }		
			}
			else if(regAddr == 0x90)
			{
                for (uint8_t i =0 ; i < 8; i++)
                {
                    ZR_I2C->S_TX_DATA = reg_0x90_data[i];
                }				
			}
			else
			{
				ZR_I2C->S_TX_DATA =  regAddr;
				ZR_I2C->S_TX_DATA =  reg_new_data[regAddr];
				ZR_I2C->S_TX_DATA =  (~(regAddr + reg_new_data[regAddr]));
								
			}		
			I2c_Slave_Clear_Pending(I2C_S_TX_DATA_PENGING);
			
			finishEnFlag = 1;
			I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
			I2c_Slave_Int_Enable(I2C_S_FINISH_INT_EN);
			
			//ZR_GPIO->DATAOUT &= ~(1 << 2);
	}

	//写会进入
	if(I2c_Slave_Get_Pending(I2C_S_RX_DATA_PENGING))
	{
		regAddr = I2c_Slave_Get_Register_Address();
		if(regAddr == I2C_OTA_REG_ADDR)
		{
			I2c_Ota_On_Rx_Byte();
		}
		else
		{
			regData = 0x00;
			I2c_Slave_Get_RX_Data((u8 *)&regData,1);
			reg_new_data[regAddr] = regData;
			Write_Fifo(regAddr);
			I2c_Slave_RX_FIFO_Reset();
			I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING);
		}
	}
	
	I2c_Ota_On_Finish();
	
	if(finishEnFlag)
	{
		//ZR_GPIO->DATAOUT |= 1 << 2;
		
		if(I2c_Slave_Get_Pending(I2C_S_FINISH_PENGING))
		{
			I2c_Slave_TX_FIFO_Reset();		
			finishEnFlag = 0;
			I2c_Slave_Int_Disable(I2C_S_FINISH_INT_EN);
			I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
		}
	}
}

void Sys_Sleep_Policy(void)
{
	
	static uint8_t readyToSleep = 0;
	static uint32_t wakeTime = 0;


	if((reg_new_data[0x34] & 0xC0) != 0x00)
	{
		wakeFlag = 1;
		notSleepFlag = 1;
	}
	
	if(wakeFlag > 0)
	{
		wakeFlag = 0;
		ATM_Init_Enable();
		wakeTime = My_GetSystemTimeMark();
		readyToSleep = 1;
	}
	
	if(readyToSleep > 0)
	{
		if(My_GetSystemTimePass(wakeTime) > 10000)
		{
			ATM_Init_Disable();
			readyToSleep = 0;
			notSleepFlag = 0;
		}
	}

	if((notSleepFlag > 0) || ((reg_new_data[SYS_SLEEP_CTRL_REG] & 0x01) == 0))
	{
		Low_Power_Not_Sleep(); 	
	}	

	Low_Power_Sleep();	
}

