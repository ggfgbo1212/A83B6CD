#ifndef SW3526_REG_H
#define SW3526_REG_H

#define VERSION_INFO_REG              0x01
#define VERSION_INFO_REG_01           VERSION_INFO_REG

#define IC_VERSION_POS                0x00
#define IC_VERSION_MSK                (0x03 << IC_VERSION_POS)

#define BUCK_OUT_VOL_H8_REG           0x03
#define BUCK_OUT_VOL_H8_REG_03        BUCK_OUT_VOL_H8_REG

#define BUCK_OUT_VOL_L4_REG           0x04
#define BUCK_OUT_VOL_L4_REG_04        BUCK_OUT_VOL_L4_REG

#define BUCK_OUT_VOL_L4_POS           0x04
#define BUCK_OUT_VOL_L4_MSK           (0x0f << BUCK_OUT_VOL_L4_POS)

#define BUCK_CUR_LIT_REG              0x05
#define BUCK_CUR_LIT_REG_05           BUCK_CUR_LIT_REG

#define BUCK_OUT_CUR_LIT_POS          0x00
#define BUCK_OUT_CUR_LIT_MSK          (0x3f << BUCK_OUT_CUR_LIT_POS)

#define PROTOCOL_INDICATION_REG       0x06
#define PROTOCOL_INDICATION_REG_06    PROTOCOL_INDICATION_REG

#define PROTOCOL_STA_POS              0x07
#define PROTOCOL_STA_MSK              (0x01 << PROTOCOL_STA_POS)

#define VOL_STA_POS                   0x06
#define VOL_STA_MSK                   (0x01 << VOL_STA_POS)

#define PD_VERSION_POS                0x04
#define PD_VERSION_MSK                (0x03 << PD_VERSION_POS)

#define PROTOCOL_INDICATION_POS       0x00
#define PROTOCOL_INDICATION_MSK       (0x0f << PROTOCOL_INDICATION_POS)

enum protocol
{
  QC2_0_PROCOTOL = 1,
  QC3_0_PROCOTOL,
	FCP_PROCOTOL,
	SCP_PROCOTOL,
	PD_FIX_PROTOCOL,
	PD_PPS_PROTOCOL,
	PE1_1_PROTOCOL,
	PE2_0_PROTOCOL,
	VOOC_PROTOCOL,
	SFCP_PROTOCOL,
	AFC_PROTOCOL,
};

#define SYS_STA_REG                     0x07
#define SYS_STA_REG_07                  SYS_STA_REG

#define PORT_STA_POS                    0x01
#define PORT_STA_MSK                    (0x01 << PORT_STA_POS)

#define BUCK_STA_POS                    0x00
#define BUCK_STA_MSK                    (0x01 << BUCK_STA_POS)

#define ABNORMAL_CASE_REG               0x0B
#define ABNORMAL_CASE_REG_0B            ABNORMAL_CASE_REG

#define VIN_OVP_STA_POS                 0x04
#define VIN_OVP_STA_MSK                 (0x01 << VIN_OVP_STA_POS)

#define OVER_TEMP_ALARM_STA_POS         0x02
#define OVER_TEMP_ALARM_STA_MSK         (0x01 << OVER_TEMP_ALARM_STA_POS)

#define OVER_TEMP_SHUTDOWN_STA_POS      0x01
#define OVER_TEMP_SHUTDOWN_STA_MSK      (0x01 << OVER_TEMP_SHUTDOWN_STA_POS)

#define SHORT_CIRCUIT_STA_POS            0x00
#define SHORT_CIRCUIT_STA_MSK            (0x01 << SHORT_CIRCUIT_STA_POS)

#define EN_I2C_REG                       0x12
#define EN_I2C_REG_12                    EN_I2C_REG

#define EN_I2C_WRITE_POS                 0x00
#define EN_I2C_WRITE_MSK                 (0xff << EN_I2C_WRITE_POS)

#define BUCK_FORCE_OFF_REG               0x13
#define BUCK_FORCE_OFF_REG_13            BUCK_FORCE_OFF_REG

#define BUCK_FORCE_TURNOFF_1S_POS        0x07
#define BUCK_FORCE_TURNOFF_1S_MSK        (0x01 << BUCK_FORCE_TURNOFF_1S_POS)

#define CC_UNDRIVE_BUCK_FORCE_OFF_POS    0x06
#define CC_UNDRIVE_BUCK_FORCE_OFF_MSK    (0x01 << CC_UNDRIVE_BUCK_FORCE_OFF_POS)

#define EN_LINE_COMOENSATION_POS         0x05
#define EN_LINE_COMOENSATION_MSK         (0x01 << EN_LINE_COMOENSATION_POS)

#define ADC_VIN_DATA_REG                 0x30
#define ADC_VIN_DATA_REG_30              ADC_VIN_DATA_REG

