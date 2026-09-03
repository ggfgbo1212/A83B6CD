#ifndef I2C_H
#define I2C_H

//配置需要的I2C端口，软I2C

#include "system.h"

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

void SOFT_IIC_Init(void);

u8 soft_iic_wrtie_1(u8 Devaddr,u8 WriteAddr,u8 DataToWrite);
u8 soft_iic_read_1(u8 Devaddr,u8 WriteAddr,u8 *readData);

u8 soft_iic_wrtie_2(u8 Devaddr,u8 WriteAddr,u8 DataToWrite);
u8 soft_iic_read_2(u8 Devaddr,u8 WriteAddr,u8 *readData);

u8 soft_iic_wrtie_3(u8 Devaddr,u8 WriteAddr,u8 DataToWrite);
u8 soft_iic_read_3(u8 Devaddr,u8 WriteAddr,u8 *readData);

#endif


