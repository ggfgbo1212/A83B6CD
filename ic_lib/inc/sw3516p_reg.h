#ifndef __SW3516P_REG_H__
#define __SW3516P_REG_H__

#define QUICK_CHARGE_STA_REG              0x06
#define QUICK_CHARGE_STA_REG_06           QUICK_CHARGE_STA_REG

#define QUICK_CHARGE_PROTOCOL_STA_POS     0x07
#define QUICK_CHARGE_PROTOCOL_STA_MSK     (0x01 << QUICK_CHARGE_PROTOCOL_STA_POS)

#define QUICK_CHARGE_VOL_STA_POS          0x06
#define QUICK_CHARGE_VOL_STA_MSK          (0x01 << QUICK_CHARGE_VOL_STA_POS)

#define PD_VERSION_POS                    0x04
#define PD_VERSION_MSK                    (0x03 << PD_VERSION_POS)

#define QUICK_CHARGE_PROTOCOL_POS         0x00
#define QUICK_CHARGE_PROTOCOL_MSK         (0x0f << QUICK_CHARGE_PROTOCOL_POS)

#define SYS0_STA_REG                      0x07
#define SYS0_STA_REG_07                   SYS0_STA_REG

#define BUCK_STA_POS                      0x02
#define BUCK_STA_MSK                      (0x01 << BUCK_STA_POS)

#define P2_STA_POS                        0x01
#define P2_STA_MSK                        (0x01 << P2_STA_POS)

#define P1_STA_POS                        0x00
#define P1_STA_MSK                        (0x01 << P1_STA_POS)

#define SYS1_STA_REG                      0x08
#define SYS1_STA_REG_08                   SYS1_STA_REG

#define PORT_SINK_STA_POS                 0x04
#define PORT_SINK_STA_MSK                 (0x0f << PORT_SINK_STA_POS)

#define P1_CUR_LIT_VAL_REG                0x0C
#define P1_CUR_LIT_VAL_REG_0C             P1_CUR_LIT_VAL_REG

#define P1_CUR_LIT_VAL_POS                0x00
#define P1_CUR_LIT_VAL_MSK                (0x7f << P1_CUR_LIT_VAL_POS)

#define P2_CUR_LIT_VAL_REG                0x0D
#define P2_CUR_LIT_VAL_REG_0D             P2_CUR_LIT_VAL_REG

#define P2_CUR_LIT_VAL_POS                0x00
#define P2_CUR_LIT_VAL_MSK                (0x7f << P2_CUR_LIT_VAL_POS)

#define EN_I2C_CTRL_REG                   0x12
#define EN_I2C_CTRL_REG_12                EN_I2C_CTRL_REG

#define EN_I2C_WRITE_POS                  0x05
#define EN_I2C_WRITE_MSK                  (0x07 << EN_I2C_WRITE_POS)

#define EN_ADC_VIN_REG                    0x13
#define EN_ADC_VIN_REG_13                 EN_ADC_VIN_REG

#define PROTOCOL_ANSWER_NTC_TEMP_POS      0x06
#define PROTOCOL_ANSWER_NTC_TEMP_MSK      (0x01 << PROTOCOL_ANSWER_NTC_TEMP_POS)

#define EN_VIN_ADC_POS                    0x01
#define EN_VIN_ADC_MSK                    (0x01 << EN_VIN_ADC_POS)

#define EN_PWR_FORCE_REG                  0x15
#define EN_PWR_FORCE_REG_15               EN_PWR_FORCE_REG

#define EN_PWR_REG_FORCE_POS              0x04
#define EN_PWR_REG_FORCE_MSK              (0x0f << EN_PWR_REG_FORCE_POS)

#define PWR_FORCE0_REG                    0x16
#define PWR_FORCE0_REG_16                 PWR_FORCE0_REG

#define FORCE_OPEN_BUCK_POS               0x01
#define FORCE_OPEN_BUCK_MSK               (0x01 << FORCE_OPEN_BUCK_POS)

#define FORCE_CLOSE_BUCK_POS              0x00
#define FORCE_CLOSE_BUCK_MSK              (0x01 << FORCE_CLOSE_BUCK_POS)

