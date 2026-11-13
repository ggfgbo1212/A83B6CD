#ifndef USR_COMMON_H
#define USR_COMMON_H
#include "system.h"

#define QUEUEMAXSIZE 64

typedef struct
{
	u8 queueHead;
	u8 queueTail;
	u8 queueData[64];
}usr_queue_t;



typedef struct 
{
	//u8 sendBuf[64];  //发送buf:64个字节
	u8 sendLen;      
	u8 sendHead;     
	u8 sendTail;     
	
	volatile u8 revcLen;     //接收长度
	volatile u8 revBuf[64];  //接收buf
	//volatile u8 tmpRevBuf[64]; 
	
	volatile bool revFinish;   //接收完成
	
	bool IsRecUnVdmMsg;       
	bool IsInitUart;         
	u16 RecUnVdmMsgHeader;    
	u8 recUnVdmVdoAmount;     
}usr_uart_msg_t;

bool User_Pd_Policy_Hook(void);

void User_Pd_Policy_Get_Sink_Cap_Extended_Run(void);

void Usr_Uart_Init(u32 baudrate);

void UnVdm_Com_Policy(void);



//u32 My_GetSystemTimePass(u32 tm);
//u32 My_GetSystemTimeMark(void);
#endif