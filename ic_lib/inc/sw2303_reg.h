#ifndef __SW2303_REG_H__
#define __SW2303_REG_H__

#define VERSION_R_REG                     0x01
#define VERSION_R_REG_01                  VERSION_R_REG

#define DAC_DATA_H8_R_REG                 0x03  
#define DAC_DATA_H8_R_REG_03              DAC_DATA_H8_R_REG

#define DAC_DATA_L4_R_REG                 0x04 
#define DAC_DATA_L4_R_REG_04              DAC_DATA_L4_R_REG

#define CUR_LIMMIT_R_REG                  0x05
#define CUR_LIMMIT_R_REG_05               CUR_LIMMIT_R_REG


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

#define SYS_STATE_3_REG                   0x0D
#define SYS_STATE_3_REG_0D                SYS_STATE_3_REG

#define ONLINE_STATE_POS                  0x07
#define ONLINE_STATE_MSK         		 (0x01 << ONLINE_STATE_POS)

#define EN_I2C_CTRL_REG                   0x12
#define EN_I2C_CTRL_REG_12                EN_I2C_CTRL_REG

#define CONNECT_CTRL_REG                  0x14
#define CONNECT_CTRL_REG_14               CONNECT_CTRL_REG

#define LINE_COMP_POS                     0x02
#define LINE_COMP_MSK                     (0x01 << LINE_COMP_POS)

#define EN_PWR_FORCE_REG                  0x15
#define EN_PWR_FORCE_REG_15               EN_PWR_FORCE_REG

/*********************************************/
#define FORCE_CTRL_REG                    0x16
#define FORCE_CTRL_REG_16                 FORCE_CTRL_REG

#define FORCE_OPEN_GATE_POS               0x03
#define FORCE_OPEN_GATE_MSK         	 (0x01 << FORCE_OPEN_GATE_POS)

#define FORCE_CLOSE_GATE_POS              0x02
#define FORCE_CLOSE_GATE_MSK         	 (0x01 << FORCE_CLOSE_GATE_POS)

#define FORCE_DAC_POS                     0x01
#define FORCE_DAC_MSK         	         (0x01 << FORCE_DAC_POS )

#define FORCE_CURRENT_LIMMIT_POS          0x00
#define FORCE_CURRENT_LIMMIT_MSK         (0x01 << FORCE_CURRENT_LIMMIT_POS)


#define SET_ADC_REG                       0x3B
#define SET_ADC_REG_3B                    SET_ADC_REG

#define ADC_DATA_SEL_POS                  0x00
#define ADC_DATA_SEL_MSK                  (0x07 << ADC_DATA_SEL_POS)

#define ADC_DATA_H8_REG                   0x3C
#define ADC_DATA_H8_REG_3C                ADC_DATA_H8_REG

#define ADC_DATA_L4_REG                   0x3D
#define ADC_DATA_L4_REG_3D                ADC_DATA_L4_REG

/*********************************************/
#define ONLIE_CONFIG_REG                  0xAC
#define ONLIE_CONFIG_REG_AC               ONLIE_CONFIG_REG

#define ONLINE_DEBOUNCE_POS               0x02
#define ONLINE_DEBOUNCE_MSK               (0x01 << ONLINE_DEBOUNCE_POS)

#define ONLINE_CURRENT_POS                0x00
#define ONLINE_CURRENT_MSK                (0x03 << ONLINE_CURRENT_POS)

/*********************************************/
#define QUICK_CHARGE_CONFIG_1             0xAE
#define QUICK_CHARGE_CONFIG_1_AE          QUICK_CHARGE_CONFIG_1

//0：最低3V, 1：最低5V
#define DAC_LOW_VOL_POS                   0x05
#define DAC_LOW_VOL_MSK                   (0x01 << DAC_LOW_VOL_POS)

//0：协议支持5V以下, 1：不支持
#define PROTOCOL_LOW_VOL_POS              0x04
#define PROTOCOL_LOW_VOL_MSK              (0x01 << PROTOCOL_LOW_VOL_POS)

#define QC3_LOW_20_POS                    0x03
#define QC3_LOW_20_MSK                    (0x01 << QC3_LOW_20_POS)

#define QC2_LOW_20_POS                    0x02
#define QC2_LOW_20_MSK                    (0x01 << QC2_LOW_20_POS)

#define PE_LOW_20_POS                     0x01
#define PE_LOW_20_MSK                     (0x01 << PE_LOW_20_POS )

#define NOT_PD_LOW_12_POS                 0x00
#define NOT_PD_LOW_12_MSK                 (0x01 << NOT_PD_LOW_12_POS)