#define ADC_VIN_DATA_REG                  0x30
#define ADC_VIN_DATA_REG_30               ADC_VIN_DATA_REG

#define VIN_VOL_H8_POS                    0x00
#define VIN_VOL_H8_MSK                    (0xff << VIN_VOL_H8_POS)

#define ADC_OUT_DATA_REG                  0x31
#define ADC_OUT_DATA_REG_31               ADC_OUT_DATA_REG

#define OUT_VOL_H8_POS                    0x00
#define OUT_VOL_H8_MSK                    (0xff << OUT_VOL_H8_POS)

#define ADC_IOUT1_DATA_REG                0x33
#define ADC_IOUT1_DATA_REG_33             ADC_IOUT1_DATA_REG

#define IOUT1_H8_POS                      0x00
#define IOUT1_H8_MSK                      (0xff << IOUT1_H8_POS)

#define ADC_IOUT2_DATA_REG                0x34
#define ADC_IOUT2_DATA_REG_34             ADC_IOUT2_DATA_REG

#define IOUT2_H8_POS                      0x00
#define IOUT2_H8_MSK                      (0xff << IOUT2_H8_POS)

#define ADC_NTC_VOL_DATA_REG              0x37
#define ADC_NTC_VOL_DATA_REG_37           ADC_NTC_VOL_DATA_REG

#define NTC_R_VOL_H8_POS                  0x00
#define NTC_R_VOL_H8_MSK                  (0xff << NTC_R_VOL_H8_POS)

#define SET_ADC_REG                       0x3A
#define SET_ADC_REG_3A                    SET_ADC_REG

#define ADC_DATA_SEL_POS                  0x00
#define ADC_DATA_SEL_MSK                  (0x07 << ADC_DATA_SEL_POS)

#define ADC_DATA_H8_REG                   0x3B
#define ADC_DATA_H8_REG_3B                ADC_DATA_H8_REG

#define ADC_DATA_L4_REG                   0x3C
#define ADC_DATA_L4_REG_3C                ADC_DATA_L4_REG

#define ADC_DATA_L4_POS                   0x00
#define ADC_DATA_L4_MSK                   (0x0f << ADC_DATA_L4_POS)

#define PD_CMD_REQ_REG                    0x70
#define PD_CMD_REQ_REG_70                 PD_CMD_REQ_REG

#define EN_PD_CMD_TX_POS                  0x07
#define EN_PD_CMD_TX_MSK                  (0x01 << EN_PD_CMD_TX_POS)

#define PD_CMD_POS                        0x00
#define PD_CMD_MSK                        (0x0f << PD_CMD_POS)

#define SET_PD_MSG_REG                    0x71
#define SET_PD_MSG_REG_71                 SET_PD_MSG_REG

#define EN_PD_GET_SOURCE_EXTEND_POS       0x04
#define EN_PD_GET_SOURCE_EXTEND_MSK       (0x01 << EN_PD_GET_SOURCE_EXTEND_POS)

#define EN_PD_GET_STATUS_POS              0x03
#define EN_PD_GET_STATUS_MSK              (0x01 << EN_PD_GET_STATUS_POS)

#define EN_PD_VCONN_SWAP_POS              0x02
#define EN_PD_VCONN_SWAP_MSK              (0x01 << EN_PD_VCONN_SWAP_POS)

#define EN_PD_DR_SWAP_POS                 0x01
#define EN_PD_DR_SWAP_MSK                 (0x01 << EN_PD_DR_SWAP_POS)

#define PD_SRC_CAP_CMD_TX_REG             0x73
#define PD_SRC_CAP_CMD_TX_REG_73          PD_SRC_CAP_CMD_TX_REG

#define PD_SRC_CAP_CMD_TX_POS             0x07
#define PD_SRC_CAP_CMD_TX_MSK             (0x01 << PD_SRC_CAP_CMD_TX_POS)

#define SET_HARDRESET_NUM_REG             0x75
#define SET_HARDRESET_NUM_REG_75          SET_HARDRESET_NUM_REG

#define SET_HARDRESET_NUM_POS             0x00
#define SET_HARDRESET_NUM_MSK             (0x01 << SET_HARDRESET_NUM_POS)

#define SET_LINK_REG                      0x76
#define SET_LINK_REG_76                   SET_LINK_REG

