/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file config_types.h
 * @brief The config data types
 * @author SD1 & SW team
 ****************************************************************************/
#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Fast charge protocol
 */
typedef enum
{
    FC_None = 0,         //!< 0  none
    FC_QC2,              //!< 1  Qc2
    FC_QC3,              //!< 2  Qc3
    FC_QC3P,             //!< 3  Qc3+
    FC_SFCP,             //!< 4  SFCP
    FC_AFC,              //!< 5  AFC
    FC_FCP,              //!< 6  FCP
    FC_SCP,              //!< 7  SCP
    FC_VOOC1P0,          //!< 8  VOOC1.0
    FC_VOOC4P0,          //!< 9  VOOC4.0
    FC_SVOOC2P0,         //!< 10 SupperVooc2.0
    FC_TFCP,             //!< 11 TFCP
    FC_UFCS,             //!< 12 UFCS
    FC_PE1,              //!< 13 PE1.0
    FC_PE2,              //!< 14 PE2.0
    FC_PD_Fix5V,         //!< 15 PD fix 5v
    FC_PD_FixHV,         //!< 16 PD fix high voltage
    FC_PD_PPS,           //!< 17 PD PPS
    FC_PD_EPR_HV,        //!< 18 PD EPR high voltage
    FC_PD_AVS,           //!< 19 PD AVS
} fast_charge_protocol_e;


/**
 * Typec cc term definition
 * refer to typec control register
 */
typedef enum
{
    RP_RESERVED = 0,
    RP_3A = 1,
    RP_1_5A = 2,
    RP_DEFAULT_USB = 3,
} typec_cc_term_set_e;

/**
 * Line resistance compensave type
 */
typedef enum
{
    CABLE_COMPENSAVE_VALUE_100MOHM = 0,        //!< 100 mohm
    CABLE_COMPENSAVE_VALUE_65MOHM,             //!< 65 mohm
} cable_compensave_value_e;

/**
 * Fix voltage output offset
 */
typedef enum
{
    FIX_VOLTAGE_OFFSET_0MV = 0,          //!< 0 mv
    FIX_VOLTAGE_OFFSET_100MV = 1,        //!< 100 mv
    FIX_VOLTAGE_OFFSET_150MV = 2,        //!< 150 mv
    FIX_VOLTAGE_OFFSET_200MV = 3,        //!< 200 mv
} fix_voltage_offset_e;

#define FIX_VOLTAGE_OFFSET_NUM (FIX_VOLTAGE_OFFSET_200MV + 1)        //!< the number of elements

/**
 * Fix voltage output offset
 */
typedef enum
{
    FIX_CURRENT_OFFSET_300MA = 0,        //!< 300 ma
    FIX_CURRENT_OFFSET_350MA = 1,        //!< 350 ma
    FIX_CURRENT_OFFSET_400MA = 2,        //!< 400 ma
    FIX_CURRENT_OFFSET_450MA = 3,        //!< 450 ma
} fix_current_offset_e;

#define FIX_CURRENT_OFFSET_NUM (FIX_CURRENT_OFFSET_450MA + 1)        //!< the number of elements

/**
 * AFC max voltage level
 */
typedef enum
{
    AFC_MAX_VOL_9V = 0,        //!< max vol 9V
    AFC_MAX_VOL_12V,           //!< max vol 12v
} afc_max_voltage_level_e;

/**
 * FCP max power level
 */
typedef enum
{
    FCP_MAX_POWER_18W = 0,        //!< max power 18w
    FCP_MAX_POWER_36W,            //!< max power 36w
} fcp_max_power_level_e;

/**
 * QC2.0 max voltage level
 */
typedef enum
{
    QC2P0_MAX_VOL_5V = 0,        //!< max voltage 5v
    QC2P0_MAX_VOL_9V,            //!< max voltage 9v
    QC2P0_MAX_VOL_12V,           //!< max voltage 12v
    QC2P0_MAX_VOL_20V,           //!< max voltage 20v
} qc2p0_max_voltage_level_e;

#define QC20_MAX_VOL_LEVEL_NUM (QC2P0_MAX_VOL_20V + 1)
/**
 * QC3.0 max voltage level
 */
