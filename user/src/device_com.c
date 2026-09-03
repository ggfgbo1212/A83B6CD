#include "sw3566_i2c_lib.h"
#include "device_com.h" 
#include "user_define.h" 
#include "device_drive.h" 
#include "ic_common.h"
#include "user_define.h"

extern const struct sw_i2c_drive sw_port_i2c_drive[4];
extern const unsigned char icAddr[4];

static void Slave_Dev_Com(device_policy_t *portDevice);

extern device_policy_t devicePolicy;

extern usr_i2c_driver_t i2c_driver_buffer;

void Dev_Com_Policy(device_policy_t *portDevice)
{
	Slave_Dev_Com(portDevice);
}

/******************** 从设备处理 ,只做被动处理 ***********************/
static void Slave_Dev_Com(device_policy_t *portDevice)
{
	//接收处理
	if(i2c_driver_buffer.revFlag) //是否开启传输
	{
		portDevice ->newPortDevice[portDevice -> devAddr].portMaxPower = i2c_driver_buffer.i2cBuf[POWER_REG] * 10;
		portDevice ->isNeedPowerPolicy = 1;
		
		i2c_driver_buffer.revFlag = 0;
	}
}
/********************** 主设备处理 ************************************/

u8 Get_Net_Dev_State(u8 devBit, u8 *state)
{
	uint8_t returnValue = 0;
	uint8_t readAddrLen[2] = {PORT1_STATE_REG, SLAVE_READ_LEN}; //从PORT1_STATE_REG开始连续读3个值
	static uint8_t stepCnt = 0;
	
	if(stepCnt == 0)
	{
		if(sw_port_i2c_drive[devBit].i2c_write_byte(icAddr[devBit],  0, &readAddrLen[0], 2)) //把要读的地址和数据写入
		{
			stepCnt = 1;
			
			returnValue = 0;
		}	
	}
	else if(stepCnt == 1)
	{
		if(sw_port_i2c_drive[devBit].i2c_read_byte(icAddr[devBit], readAddrLen[0],  &i2c_driver_buffer.i2cBuf[PORT1_STATE_REG], readAddrLen[1]) > 0)
		{
			stepCnt = 0;
			
			returnValue = 1;
			
			devicePolicy.newPortDevice[devBit].portPowerChart = i2c_driver_buffer.i2cBuf[PORT1_STATE_REG];
			devicePolicy.cBuckVolInGear[devBit] = i2c_driver_buffer.i2cBuf[REQ_VOL_REG];
			
			*state = (devicePolicy.newPortDevice[devBit].portPowerChart & 0x01);
		}
	}
	return returnValue;
}

u8 Set_Net_Dev_Power(u8 devBit)
{
	uint8_t returnValue = 0;
	
	i2c_driver_buffer.i2cBuf[POWER_REG] = devicePolicy.newPortDevice[devBit].portMaxPower / 10;
	if(sw_port_i2c_drive[devBit].i2c_write_byte(icAddr[devBit],  POWER_REG,  &i2c_driver_buffer.i2cBuf[POWER_REG], 1)) //把要读的地址和数据写入
	{	
		returnValue = 1;
	}
	
	return  returnValue;
}

u8 I2c_Wake_Up_Master(void)
{
	uint8_t returnValue = 1;
	uint8_t status;
	uint8_t tmpPower[2] = {WAKE_REG, 0x55};
	static uint8_t writeCnt = 0;
	
    status = sw_port_i2c_drive[devicePolicy.devAddr].i2c_write_byte(icAddr[0],  tmpPower[0], &tmpPower[1], 1);
	if(status == 0) //把要读的地址和数据写入
	{	
		return 0;
	}
	else
	{
		if(status == 1)
		{
			return 1;
		}
		else
		{
			writeCnt += 1;
			if(writeCnt > 3)
			{
				writeCnt = 0;
				return 1;
			}
		}
	}
	
	return returnValue;
}