#define SET_CUR_LIT_GREAS_POS             0x02
#define SET_CUR_LIT_GREAS_MSK             (0x01 << SET_CUR_LIT_GREAS_POS)

#define EN_DPDM_QUICK_CHARGE_POS          0x01
#define EN_DPDM_QUICK_CHARGE_MSK          (0x01 << EN_DPDM_QUICK_CHARGE_POS)

#define EN_FORCE_CC_NONE_DR_POS           0x00
#define EN_FORCE_CC_NONE_DR_MSK           (0x01 << EN_FORCE_CC_NONE_DR_POS)

#define SET_VOL_OFFSET_REG                0xA6
#define SET_VOL_OFFSET_REG_A6             SET_VOL_OFFSET_REG

#define FIX_80MV_VOL_OFFSET_POS           0x03
#define FIX_80MV_VOL_OFFSET_MSK           (0x01 << FIX_80MV_VOL_OFFSET_POS)

#define FIX_40MV_VOL_OFFSET_POS           0x02
#define FIX_40MV_VOL_OFFSET_MSK           (0x01 << FIX_40MV_VOL_OFFSET_POS)

#define SET_QUICK_CHARGE_0_REG            0xAA
#define SET_QUICK_CHARGE_0_REG_AA         SET_QUICK_CHARGE_0_REG

#define EN_QC3_0_POS                      0x06
#define EN_QC3_0_MSK                      (0x01 << EN_QC3_0_POS)

#define SET_PORT_MODE_REG                 0xAB
#define SET_PORT_MODE_REG_AB              SET_PORT_MODE_REG

#define EN_LINE_COM_POS                   0x07
#define EN_LINE_COM_MSK                   (0x01 << EN_LINE_COM_POS)

#define SET_CHIP_PORT_POS                 0x02
#define SET_CHIP_PORT_MSK                 (0x03 << SET_CHIP_PORT_POS)

#define SET_QUICK_CHARGE_1_REG            0xAD
#define SET_QUICK_CHARGE_1_REG_AD         SET_QUICK_CHARGE_1_REG

#define EN_SAMSUNG_1_2V_POS               0x02
#define EN_SAMSUNG_1_2V_MSK               (0x01 << EN_SAMSUNG_1_2V_POS)

#define SET_PDVID0_REG                    0xAF
#define SET_PDVID0_REG_AF                 SET_PDVID0_REG

#define SET_PDVID0_POS                    0x00
#define SET_PDVID0_MSK                    (0xff << SET_PDVID0_POS)

#define SET_PD_0_REG                      0xB0
#define SET_PD_0_REG_B0                   SET_PD_0_REG

#define EN_FIX_5V_PDO_CUR_POS             0x07
#define EN_FIX_5V_PDO_CUR_MSK             (0x01 << EN_FIX_5V_PDO_CUR_POS)

#define FIX_5V_PDO_CUR_POS                0x00
#define FIX_5V_PDO_CUR_MSK                (0x7f << FIX_5V_PDO_CUR_POS)

#define SET_PD_1_REG                      0xB1
#define SET_PD_1_REG_B1                   SET_PD_1_REG

#define EN_FIX_9V_PDO_CUR_POS             0x07
#define EN_FIX_9V_PDO_CUR_MSK             (0x01 << EN_FIX_9V_PDO_CUR_POS)

#define FIX_9V_PDO_CUR_POS                0x00
#define FIX_9V_PDO_CUR_MSK                (0x7f << FIX_9V_PDO_CUR_POS)

#define SET_PD_2_REG                      0xB2
#define SET_PD_2_REG_B2                   SET_PD_2_REG

#define EN_FIX_12V_PDO_CUR_POS            0x07
#define EN_FIX_12V_PDO_CUR_MSK            (0x01 << EN_FIX_12V_PDO_CUR_POS)

#define FIX_12V_PDO_CUR_POS               0x00
#define FIX_12V_PDO_CUR_MSK               (0x7f << FIX_12V_PDO_CUR_POS)

#define SET_PD_3_REG                      0xB3
#define SET_PD_3_REG_B3                   SET_PD_3_REG

