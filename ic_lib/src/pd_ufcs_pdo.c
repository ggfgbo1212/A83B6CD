#include "pd_ufcs_pdo.h"

/**************************PD POWER VARIABLE**************************************************/
#define  pd_5v    5000 / 50 
#define  pd_9v    9000 / 50 
#define  pd_12v   12000 / 50 
#define  pd_15v   15000 / 50 
#define  pd_20v   20000 / 50 

#define  pps0_min_vol   PPS0_MIN_VOL / 100 
#define  pps0_max_vol   PPS0_MAX_VOL / 100 
#define  pps1_min_vol   PPS1_MIN_VOL / 100 
#define  pps1_max_vol   PPS1_MAX_VOL / 100 


#define  pd_5v_power1_cur    ((POWER1 / 5000)  > 3) ? (3000 / 10) : (POWER1 / 50) 
#define  pd_9v_power1_cur    ((POWER1 / 9000)  > 3) ? (3000 / 10) : (POWER1 / 90) 
#define  pd_12v_power1_cur   ((POWER1 / 12000) > 3) ? (3000 / 10) : (POWER1 / 120) 
#define  pd_15v_power1_cur   ((POWER1 / 15000) > 3) ? (3000 / 10) : (POWER1 / 150) 
#define  pd_20v_power1_cur   ((POWER1 / 20000) > 5) ? (5000 / 10) : (POWER1 / 200) 
#define  pd_pps0_power1_cur  ((POWER1 / (PPS0_MAX_VOL - 1000)) > 5) ? (5000 / 50) : (POWER1 * 20 / (PPS0_MAX_VOL - 1000)) 

#define  pd_5v_power2_cur    ((POWER2 / 5000)  > 3) ? (3000 / 10) : (POWER2 / 50) 
#define  pd_9v_power2_cur    ((POWER2 / 9000)  > 3) ? (3000 / 10) : (POWER2 / 90)
#define  pd_12v_power2_cur   ((POWER2 / 12000) > 3) ? (3000 / 10) : (POWER2 / 120) 
#define  pd_15v_power2_cur   ((POWER2 / 15000) > 3) ? (3000 / 10) : (POWER2 / 150) 
#define  pd_20v_power2_cur   ((POWER2 / 20000) > 5) ? (5000 / 10) : (POWER2 / 200) 
#define  pd_pps0_power2_cur  ((POWER2 / (PPS0_MAX_VOL - 1000)) > 5) ? (5000 / 50) : (POWER2 * 20 / (PPS0_MAX_VOL - 1000)) 


#define  pd_5v_power3_cur    ((POWER3 / 5000)  > 3) ? (3000 / 10) : (POWER3 / 50) 
#define  pd_9v_power3_cur    ((POWER3 / 9000)  > 3) ? (3000 / 10) : (POWER3 / 90) 
#define  pd_12v_power3_cur   ((POWER3 / 12000) > 3) ? (3000 / 10) : (POWER3 / 120) 
#define  pd_15v_power3_cur   ((POWER3 / 15000) > 3) ? (3000 / 10) : (POWER3 / 150) 
#define  pd_20v_power3_cur   ((POWER3 / 20000) > 5) ? (5000 / 10) : (POWER3 / 200) 
#define  pd_pps0_power3_cur  ((POWER3 / (PPS0_MAX_VOL - 1000)) > 5) ? (5000 / 50) : (POWER3 * 20 / (PPS0_MAX_VOL - 1000)) 



#define  pd_5v_power4_cur     ((POWER4 / 5000)  > 3) ? (3000 / 10) : (POWER4 / 50) 
#define  pd_9v_power4_cur     ((POWER4 / 9000)  > 3) ? (3000 / 10) : (POWER4 / 90) 
#define  pd_12v_power4_cur    ((POWER4 / 12000) > 3) ? (3000 / 10) : (POWER4 / 120) 
#define  pd_15v_power4_cur    ((POWER4 / 15000) > 3) ? (3000 / 10) : (POWER4 / 150) 
#define  pd_20v_power4_cur    ((POWER4 / 20000) > 5) ? (5000 / 10) : (POWER4 / 200) 
#define  pd_pps0_power4_cur   ((POWER4 / (PPS0_MAX_VOL - 1000)) > 5) ? (5000 / 50) : (POWER4 * 20 / (PPS0_MAX_VOL - 1000)) 