typedef enum
{
    QC3P0_MAX_VOL_5V = 0,        //!< max voltage 5v
    QC3P0_MAX_VOL_9V,            //!< max voltage 9v
    QC3P0_MAX_VOL_12V,           //!< max voltage 12v
    QC3P0_MAX_VOL_20V,           //!< max voltage 20v
} qc3p0_max_voltage_level_e;

#define QC3P0_MAX_VOL_LEVEL_NUM (QC3P0_MAX_VOL_20V + 1)        //!< the number of elements
/**
 * QC3.0 max power
 */
typedef enum
{
    QC3_MAX_POWER_18W = 0,        //!< max power 18w
    QC3_MAX_POWER_27W,            //!< max power 27w
    QC3_MAX_POWER_45W,            //!< max power 45w
    QC3_MAX_POWER_60W,            //!< max power 60w
} qc3_max_power_level_e;

/**
 * PE max voltage level
 */
typedef enum
{
    PE_MAX_VOL_5V = 0,        //!< max voltage 5v
    PE_MAX_VOL_9V,            //!< max voltage 9v
    PE_MAX_VOL_12V,           //!< max voltage 12v
    PE_MAX_VOL_20V,           //!< max voltage 20v
} pe_max_voltage_level_e;

#define PE_MAX_VOL_NUM (PE_MAX_VOL_20V + 1)        //!< number of elements

/**
 * QC3.0 plus max power level
 */
typedef enum
{
    QC3PLUS_MAX_POWER_18W = 0,        //!< max power 18w
    QC3PLUS_MAX_POWER_27W,            //!< max power 27w
    QC3PLUS_MAX_POWER_40W,            //!< max power 40w
    QC3PLUS_MAX_POWER_45W,            //!< max power 45w
} qc3plus_max_power_level_e;

#define QC3PLUS_MAX_POWER_NUM (QC3PLUS_MAX_POWER_45W + 1)        //!< number of elements

/**
 * SFCP max voltage level
 */
typedef enum
{
    SFCP_MAX_VOL_9V = 0,        //!< max voltage 9v
    SFCP_MAX_VOL_12V,           //!< max voltage 12v
} sfcp_max_voltage_level_e;

/**
 * FCP max voltage level
 */
typedef enum
{
    FCP_MAX_VOL_9V = 0,        //!< max voltage 9v
    FCP_MAX_VOL_12V,           //!< max voltage 12v
} fcp_max_voltage_level_e;

/**
 * Port noload threshold
 */
typedef enum
{
    NOLOAD_TH_3MA,                  //!< 3mA
    NOLOAD_TH_6MA,                  //!< 6mA
    NOLOAD_TH_9MA,                  //!< 9mA
    NOLOAD_TH_12MA,                 //!< 12mA
    NOLOAD_TH_15MA,                 //!< 15mA
    NOLOAD_TH_18MA,                 //!< 18mA
    NOLOAD_TH_21MA,                 //!< 21mA
    NOLOAD_TH_24MA,                 //!< 24mA
    NOLOAD_TH_27MA,                 //!< 27mA
    NOLOAD_TH_30MA,                 //!< 30mA
    NOLOAD_TH_33MA,                 //!< 33mA
    NOLOAD_TH_36MA,                 //!< 36mA
    NOLOAD_TH_39MA,                 //!< 39mA
    NOLOAD_TH_42MA,                 //!< 42mA
    NOLOAD_TH_45MA,                 //!< 45mA
    NOLOAD_TH_48MA,                 //!< 48mA
} noload_threshold_e;

/** 
 * Port current limit 
 */
typedef enum
{
    PORT_CURR_LIMIT_1P5A = 0,                   //!< 1.5A current limit
    PORT_CURR_LIMIT_2P0A,                       //!< 2.0A current limit
    PORT_CURR_LIMIT_2P4A,                       //!< 2.4A current limit
    PORT_CURR_LIMIT_3P0A                        //!< 3.0A current limit
} port_current_limit_e;

#define PCL_NUM (PORT_CURR_LIMIT_3P0A + 1)

/** 
 * Port working mode 
 */