#define EN_FIX_15V_PDO_CUR_POS            0x07
#define EN_FIX_15V_PDO_CUR_MSK            (0x01 << EN_FIX_15V_PDO_CUR_POS)

#define FIX_15V_PDO_CUR_POS               0x00
#define FIX_15V_PDO_CUR_MSK               (0x7f << FIX_15V_PDO_CUR_POS)

#define SET_PD_4_REG                      0xB4
#define SET_PD_4_REG_B4                   SET_PD_4_REG

#define FIX_20V_PDO_CUR_POS               0x00
#define FIX_20V_PDO_CUR_MSK               (0x7f << FIX_20V_PDO_CUR_POS)

#define SET_PD_5_REG                      0xB5
#define SET_PD_5_REG_B5                   SET_PD_5_REG

#define SET_EN_PPS0_CUR_POS               0x07
#define SET_EN_PPS0_CUR_MSK               (0x01 << SET_EN_PPS0_CUR_POS)

#define PPS_0_CUR_POS                     0x00
#define PPS_0_CUR_MSK                     (0x7f << PPS_0_CUR_POS)

#define SET_PD_6_REG                      0xB6
#define SET_PD_6_REG_B6                   SET_PD_6_REG

#define SET_EN_PPS1_CUR_POS               0x07
#define SET_EN_PPS1_CUR_MSK               (0x01 << SET_EN_PPS1_CUR_POS)

#define PPS_1_CUR_POS                     0x00
#define PPS_1_CUR_MSK                     (0x7f << PPS_1_CUR_POS)

#define SET_PD_7_REG                      0xB7
#define SET_PD_7_REG_B7                   SET_PD_7_REG

#define EN_PPS_1_POS                      0x07
#define EN_PPS_1_MSK                      (0x01 << EN_PPS_1_POS)

#define EN_PPS_0_POS                      0x06
#define EN_PPS_0_MSK                      (0x01 << EN_PPS_0_POS)

#define EN_PD_2_0V_PDO_POS                0x05
#define EN_PD_2_0V_PDO_MSK                (0x01 << EN_PD_2_0V_PDO_POS)

#define EN_PD_1_5V_PDO_POS                0x04
#define EN_PD_1_5V_PDO_MSK                (0x01 << EN_PD_1_5V_PDO_POS)

#define EN_PD_1_2V_PDO_POS                0x03
#define EN_PD_1_2V_PDO_MSK                (0x01 << EN_PD_1_2V_PDO_POS)

#define EN_PD_9V_PDO_POS                  0x02
#define EN_PD_9V_PDO_MSK                  (0x01 << EN_PD_9V_PDO_POS)

#define EN_PD_READ_EMARKER_POS            0x01
#define EN_PD_READ_EMARKER_MSK            (0x01 << EN_PD_READ_EMARKER_POS)

#define EN_PD_3_POS                       0x00
#define EN_PD_3_MSK                       (0x01 << EN_PD3_POS)

#define SET_PD_8_REG                      0xB8
#define SET_PD_8_REG_B8                   SET_PD_8_REG

#define EN_PD_5V_2A_PDO_BRD_POS           0x05
#define EN_PD_5V_2A_PDO_BRD_MSK           (0x01 << EN_PD_5V_2A_PDO_BRD_POS)

#define PD_65W_70W_DET_EMARKER_POS        0x04
#define PD_65W_70W_DET_EMARKER_MSK        (0x01 << PD_65W_70W_DET_EMARKER_POS)

#define AUTO_BAN_PPS_POS                  0x03
#define AUTO_BAN_PPS_MSK                  (0x01 << AUTO_BAN_PPS_POS)

#define EN_PD_DISCOVER_ID_RESPON_POS      0x02
#define EN_PD_DISCOVER_ID_RESPON_MSK      (0x01 << EN_PD_DISCOVER_ID_RESPON_POS)

#define SET_QUICK_CHARGE_2_REG            0xB9
#define SET_QUICK_CHARGE_2_REG_B9         SET_QUICK_CHARGE_2_REG

#define EN_P1_QUICK_CHARGE_POS            0x07
#define EN_P1_QUICK_CHARGE_MSK            (0x01 << EN_P1_QUICK_CHARGE_POS)