/**************************PD POWER1 SET**************************************************/

const struct fix_pdo_t pdo_5v_power1 = 
{
 pd_5v_power1_cur,	 //max_curent_10ma
 pd_5v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_9v_power1 = 
{
 pd_9v_power1_cur,	 //max_curent_10ma
 pd_9v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_12v_power1 = 
{
 pd_12v_power1_cur,	 //max_curent_10ma
 pd_12v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_15v_power1 = 
{
 pd_15v_power1_cur,	 //max_curent_10ma
 pd_15v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_20v_power1 = 
{
 pd_20v_power1_cur,	 //max_curent_10ma
 pd_20v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct apdo_spr_t pps0_power1 = 
{
 pd_pps0_power1_cur,	
 0, //reserved	
 pps0_min_vol,		
 0, //reserved
 pps0_max_vol,	
 0, //reserved
 0, //pps power limited
 0, //spr programmable power supply
 APDO_PWR,
};

/**************************PD POWER2 SET**************************************************/

const struct fix_pdo_t pdo_5v_power2 = 
{
 pd_5v_power2_cur,	 //max_curent_10ma
 pd_5v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_9v_power2 = 
{
 pd_9v_power2_cur,	 //max_curent_10ma
 pd_9v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_12v_power2 = 
{
 pd_12v_power2_cur,	 //max_curent_10ma
 pd_12v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_15v_power2 = 
{
 pd_15v_power2_cur,	 //max_curent_10ma
 pd_15v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_20v_power2 = 
{
 pd_20v_power2_cur,	 //max_curent_10ma
 pd_20v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct apdo_spr_t pps0_power2 = 
{
 pd_pps0_power2_cur,	
 0, //reserved	
 pps0_min_vol,		
 0, //reserved
 pps0_max_vol,	
 0, //reserved
 0, //pps power limited
 0, //spr programmable power supply
 APDO_PWR,
};

/**************************PD POWER3 SET**************************************************/

const struct fix_pdo_t pdo_5v_power3 = 
{
 pd_5v_power3_cur,	 //max_curent_10ma
 pd_5v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_9v_power3 = 
{
 pd_9v_power3_cur,	 //max_curent_10ma
 pd_9v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_12v_power3 = 
{
 pd_12v_power3_cur,	 //max_curent_10ma
 pd_12v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_15v_power3 = 
{
 pd_15v_power3_cur,	 //max_curent_10ma
 pd_15v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct fix_pdo_t pdo_20v_power3 = 
{
 pd_20v_power3_cur,	 //max_curent_10ma
 pd_20v,//voltage_50mv 	
 0,//peak_current
 0,//reserved_0
 0,//epr_mode_cable
 0,//unchunk_ex_msg_suport	
 0,//dual_role_data	
 0,//usb_com_capable	
 1,//unconstrained_power	
 0,//usb_suspd_suppor	
 0,//dual_role_pwr	
 FIXED_SUPPLY_PWR,
};

const struct apdo_spr_t pps0_power3 = 
{
 pd_pps0_power3_cur,	
 0, //reserved	
 pps0_min_vol,		
 0, //reserved
 pps0_max_vol,	
 0, //reserved
 0, //pps power limited
 0, //spr programmable power supply
 APDO_PWR,
};

/**************************PD END**************************************************/

/**************************UFCS POWER DEFINE****************************************/
#define UFCS_POWER1_PPS0_MIN_VOL  340   //(3400  / 10)
#define UFCS_POWER1_PPS0_MAX_VOL  550   //(5500  / 10)

#define UFCS_POWER1_PPS1_MIN_VOL  550   //(5500  / 10)
#define UFCS_POWER1_PPS1_MAX_VOL  1100  //(11000 / 10)

#define UFCS_POWER1_PPS2_MIN_VOL  1100  //(11000 / 10)
#define UFCS_POWER1_PPS2_MAX_VOL  2100  //(21000 / 10)

/**************************UFCS POWER VARIABLE**************************************/
#define ufcs_pps0_power1_cur   ((POWER1 / 5000)   > 3) ?  (3000 / 10)  : (POWER1 / 50) 
#define ufcs_pps1_power1_cur   ((POWER1 / 10000)  > 3) ?  (3000 / 10)  : (POWER1 / 100) 
#define ufcs_pps2_power1_cur   ((POWER1 / 20000)  > 5) ?  (5000 / 10)  : (POWER1 / 100) 

#define ufcs_pps0_power2_cur   ((POWER2 / 5000)   > 3) ?  (3000 / 10)  : (POWER2 / 50) 
#define ufcs_pps1_power2_cur   ((POWER2 / 10000)  > 3) ?  (3000 / 10)  : (POWER2 / 100) 
#define ufcs_pps2_power2_cur   ((POWER2 / 20000)  > 5) ?  (5000 / 10)  : (POWER2 / 100) 

#define ufcs_pps0_power3_cur   ((POWER3 / 5000)   > 3) ?  (3000 / 10)  : (POWER3 / 50) 
#define ufcs_pps1_power3_cur   ((POWER3 / 10000)  > 3) ?  (3000 / 10)  : (POWER3 / 100) 
#define ufcs_pps2_power3_cur   ((POWER3 / 20000)  > 5) ?  (5000 / 10)  : (POWER3 / 100) 

#define ufcs_pps0_power4_cur   ((POWER4 / 5000)   > 3) ?  (3000 / 10)  : (POWER4 / 50) 
#define ufcs_pps1_power4_cur   ((POWER4 / 10000)  > 3) ?  (3000 / 10)  : (POWER4 / 100) 
#define ufcs_pps2_power4_cur   ((POWER4 / 20000)  > 5) ?  (5000 / 10)  : (POWER4 / 100) 
/**************************UFCS POWER1 SET**************************************/
const struct ufcs_pdo_t ufcs_pps0_power1 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps0_power1_cur, //max current
 UFCS_POWER1_PPS0_MIN_VOL,
 UFCS_POWER1_PPS0_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 1,	
};

const struct ufcs_pdo_t ufcs_pps1_power1 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps1_power1_cur, //max current
 UFCS_POWER1_PPS1_MIN_VOL,
 UFCS_POWER1_PPS1_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 2,	
};

const struct ufcs_pdo_t ufcs_pps2_power1 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps2_power1_cur, //max current
 UFCS_POWER1_PPS1_MIN_VOL,
 UFCS_POWER1_PPS1_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 3,	
};

/**************************UFCS POWER2 SET**************************************/
const struct ufcs_pdo_t ufcs_pps0_power2 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps0_power2_cur, //max current
 UFCS_POWER1_PPS0_MIN_VOL,
 UFCS_POWER1_PPS0_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 1,	
};

const struct ufcs_pdo_t ufcs_pps1_power2 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps1_power2_cur, //max current
 UFCS_POWER1_PPS1_MIN_VOL,
 UFCS_POWER1_PPS1_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 2,	
};

const struct ufcs_pdo_t ufcs_pps2_power2 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps2_power2_cur, //max current
 UFCS_POWER1_PPS1_MIN_VOL,
 UFCS_POWER1_PPS1_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 3,	
};

/**************************UFCS POWER3 SET**************************************/
const struct ufcs_pdo_t ufcs_pps0_power3 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps0_power3_cur, //max current
 UFCS_POWER1_PPS0_MIN_VOL,
 UFCS_POWER1_PPS0_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 1,	
};

const struct ufcs_pdo_t ufcs_pps1_power3 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps1_power3_cur, //max current
 UFCS_POWER1_PPS1_MIN_VOL,
 UFCS_POWER1_PPS1_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 2,	
};

const struct ufcs_pdo_t ufcs_pps2_power3 = 
{
 0x64, //min current = 1000ma / 10ma
 ufcs_pps2_power3_cur, //max current
 UFCS_POWER1_PPS1_MIN_VOL,
 UFCS_POWER1_PPS1_MAX_VOL,
 0, //min voltage step,0:10mv, 1:20mv
 4,	//current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
 3,	
};
/**************************UFCS PDO**************************************************/


const int *pd110wcap_point[6] = {(int *)&pdo_5v_power1, (int *)&pdo_9v_power1, (int *)&pdo_12v_power1, (int *)&pdo_15v_power1, (int *)&pdo_20v_power1}; 








