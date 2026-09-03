/****************************************************************************
 * @copyright Copyright(C) 2020-2026 Ismartware Limited. All rights reserved.
 * @file i2c_ota_slave.c
 * @brief I2C slave OTA protocol handler for SW356x (polling mode)
 *
 * Host protocol (two separate transactions):
 *   Transaction 1 (write command):
 *     START + addr(W) + 0xAA + 01 C0 00 2B 08 + STOP
 *   Transaction 2 (read ACK):
 *     START + addr(W) + 0xAA + Sr + addr(R) + read 7 bytes + STOP
 *
 * Polling flow (no interrupts):
 *   Run() polls FINISH_PENGING: rxLen>=5 -> compare payload -> load TX FIFO
 *   Run() polls TX_DATA_PENGING: s_flag set -> delay -> NVIC_SystemReset()
 *
 * @author SW team
 ****************************************************************************/
#include "i2c_ota_slave.h"
#include "zr_i2c.h"
#include "zr_gpio.h"
#include "zr_rcc.h"
#include "zr_nvic.h"
#include "zr_systick.h"
#include "zr_adc.h"
#include "system.h"
#include "api.h"
#include <stdio.h>

/* -------------------------------------------------------------------------
 * OTA protocol constants
 * ------------------------------------------------------------------------- */
#define OTA_SOF             (0xAAU)
#define OTA_FRAME_TYPE_ACK  (0x03U)
#define OTA_ACK_SUCCESS     (0x00U)
#define OTA_ACK_PAGE_CMD    (0xFFFFU)

#define OTA_RX_REG_ADDR     (OTA_SOF) /* I2C register address = SOF byte  */
#define OTA_RX_PAYLOAD_LEN  (5U)      /* type + cmd + paramLen + crc(2)   */
#define OTA_ACK_LEN         (7U)      /* SOF + type + status + page(2) + crc(2) */

/* Known reset command frame: AA 01 C0 00 [CRC-16/ANSI of AA 01 C0 00]
 * Index 0 = SOF (captured by hardware as register address, not in RX FIFO)
 * Index 1..5 = payload bytes to compare against rx_buf                     */
static const uint8_t CMD_RESET_0XC0_DATA[OTA_RX_PAYLOAD_LEN + 1] =
{
    0xAAU, /* [0] SOF / register address */
    0x01U, /* [1] frame type: CMD        */
    0xC0U, /* [2] command: RESET         */
    0x00U, /* [3] paramLen               */
    0x2BU, /* [4] CRC-16 low             */
    0x08U  /* [5] CRC-16 high            */
};

/* -------------------------------------------------------------------------
 * Internal helpers
 * ------------------------------------------------------------------------- */

/**
 * @brief  CRC-16: poly=0x8005, init=0x0000, no reflection
 * @param[in]  data  data buffer
 * @param[in]  len   number of bytes
 * @retval CRC-16 value
 */