#define EN_P2_QUICK_CHARGE_POS            0x06
#define EN_P2_QUICK_CHARGE_MSK            (0x01 << EN_P2_QUICK_CHARGE_POS)

#define EN_PD_POS                         0x05
#define EN_PD_MSK                         (0x01 << EN_PD_POS)

#define EN_QC_POS                         0x04
#define EN_QC_MSK                         (0x01 << EN_QC_POS)

#define EN_FCQ_POS                        0x03
#define EN_FCQ_MSK                        (0x01 << EN_FCQ_POS)

#define EN_SCP_POS                        0x02
#define EN_SCP_MSK                        (0x01 << EN_SCP_POS)

#define EN_PE_POS                         0x00
#define EN_PE_MSK                         (0x01 << EN_PE_POS)

#define SET_QUICK_CHARGE_3_REG            0xBA
#define SET_QUICK_CHARGE_3_REG_BA         SET_QUICK_CHARGE_3_REG

#define EN_AFC_POS                        0x06
#define EN_AFC_MSK                        (0x01 << EN_AFC_POS)

#define MAX_OUT_VOL_POS                   0x02
#define MAX_OUT_VOL_MSK                   (0x03 << MAX_OUT_VOL_POS)

#define SET_QUICK_CHARGE_4_REG            0xBC
#define SET_QUICK_CHARGE_4_REG_BC         SET_QUICK_CHARGE_4_REG

#define EN_C_NONE_LOAD_DET_POS            0x03
#define EN_C_NONE_LOAD_DET_MSK            (0x01 << EN_C_NONE_LOAD_DET_POS)

#define SET_LIMIT_CURRENT_0_REG           0xBD
#define SET_LIMIT_CURRENT_0_REG_BD        SET_LIMIT_CURRENT_0_REG

#define EFFECT_DPDM_SGL_TO_DOUBLE_POS     0x06
#define EFFECT_DPDM_SGL_TO_DOUBLE_MSK     (0x01 << EFFECT_DPDM_SGL_TO_DOUBLE_POS)

#define P1_LITCUR_VAL_2P_EN_POS           0x04
#define P1_LITCUR_VAL_2P_EN_MSK           (0x03 << P1_LITCUR_VAL_2P_EN_POS)

#define SET_PD_9_REG                      0xBE
#define SET_PD_9_REG_BE                   SET_PD_9_REG

#define EN_SET_PPS1_MAX_VOL_POS           0x07
#define EN_SET_PPS1_MAX_VOL_MSK           (0x01 << EN_SET_PPS1_MAX_VOL_POS)

#define EN_LIT_PPS1_POWWE_POS             0x06
#define EN_LIT_PPS1_POWWE_MSK             (0x01 << EN_LIT_PPS1_POWWE_POS)

#define SET_PPS1_MAX_VOL_POS              0x04
#define SET_PPS1_MAX_VOL_MSK              (0x03 << SET_PPS1_MAX_VOL_POS)

#define EN_SET_PPS0_MAX_VOL_POS           0x03
#define EN_SET_PPS0_MAX_VOL_MSK           (0x01 << EN_SET_PPS0_MAX_VOL_POS)

#define EN_SET_PPS0_LIT_P0WER_POS         0x02
#define EN_SET_PPS0_LIT_P0WER_MSK         (0x01 << EN_SET_PPS0_LIT_P0WER_POS)

#define SET_PPS0_MAX_VOL_POS              0x00
#define SET_PPS0_MAX_VOL_MSK              (0x03 << SET_PPS0_MAX_VOL_POS)

#define SET_PDVID1_REG                    0xBF
#define SET_PDVID1_REG_BF                 SET_PDVID1_REG

#define SET_PDVID1_POS                    0x00
#define SET_PDVID1_MSK                    (0xff << SET_PDVID1_POS)

#define SET_LIMIT_CURRENT_1_REG           0xC4
#define SET_LIMIT_CURRENT_1_REG_C4        SET_LIMIT_CURRENT_1_REG

#define EN_P2_LIT_CUR_2P_ONLINE_POS       0x04
#define EN_P2_LIT_CUR_2P_ONLINE_MSK       (0x01 << EN_P2_LIT_CUR_2P_ONLINE_POS)

