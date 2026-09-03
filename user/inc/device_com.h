#ifndef __DEV_COM_H
#define __DEV_COM_H

#include "system.h"
#include "device_policy.h" 

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint8_t data;
	uint8_t sumCrc;
}dev_com_frame_t;

enum dev_cmd_e
{
	PORT_STATE_ASK = 1,
	PORT_STATE_ANS,
	PORT_POWER_SET,
	PORT_POWER_ANS,
};

void Dev_Com_Policy(device_policy_t *portDevice);

extern u8 Get_Net_Dev_State(u8 devBit, u8 *state);
extern u8 Set_Net_Dev_Power(u8 devBit);
extern u8 Set_Net_Req_Info(u8 devBit);
u8 I2c_Wake_Up_Master(void);
#endif