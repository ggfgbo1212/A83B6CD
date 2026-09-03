/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file serial.h
 * @brief Function implement for printf 
 * @author David
 ****************************************************************************/
#ifndef __SERIAL_H
#define __SERIAL_H

#include "stdio.h"
#include "system.h"	

typedef struct 
{
	u8 sendBuf[64];
	u8 sendLen;
	u8 sendHead;
	u8 sendTail;
	
	u8 revLen;
	u8 revBuf[64];
	u8 tmpRevBuf[64];
	
	u8 revFinishFlag;
	u32 revTime;
	u8	uartStartFlag;

	u8 isWaitAns;
	u32 sendTime;
}usr_uart_driver_t;


void Serail_Init(u32 baudrate);

void Usr_Serail_Init(u32 baudrate, u8 devRole);

u8 Usr_Send_N_Bytes(usr_uart_driver_t *tmpUartDrv, u8 *data, u8 len);

#endif
