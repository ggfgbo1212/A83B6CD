#ifndef __PD_UFCS_PDO_H
#define __PD_UFCS_PDO_H

#include <stdint.h>

enum power_data_object
{
 FIXED_SUPPLY_PWR,
 BATTERY_PWR,
 VARIABLE_SUPPLY_PWR,
 APDO_PWR,	
};

////固定电压的PDO数据格式，电源电容由bit31-30来决定,固定电压为00
struct fix_pdo
{
		//bit9-0   最大电流
	uint32_t max_curent_10ma : 10;
		//bit19-10 固定电压数据
	uint32_t voltage_50mv : 10;
		//bit21-20 it shows tha ablity overload
	//00:default 
	//01:150%_in_1ms-125%_in_2ms-110%_in_10ms 
	//10:200%_in_1ms-150%_in_2ms-125%_in_10ms 
	//11:200%_in_1ms-175%_in_2ms-150%_in_10ms
	uint32_t peak_current : 2;
		//bit22 reserved but shall be 0
	uint32_t reserved_0 : 1;
		//bit23  support epr mode 
	uint32_t epr_mode_cable : 1;
		//bit24   support the unchunk extend msg
	uint32_t unchunk_ex_msg_suport : 1;
		//bit25   dual role data
	uint32_t dual_role_data : 1;
		//bit26   have the ablity to usb comm
	uint32_t usb_com_capable : 1;
		//bit27   unconstrained the power
	uint32_t unconstrained_power : 1;
		//bit28   support suspend the usb  
	uint32_t usb_suspd_suppor : 1;
		//bit29   dual role cap power
	uint32_t dual_role_pwr : 1;
	//bit 31-30  show power cap type,this one is 00
	uint32_t fix_supply : 2;
};  

#define fix_pdo_t fix_pdo

//可编程供电模式，步进电压递增或者递减，更灵活
struct apdo_spr
{
 //bit6-0  max currnet,unit is ma,因为要乘50，所以定义为short
	uint32_t max_current_50ma : 7;	
 //bit7 reserved :set 0
	uint32_t reserved3 : 1;	
 //bit15-8 min voltage,unit is 100mv
	uint32_t min_voltage_100mv : 8;	
 //bit16 reserved :set 0
	uint32_t reserved2 : 1;	
 //bit24-17 max voltage,unit is 100mv
	uint32_t max_voltage_100mv : 8;	
 //bit26-25 reserved :set 0
	uint32_t reserved1 : 2;	
 //bit27 the limited of pps power
	uint32_t pps_limited : 1;
 //bit29-28 the charge mode, this must be 00b mode
	uint32_t pps_reserved : 2;
 //bit31-30 power type,this one is 0b11	
	uint32_t apdo_sup : 2;
};

#define apdo_spr_t apdo_spr

//可编程供电模式 ufcs
struct ufcs_pdo
{
 //bit7-0  min currnet,unit is ma,10ma
	uint64_t min_current_10ma : 8;	
	//bit23-8  max currnet,unit is ma,10ma
	uint64_t max_current_10ma : 16;	
	//bit39-24 min voltage,unit is mv,10mv
	uint64_t min_voltage_10mv : 16;	
	//bit55-40 max voltage,unit is mv,10mv
	uint64_t max_voltage_10mv : 16;		
	//bit56 min voltage step,0:10mv, 1:20mv
	uint64_t min_voltage_step : 1;	
	//bit59-57 current step,0:10ma, 1:20ma,2:30ma, 3:40ma, 4:50ma
	uint64_t min_current_step : 3;
	//bit63-60 out_mode_number
	uint64_t out_mode_number :  4;
	
};

#define ufcs_pdo_t ufcs_pdo


#define POWER1  20000
#define POWER2  25000
#define POWER3  30000
#define POWER4  40000
#define POWER5  60000
#define POWER6  100000


#define PPS0_MIN_VOL 3300
#define PPS0_MAX_VOL 21000

#define PPS1_MIN_VOL 3300
#define PPS1_MAX_VOL 11000


extern const struct fix_pdo_t pdo_5v_power1;
extern const struct fix_pdo_t pdo_9v_power1;
extern const struct fix_pdo_t pdo_12v_power1;
extern const struct fix_pdo_t pdo_15v_power1;
extern const struct fix_pdo_t pdo_20v_power1;

extern const struct fix_pdo_t pdo_5v_power2;
extern const struct fix_pdo_t pdo_9v_power2;
extern const struct fix_pdo_t pdo_12v_power2;
extern const struct fix_pdo_t pdo_15v_power2;
extern const struct fix_pdo_t pdo_20v_power2;

extern const struct fix_pdo_t pdo_5v_power3;
extern const struct fix_pdo_t pdo_9v_power3;
extern const struct fix_pdo_t pdo_12v_power3;
extern const struct fix_pdo_t pdo_15v_power3;
extern const struct fix_pdo_t pdo_20v_power3;

extern const struct fix_pdo_t pdo_5v_power4;
extern const struct fix_pdo_t pdo_9v_power4;
extern const struct fix_pdo_t pdo_12v_power4;
extern const struct fix_pdo_t pdo_15v_power4;
extern const struct fix_pdo_t pdo_20v_power4;

extern const struct fix_pdo_t pdo_5v_power5;
extern const struct fix_pdo_t pdo_9v_power5;
extern const struct fix_pdo_t pdo_12v_power5;
extern const struct fix_pdo_t pdo_15v_power5;
extern const struct fix_pdo_t pdo_20v_power5;


#endif


