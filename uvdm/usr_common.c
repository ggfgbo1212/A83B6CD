#include "usr_common.h"
#include "zr_uart.h"
#include "system.h"
#include "zr_atm.h"
#include "zr_rcc.h"
#include "zr_nvic.h"
#include "zr_uart.h"
#include "zr_gpio.h"
#include "stdlib.h"
#include "stdio.h"
#include "api.h"
#include "pd_message.h"
#include <string.h>

#define TIMEMARKMASK 0X7FFFFFFF
// u32 SystemTimeCounter = 0;

static const pd_mesg_summ_t *pdRxMessageSummary;
static const u8 *pdRxBuff;
static pd_mesg_summ_t *pdTxMessageSummary;
static u8 *pdTxBuff;

void Usr_Uart_Init(u32 baudrate)
{
	/* uart pinmux */
	Rcc_Module_Clk_Enable(GPIO_ID);
	#if 0
		Gpio_Function_Select(GPIO10, GPIO10_FUNC_SEL_UART_TX); /* tx */
		Gpio_Function_Select(GPIO11, GPIO11_FUNC_SEL_UART_RX); /* rx */
	#else
		Gpio_Function_Select(GPIO13, GPIO13_FUNC_SEL_UART_TX); /* tx */
		Gpio_Function_Select(GPIO12, GPIO12_FUNC_SEL_UART_RX); /* rx */
	#endif
	/* clk,reset */
	Rcc_Module_Rst(UART_ID);
	Rcc_Module_Clk_Enable(UART_ID);
	Rcc_Module_Rst_Release(UART_ID);

	Uart_Mode_Cfg(MODE_TXRX);
	Uart_Baudrate_Cfg(baudrate);
	Uart_TX_Enable();
	Uart_RX_Enable();

	Nvic_Irq_Enable(UART_IRQn, PRIORITY_1);
	Uart_Interrupt_Enable(UART_TX_FINISH_INT_EN | UART_RX_FINISH_INT_EN);
	Uart_Clear_Pending(UART_TX_FINISH_INT_PENDING | UART_RX_FINISH_INT_PENDING);
	// Rcc_Module_Clk_Disable(GPIO_ID);
}

static usr_uart_msg_t UartMsgBuf = {0};
usr_uart_msg_t *pUartMsgBuf = &UartMsgBuf;

u32 recUnVdmMsgBufArray[10] = {0};

static usr_queue_t usr_queuet = {0};
static usr_queue_t *usr_queue = &usr_queuet;

// static const pd_mesg_summ_t* pdRxMessageSummary;
static const u8 *pdRxBuff;
// static pd_mesg_summ_t* pdTxMessageSummary;
static u8 *pdTxBuff;

static bool Queue_Is_FuLL();
static bool Queue_Is_Empty();
static bool Send_Byte_To_Queue(u8 data);
static bool Get_Byte_To_Queue(u8 *data);
static u8 Get_Free_Memory();
static bool Write_To_Send_Queue(u8 *data, u8 len);
static bool Read_From_Send_Queue(u8 *data);
static bool Get_Memmory(u8 byte, u8 **pp);
static void Release_Memmory(u8 *addr);
static bool Analyse_Uart_Msg(usr_uart_msg_t *UartMsg);
static bool Analyse_UnVdm_Msg(u32 *vdoData);
static void Init_Usr_UnVdm(void);
static void Pd_Send_Vdo(const unsigned int *data, int count);
static u8 Usr_Send_Bytes(u8 *data, u8 len);

