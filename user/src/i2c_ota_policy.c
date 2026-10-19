/*
***************************************************************************
* Copyright (C) 2022-2026 Ismartware Limited. All rights reserved.
* File name: i2c_ota_policy.c
* Description: I2C从机OTA功能，通过空闲寄存器0xAA接收特定命令并复位进入Bootloader
* Version: V1.0
* Date:
* History:
***************************************************************************
*/
// 主机写命令：START + 地址(写) + 0xAA + 01 C0 00 2B 08 + STOP
// 主机读应答：START + 地址(写) + 0xAA + Sr + 地址(读) + 7字节 + STOP
// 从机应答：  AA 03 00 FF FF 3D 8C，发送完成后复位进入Bootloader
// 说明：命令帧在STOP前保留在RX FIFO，期间挂起RX中断，避免清除S_STATUS导致帧信息丢失；
//       0xAA以外的寄存器访问逻辑保持不变。
#include "i2c_ota_policy.h"
#include "zr_i2c.h"
#include "zr_systick.h"
#include "zr_nvic.h"

#define OTA_RX_PAYLOAD_LEN  (5U)    // 命令长度：01 C0 00 2B 08
#define OTA_ACK_LEN         (7U)    // 应答长度：AA 03 00 FF FF 3D 8C

// 唯一的有效命令
static const u8 CMD_RESET[OTA_RX_PAYLOAD_LEN] =
{
    0x01U, 0xC0U, 0x00U, 0x2BU, 0x08U
};

// 固定应答数据
static const u8 OTA_ACK[OTA_ACK_LEN] =
{
    0xAAU, 0x03U, 0x00U, 0xFFU, 0xFFU, 0x3DU, 0x8CU
};

static u8 otaAckReady     = 0;  // 应答已预装载，等待主机读取
static u8 otaResetPending = 0;  // 主机已开始读取，复位待执行
static u8 otaRxHold       = 0;  // OTA帧接收中，RX中断已挂起
static u8 otaRxBuf[OTA_RX_PAYLOAD_LEN];  // 接收到的命令数据

// 数据比较，完全相同返回0
static u8 compare_data(const u8 *ref, const u8 *data, u8 len)
{
    u8 i = 0;

    for (i = 0; i < len; i++)
    {
        if (ref[i] != data[i])
        {
            return 1;
        }
    }
    return 0;
}

//------------------- 中断与主循环回调 -------------------

void I2c_Ota_On_Rx_Byte(void)
{
    // 0xAA命令帧首字节：挂起RX中断，数据保留在FIFO，帧信息保持到STOP
    if (otaRxHold == 0)
    {
        otaRxHold = 1;

        // 清除上次事务遗留的FINISH标志，否则使能后会立即误触发，导致帧被提前处理
        I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);

        I2c_Slave_Int_Disable(I2C_S_RX_DATA_INT_EN);
    }

    // STOP到来时结束本帧
    I2c_Slave_Int_Enable(I2C_S_FINISH_INT_EN);
}

void I2c_Ota_On_Finish(void)
{
    u8 guard = 0;
    u8 i = 0;
    u8 rxLen = 0;

    if (I2c_Slave_Get_Pending(I2C_S_FINISH_PENGING) == 0)
    {
        return;
    }

    // 非OTA命令帧：可能是主机读取应答或普通寄存器访问，交由后续读处理
    if (otaRxHold == 0)
    {
        return;
    }

    // STOP到来，检查命令长度（FIFO中至少5字节）
    rxLen = I2c_Slave_Get_RX_Byte_Length();

    if ((rxLen >= OTA_RX_PAYLOAD_LEN) || (I2c_Slave_Get_RX_FIFO_Left() >= OTA_RX_PAYLOAD_LEN))
    {
        for (i = 0; i < OTA_RX_PAYLOAD_LEN; i++)
        {
            otaRxBuf[i] = (u8)ZR_I2C->S_RX_DATA;
        }

        if (compare_data(CMD_RESET, otaRxBuf, OTA_RX_PAYLOAD_LEN) == 0)
        {
            // 命令匹配：预装载7字节应答，等待主机读取
            I2c_Slave_TX_FIFO_Reset();
            while (((ZR_I2C->S_FIFO & I2C_S_TX_FIFO_RESET) != 0U) && (guard < 100U))
            {
                guard++;
            }

            for (i = 0; i < OTA_ACK_LEN; i++)
            {
                ZR_I2C->S_TX_DATA = OTA_ACK[i];
            }

            otaAckReady = 1;
        }
    }

    // 帧结束：清空FIFO残留，恢复RX中断和状态
    while (I2c_Slave_Get_RX_FIFO_Left() > 0)
    {
        (void)ZR_I2C->S_RX_DATA;
    }
    I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING);
    I2c_Slave_Int_Enable(I2C_S_RX_DATA_INT_EN);
    otaRxHold = 0;

    I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
    I2c_Slave_Int_Disable(I2C_S_FINISH_INT_EN);
}

void I2c_Ota_On_Tx_Request(void)
{
    if (otaAckReady == 0)
    {
        return;
    }

    // 主机开始读取应答：标记复位待执行
    otaAckReady = 0;
    otaResetPending = 1;
}

void I2c_Ota_Policy_Run(void)
{
    if (otaResetPending == 0)
    {
        return;
    }

    // 等待应答发送完成后再复位进入Bootloader
    Systick_Delay_Ms(3);
    I2c_Slave_Int_Disable(I2C_S_TX_DATA_INT_EN | I2C_S_RX_DATA_INT_EN | I2C_S_FINISH_INT_EN);
    Nvic_Irq_Disable(I2CS_IRQn);    
    NVIC_SystemReset();
}