typedef enum
{
    SINGLE_A_MODE = 0,                        //!< single A mode
    SINGLE_C_MODE,                            //!< single C mode
    DUO_AA_MODE,                              //!< A + A mode
    DUO_CA_MODE,                              //!< C + A mode
    DUO_CC_MODE                               //!< C + C mode
} port_mode_e;


/** 
 * hsmos resistance
 */
typedef enum
{
    MOS_RES_2MOHM,                          //!< 2 mOhm
    MOS_RES_3MOHM,                          //!< 3 mOhm
    MOS_RES_4MOHM,                          //!< 4 mOhm
    MOS_RES_5MOHM,                          //!< 5 mOhm
    MOS_RES_6MOHM,                          //!< 6 mOhm
    MOS_RES_8MOHM,                          //!< 8 mOhm
    MOS_RES_10MOHM,                         //!< 10 mOhm
    MOS_RES_13MOHM,                         //!< 13 mOhm
} mos_res_e;


/** 
 * hsmos resistance
 */
typedef enum
{
    VR_OCP_6A,                          //!< 6 A
    VR_OCP_8A,                          //!< 8 A
    VR_OCP_10A,                         //!< 10 A
    VR_OCP_12A,                         //!< 12 A
    VR_OCP_15A,                         //!< 15 A
    VR_OCP_17A,                         //!< 17 A
    VR_OCP_19A,                         //!< 19 A
    VR_OCP_21A,                         //!< 21 A
} vr_ocp_e;


/**
 * Vd detect threshold
 */
typedef enum
{
    TH_1P075V_0P550V = 0x00,        //!< threshold 0: high side is 1.075V, low side is 0.550V
    TH_1P150V_0P625V = 0x01,        //!< threshold 1: high side is 1.150V, low side is 0.625V
    TH_1P225V_0P700V = 0x02,        //!< threshold 2: high side is 1.225V, low side is 0.700V
    TH_1P300V_0P775V = 0x03,        //!< threshold 3: high side is 1.300V, low side is 0.775V
    TH_1P375V_0P850V = 0x04,        //!< threshold 4: high side is 1.375V, low side is 0.850V
    TH_1P450V_0P925V = 0x05,        //!< threshold 5: high side is 1.450V, low side is 0.925V
    TH_1P525V_1P000V = 0x06,        //!< threshold 6: high side is 1.525V, low side is 1.000V
    TH_1P600V_1P075V = 0x07,        //!< threshold 7: high side is 1.600V, low side is 1.075V
    TH_1P675V_1P150V = 0x08,        //!< threshold 8: high side is 1.675V, low side is 1.150V
    TH_1P750V_1P225V = 0x09,        //!< threshold 9: high side is 1.750V, low side is 1.225V
    TH_1P825V_1P300V = 0x0a,        //!< threshold 10: high side is 1.825V, low side is 1.300V
    TH_1P900V_1P375V = 0x0b,        //!< threshold 11: high side is 1.900V, low side is 1.375V
    TH_1P975V_1P450V = 0x0c,        //!< threshold 12: high side is 1.975V, low side is 1.450V
    TH_2P050V_1P525V = 0x0d,        //!< threshold 13: high side is 2.050V, low side is 1.525V
    TH_2P125V_1P600V = 0x0e,        //!< threshold 14: high side is 2.125V, low side is 1.600V
    TH_2P200V_1P675V = 0x0f,        //!< threshold 15: high side is 2.200V, low side is 1.675V
} vd_compare_threshold_e;

/**
 * Vd detect result
 */
typedef enum
{
    VD_DETECT_110V = 0,        //!< vd detect result is 110V
    VD_DETECT_220V = 1,        //!< vd detect result is 220V
} vd_result_e;


/**
 * buck config type
 */
typedef enum
{
    BUCK_CFG_181KHZ_10UH,       //!< 181KHz 10uH
    BUCK_CFG_333KHZ_4UH7,       //!< 333KHz 4.7uH
    BUCK_CFG_125KHZ_22UH,       //!< 125KHz 22uH
} buck_cfg_type_e;


#ifdef __cplusplus
}
#endif

#endif        // end of CONFIG_TYPES_H