#define ADC_VOUT_DATA_REG                0x31
#define ADC_VOUT_DATA_REG_31             ADC_VOUT_DATA_REG

#define ADC_IOUT_DATA_REG                0x33
#define ADC_IOUT_DATA_REG_33             ADC_IOUT_DATA_REG

#define SET_ADC_REG                      0x3A
#define SET_ADC_REG_3A                   SET_ADC_REG

#define ADC_DATA_TYPE_POS                0x00
#define ADC_DATA_TYPE_MSK                (0x07 << ADC_DATA_TYPE_POS)

#define ADC_DATA_H8_REG                  0x3B
#define ADC_DATA_H8_REG_3B               ADC_DATA_H8_REG

#define ADC_DATA_L4_REG                  0x3C
#define ADC_DATA_L4_REG_3C               ADC_DATA_L4_REG

#define ADC_L4_BITS_POS                  0x00
#define ADC_L4_BITS_MSK                  (0x0f << ADC_L4_BITS_POS)

#define POWER_STA_REG                    0x68
#define POWER_STA_REG_68                 POWER_STA_REG

#define POWER_STA_POS                    0x00
#define POWER_STA_MSK                    (0x7f << POWER_STA_POS)

#define CC_STA_REG                       0x69
#define CC_STA_REG_69                    CC_STA_REG

#define CC2_STA_POS                      0x05
#define CC2_STA_MSK                      (0x01 << CC2_STA_POS)

#define CC1_STA_POS                      0x04
#define CC1_STA_MSK                      (0x01 << CC1_STA_POS)

#define PD_CMD_REQUEST_REG               0x70
#define PD_CMD_REQUEST_REG_70            PD_CMD_REQUEST_REG

#define EN_PD_CMD_SEND_POS               0x07
#define EN_PD_CMD_SEND_MSK               (0x01 << EN_PD_CMD_SEND_POS)

#define PD_CMD_POS                       0x00
#define PD_CMD_MSK                       (0x0f << PD_CMD_POS)

#define PD_SRC_CHANGE_REG_73             0x73
#define PD_SRC_CHANGE_POS                0x07
#define PD_SRC_CHANGE_MSK                (0x01 << PD_SRC_CHANGE_POS)  

#define SET_UNLOAD_VOUT_REG              0xA0
#define SET_UNLOAD_VOUT_REG_A0           SET_UNLOAD_VOUT_REG

#define SET_UNLOAD_VOUT_VOL_POS          0x06
#define SET_UNLOAD_VOUT_VOL_MSK          (0x03 << SET_UNLOAD_VOUT_VOL_POS)

#define SET_QUICK_CHARGE_6_REG           0xA2
#define SET_QUICK_CHARGE_6_REG_A2        SET_QUICK_CHARGE_6_REG

#define QC2_QC3_CABLE_COM_OFFSET_EN_POS  0x06
#define QC2_QC3_CABLE_COM_OFFSET_EN_MSK  (0x01 << QC2_QC3_CABLE_COM_OFFSET_EN_POS)

#define PD0_LINK_VIN_POS                 0x05
#define PD0_LINK_VIN_MSK                 (0x01 << PD0_LINK_VIN_POS)

#define SET_QUICK_CHARGE_5_REG           0xA4
#define SET_QUICK_CHARGE_5_REG_A4        SET_QUICK_CHARGE_5_REG

#define SCP_SELECT_POS                   0x06
#define SCP_SELECT_MSK                   (0x01 << SCP_SELECT_POS)

#define SET_PE2_0_MAX_VOL_POS            0x05
#define SET_PE2_0_MAX_VOL_MSK            (0x01 << SET_PE2_0_MAX_VOL_POS)

#define SET_LINE_COMPENSATION_REG        0xA6
#define SET_LINE_COMPENSATION_REG_A6     SET_LINE_COMPENSATION_REG

#define LINE_DROP_COMPENSATION_SET_POS   0x04
#define LINE_DROP_COMPENSATION_SET_MSK   (0x01 << LINE_DROP_COMPENSATION_SET_POS)

#define SET_POWER_REG                    0xA7
#define SET_POWER_REG_A7                 SET_POWER_REG

#define SETTING_POWER_POS                0x00
#define SETTING_POWER_MSK                (0x3f << SETTING_POWER_POS)

#define SET_QUICK_CHARGE_0_REG           0xA8
#define SET_QUICK_CHARGE_0_REG_A8        SET_QUICK_CHARGE_0_REG

#define EN_SCP_POS                       0x07
#define EN_SCP_MSK                       (0x01 << EN_SCP_POS)

#define EN_VOOC_POS                      0x06
#define EN_VOOC_MSK                      (0x01 << EN_VOOC_POS)

