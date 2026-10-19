/****************************************************************************
 * @copyright Copyright(C) 2020-2026 Ismartware Limited. All rights reserved.
 * @file i2c_ota_policy.c
 * @brief I2C slave OTA trigger on the vacant register 0xAA.
 *
 *   master writes : START + addr(W) + 0xAA + 01 C0 00 2B 08 + STOP
 *   master reads  : START + addr(W) + 0xAA + Sr + addr(R) + 7 bytes + STOP
 *   slave replies : AA 03 00 FF FF 3D 8C, then resets into the bootloader
 *
 * Adapted from the working vendor reference (my/i2c_ota_slave), which serves
 * the whole exchange from one polling loop; the same sequence of actions is
 * kept here, driven by the hooks slave_policy_drive.c calls into:
 *
 *   - the command frame is left in the RX FIFO until its STOP. The RX_DATA
 *     interrupt is parked for the duration of the frame (I2c_Ota_On_Rx_Byte)
 *     so the hardware keeps the S_STATUS register-address / byte-length fields
 *     that describe the frame. Servicing RX events mid-frame is what broke the
 *     previous port: I2c_Slave_Clear_Pending() writes the whole S_STATUS
 *     register with the pending mask, so the address field reads back 0
 *     afterwards and both the RX branch and the STOP check lose the frame.
 *   - at the STOP (I2c_Ota_On_Finish) the frame is accepted when
 *     I2c_Slave_Get_RX_Byte_Length() >= 5; the 5 payload bytes are read out of
 *     the FIFO, compared against the fixed command, and the 7-byte ACK is
 *     pre-loaded into the TX FIFO - ready before the master's read begins.
 *   - any FINISH flag left behind by earlier transactions is dropped when the
 *     frame starts: write transactions never clear it (only the read path
 *     does), and it would fire the moment the FINISH interrupt is enabled,
 *     cutting this frame short before its real STOP.
 *   - the master's read raises TX_DATA (I2c_Ota_On_Tx_Request); the reset is
 *     armed and executed from the main loop (I2c_Ota_Policy_Run).
 *
 * Register access for every address other than 0xAA is untouched.
 *
 * The command frame is fixed, so the reply is fixed too and is held as a
 * constant rather than recomputed. 3D 8C is the CRC-16 of AA 03 00 FF FF
 * (poly 0x8005, init 0x0000, no reflection).
 *
 * Neither I2C/GPIO/ADC setup nor the slave address is touched here: pin muxing,
 * pull-ups and FIFO trigger levels stay as I2C_Drive_Init() left them.
 ****************************************************************************/
#include "i2c_ota_policy.h"
#include "zr_i2c.h"
#include "zr_systick.h"
#include "zr_nvic.h"

#define OTA_RX_PAYLOAD_LEN  (5U)    /* 01 C0 00 2B 08 */
#define OTA_ACK_LEN         (7U)    /* AA 03 00 FF FF 3D 8C */

/* The one command we act on. */
static const u8 CMD_RESET[OTA_RX_PAYLOAD_LEN] =
{
    0x01U, 0xC0U, 0x00U, 0x2BU, 0x08U
};

static const u8 OTA_ACK[OTA_ACK_LEN] =
{
    0xAAU, 0x03U, 0x00U, 0xFFU, 0xFFU, 0x3DU, 0x8CU
};

static u8 otaAckReady     = 0;  /* ACK pre-loaded, owed to the master's read */
static u8 otaResetPending = 0;  /* read started, reset from the main loop    */
static u8 otaRxHold       = 0;  /* RX interrupt parked for an OTA frame      */
static u8 otaRxBuf[OTA_RX_PAYLOAD_LEN];

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

/* -------------------------------------------------------------------------
 * ISR / main-loop hooks
 * ------------------------------------------------------------------------- */

void I2c_Ota_On_Rx_Byte(void)
{
    /* First data byte of a frame on 0xAA: park the RX_DATA interrupt so the
     * payload stays in the FIFO and the frame descriptor in S_STATUS stays
     * intact until the STOP. */
    if (otaRxHold == 0)
    {
        otaRxHold = 1;

        /* Drop any FINISH flag a previous transaction has left behind. Write
         * transactions never clear it (only the read path does), and with the
         * FINISH interrupt enabled below it would fire right away and gut this
         * frame mid-way: On_Finish would run on an incomplete payload, flush
         * the FIFO and disable the interrupt before the real STOP arrives. */
        I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);

        I2c_Slave_Int_Disable(I2C_S_RX_DATA_INT_EN);
    }

    /* The STOP is what ends the frame. */
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

    /* No command frame open: this STOP ends the master's read of the ACK (or
     * a normal register access). Leave it to the read bookkeeping further down
     * I2CS_Handler(), which is what clears the TX FIFO afterwards. */
    if (otaRxHold == 0)
    {
        return;
    }

    /* The STOP ends the command frame. The hold is only taken when the frame
     * was addressed to 0xAA, so the payload itself is the gate: accept once
     * the five bytes are in - counted by the hardware or by the FIFO level
     * (a pending clear may have reset the byte counter). */
    rxLen = I2c_Slave_Get_RX_Byte_Length();

    if ((rxLen >= OTA_RX_PAYLOAD_LEN) || (I2c_Slave_Get_RX_FIFO_Left() >= OTA_RX_PAYLOAD_LEN))
    {
        for (i = 0; i < OTA_RX_PAYLOAD_LEN; i++)
        {
            otaRxBuf[i] = (u8)ZR_I2C->S_RX_DATA;
        }

        if (compare_data(CMD_RESET, otaRxBuf, OTA_RX_PAYLOAD_LEN) == 0)
        {
            /* Command accepted: hand the master a FIFO that holds exactly
             * the ACK, loaded before its read transaction starts. */
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

    /* Frame over: drop leftovers, release the RX hold, then clear pendings. */
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

    /* Master has started reading the ACK. The bytes have been in the TX FIFO
     * since the command frame's STOP, so there is nothing to load here -
     * only the reset to arm. */
    otaAckReady = 0;
    otaResetPending = 1;
}

void I2c_Ota_Policy_Run(void)
{
    if (otaResetPending == 0)
    {
        return;
    }

    /* Give the master time to clock the seven ACK bytes out before we
     * disappear: the host's read transaction is still in flight here. No UART
     * logging is done on purpose - this port deliberately leaves serial.c and
     * the I2C pins exactly as the 5412 project had them. */
    Systick_Delay_Ms(3);
    I2c_Slave_Int_Disable(I2C_S_TX_DATA_INT_EN | I2C_S_RX_DATA_INT_EN | I2C_S_FINISH_INT_EN);
    Nvic_Irq_Disable(I2CS_IRQn);    
    NVIC_SystemReset();
}