/*********************************************/
#define POWER_CONFIG                      0xAF
#define POWER_CONFIG_AF                   POWER_CONFIG


#define QUICK_CHARGE_CONFIG_2             0xB0
#define QUICK_CHARGE_CONFIG_2_B0          QUICK_CHARGE_CONFIG_2


#define EN_H_SCP_POS                   		0x05
#define EN_H_SCP_MSK                   		(0x01 << EN_H_SCP_POS)

#define EN_L_SCP_POS                   		0x04
#define EN_L_SCP_MSK                   		(0x01 << EN_L_SCP_POS)

#define EN_QC3_POS                   		  0x03
#define EN_QC3_MSK                   		  (0x01 << EN_QC3_POS)

#define EN_QC2_POS                   		  0x02
#define EN_QC2_MSK                   		  (0x01 << EN_QC2_POS)

#define EN_QC_ALL_POS                   	0x01
#define EN_QC_ALL_MSK                   	(0x01 << EN_QC_ALL_POS)

//BC1.2如果关闭，则不再驱动DPDM，DPDM保持短接
#define EN_BC12_POS                       0x00
#define EN_BC12_MSK                   	  (0x01 << EN_BC12_POS)





#define QUICK_CHARGE_CONFIG_3             0xB1
#define QUICK_CHARGE_CONFIG_3_B1          QUICK_CHARGE_CONFIG_3

//高于DAC的20%
#define EN_VIN_OVP1_POS                   0x07
#define EN_VIN_OVP1_MSK                   (0x01 << EN_VIN_OVP_POS)
//高于DAC设定的2V
#define EN_VIN_OVP2_POS                   0x06
#define EN_VIN_OVP2_MSK                   (0x01 << EN_VIN_OVP_POS)

#define EN_SUNG12_POS                     0x05
#define EN_SUNG12_MSK                   	(0x01 << EN_SUNG12_POS)

#define EN_SFCP_POS                   		0x04
#define EN_SFCP_MSK                   		(0x01 << EN_SFCP_POS)

#define EN_FCP_POS                   		  0x03
#define EN_FCP_MSK                   		  (0x01 << EN_FCP_POS)

#define EN_AFC_POS                   		  0x02
#define EN_AFC_MSK                   		  (0x01 << EN_AFC_POS)

#define EN_PE_POS                   		  0x01
#define EN_PE_MSK                   		  (0x01 << EN_PE_POS)



#define PD_CONFIG2_RW_REG                 0xB5
#define PD_CONFIG2_RW_REG_B5              PD_CONFIG2_RW_REG

#define EN_FIX_9V_PDO_POS                 0
#define EN_FIX_9V_PDO_MSK                 (0x01 << EN_FIX_9V_PDO_POS)

#define EN_FIX_12V_PDO_POS                1
#define EN_FIX_12V_PDO_MSK               (0x01 << EN_FIX_12V_PDO_POS)

#define EN_FIX_15V_PDO_POS                2
#define EN_FIX_15V_PDO_MSK               (0x01 << EN_FIX_15V_PDO_POS)
 
#define EN_FIX_20V_PDO_POS                3
#define EN_FIX_20V_PDO_MSK               (0x01 << EN_FIX_20V_PDO_POS)

#define EN_PPS0_PDO_POS                   4
#define EN_PPS0_PDO_MSK                   (0x01 << EN_PPS0_PDO_POS)

#define EN_PPS1_PDO_POS                   5
#define EN_PPS1_PDO_MSK                   (0x01 << EN_PPS1_PDO_POS)

#define EN_PPS2_PDO_POS                   6
#define EN_PPS2_PDO_MSK                   (0x01 << EN_PPS2_PDO_POS)

#define EN_PPS3_PDO_POS                   7
#define EN_PPS3_PDO_MSK                   (0x01 << EN_PPS3_PDO_POS)
   
#define PD_CONFIG_0                       0xB3
#define PD_CONFIG_0_B3                    PD_CONFIG_0

#define EN_PD_READ_EMARKER_POS            0x04
#define EN_PD_READ_EMARKER_MSK            (0x01 << EN_PD_READ_EMARKER_POS)
	 
#define SET_VID_REG_H                     0xB6
#define SET_VID_REG_H_B6                  SET_VID_REG_H

#define SET_VID_REG_L                     0xB7
#define SET_VID_REG_L_B7                  SET_VID_REG_L

#define SET_PID_REG_H                     0xBC
#define SET_PID_REG_H_BC                  SET_PID_REG_H

#define SET_PID_REG_L                     0xBD
#define SET_PID_REG_L_BD                  SET_PID_REG_L

#endif