#define EN_SFCP_POS                      0x05
#define EN_SFCP_MSK                      (0x01 << EN_SFCP_POS)

#define EN_QC2_0_POS                     0x04
#define EN_QC2_0_MSK                     (0x01 << EN_QC2_0_POS)

#define EN_QC3_0_POS                     0x03
#define EN_QC3_0_MSK                     (0x01 << EN_QC3_0_POS)

#define EN_FCP_POS                       0x02
#define EN_FCP_MSK                       (0x01 << EN_FCP_POS)

#define EN_AFC_POS                       0x01
#define EN_AFC_MSK                       (0x01 << EN_AFC_POS)

#define EN_PE_POS                        0x00
#define EN_PE_MSK                        (0x01 << EN_PE_POS)

#define SET_QUICK_CHARGE1_REG            0xA9
#define SET_QUICK_CHARGE1_REG_A9         SET_QUICK_CHARGE1_REG

#define EN_PPS1_POS                      0x07
#define EN_PPS1_MSK                      (0x01 << EN_PPS1_POS)

#define EN_PPS0_POS                      0x06
#define EN_PPS0_MSK                      (0x01 << EN_PPS0_POS)

#define EN_PD2_0_V_POS                   0x05
#define EN_PD2_0_V_MSK                   (0x01 << EN_PD2_0_V_POS)

#define EN_PD1_5_V_POS                   0x04
#define EN_PD1_5_V_MSK                   (0x01 << EN_PD1_5_V_POS)

#define EN_PD1_2_V_POS                   0x03
#define EN_PD1_2_V_MSK                   (0x01 << EN_PD1_2_V_POS)

#define EN_PD_9V_POS                     0x02
#define EN_PD_9V_MSK                     (0x01 << EN_PD_9V_POS)

#define EN_PD_POS                        0x00
#define EN_PD_MSK                        (0x01 << EN_PD_POS)

#define SET_QUICK_CHARGE_2_REG           0xAA
#define SET_QUICK_CHARGE_2_REG_AA        SET_QUICK_CHARGE_2_REG

#define EN_DPDM_POS                      0x05
#define EN_DPDM_MSK                      (0x01 << EN_DPDM_POS)

#define MAX_OUT_VOL_EXCEPT_PD_POS        0x00
#define MAX_OUT_VOL_EXCEPT_PD_MSK        (0x03 << MAX_OUT_VOL_EXCEPT_PD_POS)

#define SET_QUICK_CHARGE_3_REG           0xAB
#define SET_QUICK_CHARGE_3_REG_AB        SET_QUICK_CHARGE_3_REG

#define PD_CUR_LIM_PROTECT_METHOD_POS    0x07
#define PD_CUR_LIM_PROTECT_METHOD_MSK    (0x01 << PD_CUR_LIM_PROTECT_METHOD_POS)

#define QC3_0_CUR_LIM_PROTECT_METHOD_POS 0x06
#define QC3_0_CUR_LIM_PROTECT_METHOD_MSK (0x01 << QC3_0_CUR_LIM_PROTECT_METHOD_POS)

#define EN_QC3_0_CONSTANT_POWER_POS      0x05
#define EN_QC3_0_CONSTANT_POWER_MSK      (0x01 << EN_QC3_0_CONSTANT_POWER_POS)

#define EN_PPS_CONSTANT_POWER_POS        0x04
#define EN_PPS_CONSTANT_POWER_MSK        (0x01 << EN_PPS_CONSTANT_POWER_POS)

#define POWER_SETTING_METHOD_POS         0x02
#define POWER_SETTING_METHOD_MSK         (0x01 << POWER_SETTING_METHOD_POS)

#define SET_QUICK_CHARGE_4_REG           0xAC
#define SET_QUICK_CHARGE_4_REG_AC        SET_QUICK_CHARGE_4_REG

#define EN_PORT_QUICK_CHARGE_POS         0x02
#define EN_PORT_QUICK_CHARGE_MSK         (0x01 << EN_PORT_QUICK_CHARGE_POS)

#define EN_PD_5V_2A_POS                  0x00
#define EN_PD_5V_2A_MSK                  (0x01 << EN_PD_5V_2A_POS)

#define SET_VID_0_REG                    0xAE
#define SET_VID_0_REG_AE                 SET_VID_0_REG

#define USB_VID0_POS                     0x00
#define USB_VID0_MSK                     (0x07 << USB_VID0_POS)

#define SET_VID_1_REG                    0xAF
#define SET_VID_1_REG_AF                 SET_VID_1_REG

#define USB_VID1_POS                     0x00
#define USB_VID1_MSK                     (0x01 << USB_VID1_POS)

////////ENABLE I2C POS SEQUENCE
#define EN_I2C_SEQ1                      0x20
#define EN_I2C_SEQ2                      0x40
#define EN_I2C_SEQ3                      0x80


#endif




