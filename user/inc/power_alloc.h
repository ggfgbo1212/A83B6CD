#ifndef __POWER_ALLOC_H
#define __POWER_ALLOC_H

#include "system.h"


																					
//获取端口状态
#define GET_P1_POWER_UPDATE(x)      (x & 0x01)																					
#define GET_P2_POWER_UPDATE(x)      (x & 0x02)
#define GET_P3_POWER_UPDATE(x)      (x & 0x04)	
#define GET_P4_POWER_UPDATE(x)      (x & 0x08)	
#define GET_P5_POWER_UPDATE(x)      (x & 0x10)	

#define SET_P1_POWER_UPDATE(x)      (x |= 0x01)																					
#define SET_P2_POWER_UPDATE(x)   	(x |= 0x02)	
#define SET_P3_POWER_UPDATE(x)   	(x |= 0x04)	
#define SET_P4_POWER_UPDATE(x)   	(x |= 0x08)	
#define SET_P5_POWER_UPDATE(x)   	(x |= 0x10)	

#define CLEAR_P1_POWER_UPDATE(x)    (x &= (~(0x01)))																					
#define CLEAR_P2_POWER_UPDATE(x)    (x &= (~(0x02)))	
#define CLEAR_P3_POWER_UPDATE(x)    (x &= (~(0x04)))	
#define CLEAR_P4_POWER_UPDATE(x)    (x &= (~(0x08)))	
#define CLEAR_P5_POWER_UPDATE(x)    (x &= (~(0x10)))	

//功率档位以mv为单位
#define 	VOL_GEAR_1 			5300
#define		VOL_GEAR_2 			9300
#define 	VOL_GEAR_3 			12300
#define 	VOL_GEAR_4 			15300
#define 	VOL_GEAR_5 			21300
#define 	VOL_GEAR_6 			28100

//功率档位以mw为单位
#define 	POWER_GEAR_1 		20000
#define		POWER_GEAR_2 		30000
#define 	POWER_GEAR_3 		50000
#define 	POWER_GEAR_4 		60000
#define 	POWER_GEAR_5 		65000
#define 	POWER_GEAR_6 		100000
#define 	POWER_GEAR_7 		140000
//获取端口状态
#define GET_PORT_STATE(x)    (x & 0x01)

//获取端口协议
#define GET_PORT_PROTOCOL(x) (x & 0x02)

//获取端口功率档（分配的，不是实际）
#define GET_POWER_GEAR(x)     (x >> 5)

//获取端口电压档（实际电压）
#define GET_VOL_GEAR(x)       ((x >> 2) & 0x07)

//设置功率档
#define SET_POWER_GEAR(oldValue, newValue) do \
	                                        {\
												oldValue &= 0x1F; \
												oldValue |= (newValue << 5);\
											}while(0);	
//设置电压档																					
#define SET_VOL_GEAR(oldValue, newValue) do \
	                                        {\
												oldValue &= 0xE3; \
												oldValue |= (newValue << 2);\
											}while(0);															 

enum related_kind_e
{
	RELATED_NONE,
	RELATED_VOL = 1,
	RELATED_POWER,
	RELATED_STATE,
};

																					
#pragma pack(1)

//端口协议功率表
typedef struct 
{
	uint8_t  portPowerChart; //bit 0:状态有无。bit1:协议有无。bit2-4:电压档。bit5-7:功率档 
	uint32_t portMaxPower;
	uint32_t portVoltage;	
	uint32_t portCurrent;
	uint8_t  portStateChange; //0：未变，1：插入，2：拔出
}port_device_t;


typedef struct
{
	uint8_t  relatedPort[2];
	uint8_t  relatedKnd[2];
	uint8_t  relatedThres[2];
	uint16_t powerList[2][2]; 	//正常情况下  powerList[0][0]:无关联功率;powerList[0][1]:关联成功功率; 
								//过温情况下  powerList[1][0]:无关联功率;powerList[1][1]:关联成功功率; 		
}related_port_t;	

typedef struct
{
	related_port_t	 firstRelated;
	related_port_t	 afterRelated;	
	related_port_t	 otherOutRelated;
	u8               tempertureRelated;//0:温度不影响功率分配
	u8               relatedOnOff; //0：不分先后拔出。1：分先后插。2：其它设备拔出影响当前。在不区分时只需要配置先插功率
}port_power_alloc_t;

typedef struct
{
	u8 port;
	u8 onlineNum; 
	port_device_t *portChart; 
	u8 tempertureState;
}ask_power_para_t;


typedef struct
{
	const volatile uint32_t* pdSrcCap;  
	
	uint8_t sprFixPdoNumber;
	uint8_t sprPdoNumber;
	uint8_t eprFixPdoNumber;
	uint8_t	eprPdoNumber;
}pdo_cfg_t;	


#pragma pack()																					



uint8_t Power_To_Gear(uint16_t power);
uint8_t Get_Vol_Gear(uint16_t vol);
void Ask_Port_Power(port_device_t *portChart, u8 tmpertureState, u32 *power);
u32 Easy_Handle_Ask(u8 port, port_device_t *portChart, u8 tmpertureState);

u8 Calc_Online_Num( port_device_t  *portChart);

void Cfg_Sys_Pdo(u16 power);

#endif