static uint16_t ota_crc16_calc(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0x0000;
    const uint16_t poly = 0x8005;
    uint16_t i = 0;
    uint8_t  j = 0;

    for (i = 0; i < len; i++)
    {
        crc ^= (uint16_t)data[i] << 8;
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x8000U)
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

/**
 * @brief  Compare two byte arrays
 * @param[in]  ref   reference data
 * @param[in]  data  received data
 * @param[in]  len   number of bytes to compare
 * @retval 0 if equal, 1 if different
 */
static uint8_t compare_data(const uint8_t *ref, const uint8_t *data, uint8_t len)
{
    uint8_t i = 0;

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
 * Module state
 * ------------------------------------------------------------------------- */
static uint8_t  s_otaFlag            = 0;
static uint8_t  s_rxBuf[OTA_RX_PAYLOAD_LEN];
static uint8_t  s_txBuf[OTA_ACK_LEN];

/* -------------------------------------------------------------------------
 * Public API
 * ------------------------------------------------------------------------- */

static void I2c_Ota_Gpio_Init(void)
{
    Gpio_Function_Select(GPIO0, FUNCTION_SEL6);
    Gpio_Function_Select(GPIO1, FUNCTION_SEL6);
    Gpio_Mos_Opendrain_Cfg(GPIO0, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Mos_Opendrain_Cfg(GPIO1, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
//    Gpio_Pulldown_Pullup_Cfg(GPIO2, PULLDOWN_DIS, PULLUP_EN);
//    Gpio_Pulldown_Pullup_Cfg(GPIO3, PULLDOWN_DIS, PULLUP_EN);
}

/**
 * @brief  Init I2C slave for OTA (GPIO + clock, polling mode — no interrupts)
 */
void I2c_Ota_Slave_Init(void)
{
    uint8_t      devAddr = 0x42;
    uint16_t     vdValue = 0;
    adc_config_t adcCfg;

    Rcc_Module_Rst(I2C_ID);
    Rcc_Module_Clk_Enable(GPIO_ID | I2C_ID | ADC_ID);
    Rcc_Module_Rst_Release(I2C_ID);

    I2c_Ota_Gpio_Init();

    /* VD pin sample, get I2C slave address from external resistor */
    Adc_Get_Config(&adcCfg);
    adcCfg.adc2Source = ADC2_SIG_SRC_VD;
    adcCfg.adc2Pullup = ADC_PULLUP_CURRENT_20UA; /* 20uA pullup, 400mV/20K matching */
    adcCfg.adc2SampleRate = ADC_SAMPLE_RATE_1KH;
    Adc_Set_Config(&adcCfg);
    Adc_Start_Adc_Channel(ADC_CHL_2);
    Adc_Enabled();
    Systick_Delay_Ms(25);

    vdValue = Adc_Get_Channel_Data(ADC_CHL_2);
//    printf("[OTA] VD value: %d\r\n", vdValue);
    if (vdValue < 400)
    {
        devAddr = 0x3D; /* 0-20K */
    }
    else if (vdValue < 800)
    {
        devAddr = 0x3E; /* 20-40K */
    }
    else if (vdValue < 1200)
    {
        devAddr = 0x3F; /* 40-60K */
    }
    else if (vdValue < 1600)
    {
        devAddr = 0x40; /* 60-80K */
    }
    else if (vdValue < 2000)
    {
        devAddr = 0x41; /* 80-100K */
    }
    else
    {
        devAddr = 0x42;
    }
    printf("[OTA] device address: 0x%02X\r\n", devAddr);
    I2c_Slave_Device_Addr_Cfg(devAddr);
    I2c_Slave_RX_FIFO_Trigger_Level_Cfg(1);
	I2c_Slave_TX_FIFO_Trigger_Level_Cfg(8);

    // I2c_Slave_Int_Enable(I2C_S_TX_DATA_INT_EN | I2C_S_FINISH_INT_EN);
    // // set to highest priority - > PRIORITY_0
    // Nvic_Irq_Enable(I2CS_IRQn, PRIORITY_0);

    I2c_Slave_Wait_Enable();
    I2c_Slave_Enable();
    I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING | I2C_S_TX_DATA_PENGING | I2C_S_FINISH_PENGING
                            | I2C_S_RX_OVERFLOW_PENGING | I2C_S_TX_UNDERFLOW_PENGING | I2C_S_RX_FIFO_PENGING
                            | I2C_S_TX_FIFO_PENGING | I2C_S_BUS_ERROR_PENGING);
}

/**
 * @brief  OTA run — call in main loop after Device_Run().
 *         Polls I2C slave status register; no interrupts required.
 */
void I2c_Ota_Slave_Run(void)
{
    uint16_t crc16   = 0;
    uint8_t  rx_len  = 0;
    uint8_t  i       = 0;
    u32      pending = ZR_I2C->S_STATUS; /* snapshot all pending flags at once */

    /* --- FINISH: STOP received (T1 write command complete) --- */
    if (pending & I2C_S_FINISH_PENGING)
    {
        if (I2c_Slave_Get_Register_Address() == OTA_RX_REG_ADDR)
        {
            rx_len = I2c_Slave_Get_RX_Byte_Length();

            if (rx_len >= OTA_RX_PAYLOAD_LEN)
            {
                /* drain RX FIFO */
                for (i = 0; i < OTA_RX_PAYLOAD_LEN; i++)
                {
                    s_rxBuf[i] = (uint8_t)ZR_I2C->S_RX_DATA;
                }
                while (I2c_Slave_Get_RX_FIFO_Left() > 0)
                {
                    (void)ZR_I2C->S_RX_DATA;
                }

                /* compare against known reset command (skip SOF byte at index 0) */
                if (compare_data(&CMD_RESET_0XC0_DATA[1], s_rxBuf, OTA_RX_PAYLOAD_LEN) == 0)
                {
                    /* build ACK frame with dynamic CRC */
                    s_txBuf[0] = OTA_SOF;
                    s_txBuf[1] = OTA_FRAME_TYPE_ACK;
                    s_txBuf[2] = OTA_ACK_SUCCESS;
                    s_txBuf[3] = (uint8_t)(OTA_ACK_PAGE_CMD & 0xFFU);
                    s_txBuf[4] = (uint8_t)((OTA_ACK_PAGE_CMD >> 8) & 0xFFU);
                    crc16      = ota_crc16_calc(s_txBuf, 5U);
                    s_txBuf[5] = (uint8_t)(crc16 & 0xFFU);
                    s_txBuf[6] = (uint8_t)((crc16 >> 8) & 0xFFU);

                    /* reset TX FIFO and load ACK (ready before master's T2 read) */
                    I2c_Slave_TX_FIFO_Reset();
                    while (ZR_I2C->S_FIFO & I2C_S_TX_FIFO_RESET);
                    for (i = 0; i < OTA_ACK_LEN; i++)
                    {
                        ZR_I2C->S_TX_DATA = s_txBuf[i];
                    }

                    s_otaFlag = 1;
                }
            }
        }
        ZR_I2C->S_STATUS = I2C_S_FINISH_PENGING;
        // I2c_Slave_Clear_Pending(I2C_S_FINISH_PENGING);
    }

    /* --- TX_DATA: master starts reading ACK (T2 read phase) --- */
    if (pending & I2C_S_TX_DATA_PENGING)
    {
        ZR_I2C->S_STATUS = I2C_S_TX_DATA_PENGING;
        if (s_otaFlag)
        {
            printf("[OTA] goto bootloader\r\n");
            Systick_Delay_Ms(3);
            NVIC_SystemReset();
        }
    }
}
