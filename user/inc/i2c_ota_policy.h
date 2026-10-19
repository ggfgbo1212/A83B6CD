/****************************************************************************
 * @copyright Copyright(C) 2020-2026 Ismartware Limited. All rights reserved.
 * @file i2c_ota_policy.h
 * @brief I2C slave OTA handler using the vacant register 0xAA.
 *
 * Host protocol (two separate transactions):
 *   T1 write command : START + addr(W) + 0xAA + 01 C0 00 2B 08 + STOP
 *   T2 read ACK      : START + addr(W) + 0xAA + Sr + addr(R) + 7 bytes + STOP
 *   ACK frame        : AA 03 00 FF FF <crcLo> <crcHi>
 *                      crc = CRC-16, poly 0x8005, init 0x0000, no reflection
 *   After the ACK is clocked out the MCU resets so the vendor bootloader runs.
 *
 * Adapted from the working vendor reference project (my/i2c_ota_slave) - it
 * runs a standalone polling-mode I2C slave; this port folds the same sequence
 * into the existing interrupt-driven I2CS_Handler() so that register access
 * for every address other than 0xAA keeps its original behaviour byte for byte.
 *
 * The hooks below deliberately do NOT touch I2C/GPIO/ADC setup: pin muxing,
 * pull-ups, slave address and FIFO trigger levels all stay exactly as
 * I2C_Drive_Init() and ADC_Config_Init() configured them.
 *
 * IMPORTANT: in the RX branch of I2CS_Handler(), I2c_Ota_On_Rx_Byte() must be
 * called WITHOUT clearing I2C_S_RX_DATA_PENGING - that clear writes the whole
 * S_STATUS register and would wipe the frame's register-address / byte-length
 * fields while the frame is still being received.
 ****************************************************************************/
#ifndef I2C_OTA_POLICY_H
#define I2C_OTA_POLICY_H

#include "system.h"

/* I2C register address reserved for the OTA channel (vacant in this project). */
#define I2C_OTA_REG_ADDR    (0xAAU)

/**
 * @brief  Call from the RX branch of I2CS_Handler() when the register address
 *         is I2C_OTA_REG_ADDR (do NOT clear the RX pending flag).
 *         Parks the RX_DATA interrupt so the payload stays in the RX FIFO until
 *         the STOP, drops any FINISH flag left by earlier transactions (write
 *         transactions never clear it - it would fire as soon as the FINISH
 *         interrupt is enabled and cut the frame short), and enables the
 *         FINISH interrupt so the STOP event becomes visible.
 */
void I2c_Ota_On_Rx_Byte(void);

/**
 * @brief  Call from I2CS_Handler() on every entry. At the command frame's STOP
 *         it checks the payload against I2c_Slave_Get_RX_Byte_Length() and the
 *         fixed 5-byte command, pre-loads the 7-byte ACK into the TX FIFO,
 *         then releases the RX hold. Safe to call unconditionally - it returns
 *         immediately unless an OTA frame is open AND FINISH is set.
 */
void I2c_Ota_On_Finish(void);

/**
 * @brief  Call from the TX branch of I2CS_Handler() when the register address is
 *         I2C_OTA_REG_ADDR. The ACK is already in the TX FIFO (pre-loaded at
 *         the command frame's STOP); this only arms the deferred reset that
 *         I2c_Ota_Policy_Run() executes from the main loop.
 */
void I2c_Ota_On_Tx_Request(void);

/**
 * @brief  Call from the main loop. Once the ACK has been served, waits for the
 *         master to clock it out and then resets the MCU so the vendor
 *         bootloader takes over. Never returns when the reset is armed.
 */
void I2c_Ota_Policy_Run(void);

#endif
