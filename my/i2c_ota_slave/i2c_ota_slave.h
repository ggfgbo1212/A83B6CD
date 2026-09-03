/****************************************************************************
 * @copyright Copyright(C) 2020-2026 Ismartware Limited. All rights reserved.
 * @file i2c_ota_slave.h
 * @brief I2C slave OTA protocol handler interface
 ****************************************************************************/
#ifndef I2C_OTA_SLAVE_H
#define I2C_OTA_SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Init I2C slave for OTA: configure GPIO/I2C, enable interrupt,
 *         pre-load the fixed ACK into the TX FIFO. Call once before main loop.
 */
void I2c_Ota_Slave_Init(void);

/**
 * @brief  OTA run, call in main loop after Device_Run()
 *         Triggers system reset when master sends valid reset command and ACK is complete
 */
void I2c_Ota_Slave_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* I2C_OTA_SLAVE_H */