bool Queue_Is_FuLL()
{
	if ((usr_queue->queueTail + 1) % QUEUEMAXSIZE == usr_queue->queueHead)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Queue_Is_Empty()
{
	if (usr_queue->queueHead == usr_queue->queueTail)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Send_Byte_To_Queue(u8 data)
{
	if (Queue_Is_FuLL())
	{
		return false;
	}
	else
	{
		usr_queue->queueTail = (usr_queue->queueTail + 1) % QUEUEMAXSIZE;
		usr_queue->queueData[usr_queue->queueTail] = data;
	}
	return true;
}

bool Get_Byte_To_Queue(u8 *data)
{
	if (Queue_Is_Empty())
	{
		return false;
	}
	else
	{
		usr_queue->queueHead = (usr_queue->queueHead + 1) % QUEUEMAXSIZE;
		*data = usr_queue->queueData[usr_queue->queueHead];
	}
	return true;
}

u8 Get_Free_Memory()
{
	u8 freeMem = 0;
	if (Queue_Is_Empty())
	{
		freeMem = 63;
	}

	else if (Queue_Is_FuLL())
	{
		freeMem = 0;
	}

	else if (usr_queue->queueHead < usr_queue->queueTail)
	{
		freeMem = QUEUEMAXSIZE - (usr_queue->queueTail - usr_queue->queueHead) - 1;
	}
	else if (usr_queue->queueHead > usr_queue->queueTail)
	{
		freeMem = usr_queue->queueHead - usr_queue->queueTail - 1;
	}
	else
	{
		freeMem = 255;
	}
	return freeMem;
}

bool Write_To_Send_Queue(u8 *data, u8 len)
{
	u8 temLen = len;
	if ((Get_Free_Memory() < temLen) || (len == 0))
	{
		return false;
	}

	while (temLen--)
	{
		Send_Byte_To_Queue(*data);
		if (temLen > 0)
		{
			data++;
		}
	}

	return true;
}

bool Read_From_Send_Queue(u8 *data)
{
	static u32 tm = 0;
	static bool mark = 1;
	static u8 wait = 0;

	if (Queue_Is_Empty())
	{
		return false;
	}

	Get_Byte_To_Queue(data);
	return true;
}

/******************************************
串口发送
******************************************/
u8 Usr_Send_Bytes(u8 *data, u8 len)
{
	u8 tmpData = 0;
	static u8 tmpLen = 0;
	Write_To_Send_Queue(data, len);
	if (Read_From_Send_Queue(&tmpData))
	{
		Uart_TX_Byte((u8)tmpData);
	}
	return 1;
}

// 串口中断
void UART_Handler()
{
	u8 tmpData = 0;
	volatile u8 i = 0;
	volatile bool failMark = false;
	volatile u16 Header = 0;
	static u8 amount = 0;
	static u8 step = 0;
	volatile u8 crcData1 = 0;
	volatile u8 crcData2 = 0;
	volatile u8 leadCode = 0;
	volatile u8 num = 0;
	static u8 leadByteIndex = 0;
	static u8 leadTwoByte[2] = {0, 0};

	if (Uart_Get_Int_Pending(UART_RX_FINISH_INT_PENDING))
	{
		if (step == 0)
		{

			leadTwoByte[leadByteIndex++] = Uart_RX_Byte();
		}
		else
		{
			UartMsgBuf.revBuf[UartMsgBuf.revcLen++] = Uart_RX_Byte();
		}

		switch (step)
		{

		case 0:
			if (leadByteIndex == 2)
			{
				leadByteIndex = 0;
				Header = *(u16 *)(leadTwoByte);
				amount = (Header >> 4) & 0x07;
				leadCode = leadTwoByte[1] & 0x1F;
				if (leadCode != 0x0F)
				{
					UartMsgBuf.revcLen = 0;
					break;
				}
				UartMsgBuf.revBuf[0] = leadTwoByte[0];
				UartMsgBuf.revBuf[1] = leadTwoByte[1];
				UartMsgBuf.revcLen = 2;
				step = 1;
			}
			break;
		case 1:
			// 第七第八个字节必须是 41 6B校验
			if (UartMsgBuf.revcLen == 8)
			{
				crcData1 = UartMsgBuf.revBuf[6];
				crcData2 = UartMsgBuf.revBuf[7];
				if (crcData1 != 0x41 || crcData2 != 0x6B)
				{
					failMark = true;
				}
			}

			if (UartMsgBuf.revcLen == amount * 4 + 2)
			{
				UartMsgBuf.revcLen = 0;
				// 收到非0x416B则忽略消息不做处理
				if (!failMark)
				{
					UartMsgBuf.revFinish = true;
				}
				else
				{
					UartMsgBuf.revFinish = false;
					failMark = false;
				}
				step = 0;
			}
			break;
		}
		Uart_Clear_Pending(UART_RX_FINISH_INT_PENDING);
	}

	if (Uart_Get_Int_Pending(UART_TX_FINISH_INT_PENDING))
	{
		Uart_Clear_Pending(UART_TX_FINISH_INT_PENDING);
		if (Read_From_Send_Queue(&tmpData))
		{
			Uart_TX_Byte(tmpData);
		}
	}
}

static bool Get_Memmory(u8 byte, u8 **pp)
{
	u8 *Mem = (u8 *)malloc(sizeof(u8) * byte);
	return (Mem == NULL) ? false : true;
}

static void Release_Memmory(u8 *addr)
{
	free(addr);
	addr = NULL;
}

//pd发送数据
static void Pd_Send_Vdo(const unsigned int *data, int count)
{
    unsigned char *pp;
    int i;
    int len;
    uint8_t *pdTxBuff = NULL;
		uint8_t *pdTxBuf = NULL;
    int send_c=count;
    pp=(unsigned char *)data;
		pdTxMessageSummary->packet_start = PD_SOP;
		pdTxMessageSummary->mesg_class = PD_DATA_CLASS;
		pdTxMessageSummary->mesg_type = PD_Vendor_Defined;

		pdTxBuf = Pd_Get_Tx_Buff_Start_Address(FALSE);
		pdTxBuff = Pd_Get_Tx_Buff_Start_Address(FALSE);    
		pdTxBuff+=2;
    
    while(send_c>0)
    {
        len=4;
       while(len>0)
       {
				 *pdTxBuff=*pp;
				 pdTxBuff++;
				 pp++;
				 len--;
        } 
        send_c--;
    }
    Pd_Tx_Message(count,pdTxBuf);	
} 

// 串口逻辑
static bool Analyse_Uart_Msg(usr_uart_msg_t *UartMsg)
{
	u16 *Header = NULL;
	u8 amount = 0;
	u32 uartMsgAddr = 0;
	u32 pUartMsgAddr[6] = {0};

	u8 seq = 0;
	u8 seqUartMsg[64] = {0};
	u8 k = 0;

	Header = (u16 *)UartMsg->revBuf;

	amount = (*Header >> 4) & 0x07;

	// 小端转换
	for (int j = amount * 4 + 2 - 1; j >= 0; j--)
	{
		seqUartMsg[j] = UartMsg->revBuf[(amount * 4 + 2 - 1) - j];
	}
	// 数据格式转换为32位
	for (int i = 0; i < amount; i++)
	{
		pUartMsgAddr[i] = *((u32 *)(seqUartMsg) + amount - i - 1);
	}
	// 发送PD包
	Pd_Send_Vdo(pUartMsgAddr, amount);

	return true;
}

// vdm通信逻辑
static bool Analyse_UnVdm_Msg(u32 *vdoData)
{
	u8 tmpUartSendMsg[64] = {0};
	int i = 0;
	u8 tid = 0;
	static u8 *pTmpUartSendMsg;
	static bool uartSndSuccess = false;
	pTmpUartSendMsg = tmpUartSendMsg;

	tmpUartSendMsg[0] = pUartMsgBuf->RecUnVdmMsgHeader >> 8;
	tmpUartSendMsg[1] = pUartMsgBuf->RecUnVdmMsgHeader;

	for (int j = 0; j < pUartMsgBuf->recUnVdmVdoAmount; j++)
	{
		for (i = 0; i < 4; i++)
		{
			tmpUartSendMsg[2 + tid] = *(vdoData + j) >> 8 * (3 - i);
			tid++;
		}
		i = 0;
	}
	tid = 0;

	Usr_Send_Bytes(tmpUartSendMsg, 2 + pUartMsgBuf->recUnVdmVdoAmount * 4);

	return 1;
}

void Init_Usr_UnVdm(void)
{
	pdTxBuff = Pd_Get_Tx_Buff_Start_Address(FALSE);
	pdRxBuff = Pd_Get_Rx_Buff_Start_Address(FALSE);

	pdRxMessageSummary = Pd_Init_Rx_Message_Summary(); // get rx message summary
	pdTxMessageSummary = Pd_Init_Tx_Message_Summary(); // get tx message summary
}

// PD二次开发
bool User_Pd_Policy_Hook(void)
{
	u32 vdmHeader = 0;
	u16 Header = 0;
	static u8 amountBit = 0;
	u8 vdmDefineMsgBit;
	u32 testUnVdmMsg = 0;
	u16 SVID = 0;
	u16 VERFIY = 0;
	u16 enterUartModeField = 0;

	u32 tempData = 0;
	u8 sop1ProductType = 0; // bit29-27   3
	u8 cableLatency = 0;	// bit16-13      4
	u8 maxVol = 0;			// bit10-9             2
	u8 maxCur = 0;			// bit6-5              2
	u8 usbHighestSpeed = 0; // bit2-0     3

	// 判断属于PD接收命令
	if (Pd_Is_Event_Happen(PD_RX_MESSAGE_SUCCESS_EVENT))
	{
		// 判断属于Sink设备
		if (PD_SOP == pdRxMessageSummary->packet_start)
		{
			if (PD_SOP == pdRxMessageSummary->packet_start && PD_Vendor_Defined == pdRxMessageSummary->mesg_type)
			{
				//					Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);
				pdRxBuff = Pd_Get_Rx_Buff_Start_Address(FALSE);
				vdmHeader = *(u32 *)(pdRxBuff + 2);
				Header = *(u16 *)(pdRxBuff);
				// 将HeaderMsg取出
				pUartMsgBuf->RecUnVdmMsgHeader = Header;
				vdmDefineMsgBit = vdmHeader & 0x001F;
				// vdo数目
				amountBit = (Header & 0x7000) >> 12;
				pUartMsgBuf->recUnVdmVdoAmount = amountBit;

				testUnVdmMsg = vdmHeader & 0x00008000;
				testUnVdmMsg = testUnVdmMsg >> 15;

				enterUartModeField = *(u16 *)(pdRxBuff + 2);
				SVID = *(u16 *)(pdRxBuff + 4);
				VERFIY = *(u16 *)(pdRxBuff + 8);

				// 这个加上VDM与UVDM的判断
				if (testUnVdmMsg == 0 && SVID == 0x291A && VERFIY == 0x416B)
				{
					Pd_Clear_Event(PD_RX_MESSAGE_SUCCESS_EVENT);
					// 根据UVDM消息初始化串口
					if (enterUartModeField == 0)
					{
						pUartMsgBuf->IsInitUart = true;
					}
					// 将VDM的数据保存下来
					for (int i = 0; i < amountBit; i++)
					{
						recUnVdmMsgBufArray[i] = *(u32 *)(pdRxBuff + 2 + i * 4);
					}

					pUartMsgBuf->IsRecUnVdmMsg = true;
					return true;
				}
			}
		}
	}
	return FALSE;
}

// 通信总策略逻辑
void UnVdm_Com_Policy(void)
{
	u8 i = 0;
	static bool justRunOnly = true;

	if (justRunOnly == true)
	{
		Init_Usr_UnVdm();
		justRunOnly = false;
	}

	if (UartMsgBuf.IsInitUart)
	{
		Usr_Uart_Init(115200);
		UartMsgBuf.IsInitUart = false;
	}

	if (UartMsgBuf.revFinish == true)
	{
		UartMsgBuf.revFinish = false;
		Analyse_Uart_Msg(pUartMsgBuf);
	}

	if (pUartMsgBuf->IsRecUnVdmMsg)
	{
		pUartMsgBuf->IsRecUnVdmMsg = false;
		Analyse_UnVdm_Msg(recUnVdmMsgBufArray);
	}
}
