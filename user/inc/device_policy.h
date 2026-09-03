#ifndef __POLICY_h
#define __POLICY_h

#include "system.h"
#include "power_alloc.h"
#include "user_define.h"
#include "ic_lib_rely.h" 
//#include <stdint.h>

enum PORT_CODE
{
	PORT_P1 = 1,
	PORT_P2,	
	PORT_P3,
	PORT_P4,
	PORT_P5
};

#pragma pack(1)

enum dev_role_e
{
	MASTER_DEV,
	SLAVE_DEV,
};

enum sys_policy_state_e
{
	SYS_POLICY_INIT = 0,
	PORT_STATE_POLICY,
	PORT_VOL_POLICY,
	PORT_CUR_POLICY,
	PORT_TEMPER_POLICY,
	PORT_PROTOCOL_POLICY,
	PORT_POWER_POLICY,
	SYS_POLICY_ANA,
	PORT_REQ_VOL_POLICY,
};


//策略层变量
typedef struct 
{
	port_device_t	newPortDevice[PORT_SUM];//新设备表，包括了状态功率电压等相关信息
	port_device_t	lastPortDevice[PORT_SUM];//旧设备表，包括了状态功率电压等相关信息
	
	get_port_state_mode getPortMode[PORT_SUM];//获取端口模式的设置。
	
	volatile uint8_t   	devRole;
	volatile uint8_t 	devAddr;	
 
	volatile uint8_t  cBuckVolInGear[3];    //c口请求电压档
	volatile uint8_t  sysBuckVolInGear;
	
	
	uint8_t         isResetPDO;//是否需要更新PDO
	uint8_t         powerGear;//温度功率档，常温和高温
	uint8_t         isNeedPowerPolicy;//是否需要功率分配策略
   
	int             devTemperture;
	uint8_t			dlyToUpdatePower;
	uint32_t  		portOutTime;
	
	uint32_t   		dlyToClosePFCTime;
	uint32_t		gpioUpdateTime; 
	uint8_t			powerFlag;
	uint8_t 		readyClosePFCFlag;
	volatile uint8_t notSleepFlag;
	
	volatile uint8_t portUpdate;
	volatile uint8_t curUpdate;		//bit0-bit4分别为C1/C2/C3/C4/A5需要电流更新位
	volatile uint8_t volUpdate;		//bit0-bit4分别为C1/C2/C3/C4/A5需要电压更新位
	volatile uint8_t tempUpdate;    //bit0-bit4分别为C1/C2/C3/C4/A5需要温度更新位，现在只用了一位
	volatile uint8_t powerUpdate;  //bit0-bit4分别为C1/C2/C3/C4/A5需要功率更新位
	volatile uint8_t broadcastUpdate;  //bit0-bit4分别为C1/C2/C3/C4/A5需要广播更新位
	volatile uint8_t protocolUpdate;
	
	volatile uint8_t slaveWakeFlag;
	uint32_t          wakeTime;
	volatile uint8_t  wakeFlag;
	volatile uint8_t  readyToSleep;
	
}device_policy_t;

#pragma pack()
	
void Device_Policy(device_policy_t *portDevice);
void Power_Policy(device_policy_t *portDevice);
void Gpio_Policy(device_policy_t *portDevice);
void MasterSlaveIdentify(device_policy_t *portDevice);
void Sleep_Policy(device_policy_t *portDevice);

#endif