#define P2_LITCUR_VAL_POS                 0x02
#define P2_LITCUR_VAL_MSK                 (0x03 << P2_LITCUR_VAL_POS)

#define SET_LITCUR_FIX_OFFSET_POS         0x00
#define SET_LITCUR_FIX_OFFSET_MSK         (0x03 << SET_LITCUR_FIX_OFFSET_POS)

#define SET_QUICK_CHARGE_5_REG            0xC5
#define SET_QUICK_CHARGE_5_REG_C5         SET_QUICK_CHARGE_5_REG

#define SET_TYPEC_POWER_LEVEL_POS         0x04
#define SET_TYPEC_POWER_LEVEL_MSK         (0x01 << SET_TYPEC_POWER_LEVEL_POS)

#define EN_SCP_HIGH_VOI_POS               0x02
#define EN_SCP_HIGH_VOI_MSK               (0x01 << EN_SCP_HIGH_VOI_POS)

#define SET_QUICK_CHARGE_6_REG            0xCF
#define SET_QUICK_CHARGE_6_REG_CF         SET_QUICK_CHARGE_6_REG

#define PDO_VOL_LINK_VIN_VOL_POS          0x00
#define PDO_VOL_LINK_VIN_VOL_MSK          (0x01 << PDO_VOL_LINK_VIN_VOL_POS)

///////////QUICK CHARGE PROTOCOL POS
enum quick_charge_protocol
{
QC2_0_PROTOCOL = 1,
QC3_0_PROTOCOL,
FCP_PROTOCOL,
SCP_PROTOCOL,
PD_FIX_PROTOCOL,
PD_PPS_PROTOCOL,
PE1_1_PROTOCOL,
PE2_0_PROTOCOL,
SFCP_PROTOCOL = 10,
AFC_PROTOCOL,
};

////////ENABLE I2C POS SEQUENCE
#define EN_I2C_SEQ1                      0x20
#define EN_I2C_SEQ2                      0x40
#define EN_I2C_SEQ3                      0x80

////////ENABLE PWR_FORCE POS SEQUENCE
#define EN_PWR_FORCE_SEQ1                0x20 
#define EN_PWR_FORCE_SEQ2                0x40 
#define EN_PWR_FORCE_SEQ3                0x80 

///////CHIP PORT CONFIG POS SEQUENCE
enum set_chip_port
{
 SINGLE_A,
 DOUBLE_A,
 SINGLE_C,
 BOTH_A_C,
};

////////SYS1 PORT SINK STA POS (MODE+PORT STATE)
enum port_sink_state
{
 AA_NONE_A1_A2 = 1,
 AA_ONLY_A1,
 AA_ONLY_A2,
 AA_BOTH_A1_A2,
 AC_NONE_A_C,
 AC_ONLY_C,
 AA_ONLY_A,
 AA_BOTH_A_C,
 A_A_OPEN,
 C_C_CLODE,
 C_C_OPEN,
};

////////PORT1 LIMIT CURRENT VALUE
#define P1_LIM_CUR_2_6                 0
#define P1_LIM_CUR_2_2                 1
#define P1_LIM_CUR_1_7                 2
#define P1_LIM_CUR_3_2                 3

////////PORT2 LIMIT CURRENT VALUE
#define P2_LIM_CUR_2_6                 0
#define P2_LIM_CUR_2_2                 1
#define P2_LIM_CUR_1_7                 2
#define P2_LIM_CUR_3_2                 3

////////SET PPS1 MAX VOL
#define PPS1_MAX_VOL_5_9               0
#define PPS1_MAX_VOL_11                1
#define PPS1_MAX_VOL_16                2
#define PPS1_MAX_VOL_21                3 

////////SET PPS0 MAX VOL
#define PPS0_MAX_VOL_5_9               0
#define PPS0_MAX_VOL_11                1
#define PPS0_MAX_VOL_16                2
#define PPS0_MAX_VOL_21                3 

////////SET LIMIT CURRENT FIX OFFSET
#define FIX_OFFSET_LIMCUR_150          0
#define FIX_OFFSET_LIMCUR_450          1
#define FIX_OFFSET_LIMCUR_600          2
#define FIX_OFFSET_LIMCUR_300          3



#endif


