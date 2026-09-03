/*
***************************************************************************
* Copyright (C) 2020-2020 Ismartware Limited. All rights reserved. 
* File name: user_define.h
* Description: api for user to define 
* Author: ZR_ShenZhenSoftTeam
* Version: V1.0
* Date:15/02/23 
* History: none
***************************************************************************
*/

#ifndef USER_DEFINE_H
#define USER_DEFINE_H

#include "system.h"

#define PORT1 0
#define PORT2 1
#define PORT3 2
#define PORT4 3
#define PORT5 4

#define PORT_NUM1 0
#define PORT_NUM2 1

#define DIRECT_GET 			0
#define READ_CURRENT_GET 	1

/***********************以下用户按顺序进行配置********************/

/*************** 1、端口1软件I2C的GPIO配置 *********/
#define SCL0	GPIO1
#define SDA0	GPIO0

#define SCL1 	GPIO1
#define SDA1 	GPIO2

#define SCL2 	0
#define SDA2 	0 

#define SCL3 	0
#define SDA3 	0

#define SCL4 	0
#define SDA4	0

//#define SCL5 GPIO5
//#define SDA5 GPIO4 
#define SLAVE_GPIO_GROUP GROUP0 //定义SLAVE设备，使用哪一组GPIO脚
#define SLAVE_READ_LEN    3

/*************** 3、以下是i2c模拟1、模拟2、硬件I2C接口，使用哪一组，把值给对应的接口。
                    可用值为以下函数，名字不可变。
	u8 soft_iic_wrtie_1;
	u8 soft_iic_read_1;

	u8  soft_iic_wrtie_2;
	u8 soft_iic_read_2;

	u8  soft_iic_wrtie_3;
	u8 soft_iic_read_3;

例：
	#define PORT1_I2C_READ    I2c_Read_Gruop0
	#define PORT1_I2C_WRITE   I2c_Write_Gruop0

    #define PORT2_I2C_READ    I2c_Read_Gruop1
    #define PORT2_I2C_WRITE   I2c_Write_Gruop1
		
    #define PORT3_I2C_READ    I2c_Read_Gruop2
    #define PORT3_I2C_WRITE   I2c_Write_Gruop2

如果是A+C端口，两个PORT共用一个端口，则定义成相同的i2c程序接口
    #define PORT4_I2C_READ    I2c_Read_Gruop2
    #define PORT4_I2C_WRITE   I2c_Write_Gruop2		
		
*******************************************************************************/

#define PORT1_I2C_READ    0
#define PORT1_I2C_WRITE   0

#define PORT2_I2C_READ    I2c_Read_Gruop1
#define PORT2_I2C_WRITE   I2c_Write_Gruop1

#define PORT3_I2C_READ    I2c_Read_Gruop0
#define PORT3_I2C_WRITE   I2c_Write_Gruop0

#define PORT4_I2C_READ    I2c_Read_Gruop0
#define PORT4_I2C_WRITE   I2c_Write_Gruop0

#define PORT5_I2C_READ    0  //此处5口为芯片4的A口，所以共用I2C
#define PORT5_I2C_WRITE   0

/*************** 4、配置对应接口的I2C地址,此地址只针对除主机外的设备。如果从机配置了I2C多从机的方式，此地址不再起作用 *********/
//非3566设备（3516/3526/3536等非MCU产品）地址一定要把实际的地址，一般设为0x3C，否则会出现读写问题。

#define PORT1_IC_ADDR     0x37
#define PORT2_IC_ADDR     0x38
#define PORT3_IC_ADDR     0x39
#define PORT4_IC_ADDR     0x3A
#define PORT5_IC_ADDR     0x3B

/**************5、*/


/*************** 5、用户定义端口芯片组
注意此时的端口顺序，1口通常设为3566，如果主机位置
不在PORT1,请在功率分配的时候做下调节即可。
										*********/
#define PORT1_IC SW_3566
#define PORT2_IC SW_NET_DEV 
#define PORT3_IC SW_3516P 
#define PORT4_IC SW_3516P
#define PORT5_IC 0
/*************** 6、宏定义使用对应的库 *********/
#define _USE_SW3566_LIB
#define _USE_NET_LIB
#define _USE_SW3516P_LIB

/*************** 7、宏定义端口所在的序号 PORT_NUM1为芯片第一个口，PORT_NUM2为芯片的第2个口*********/

#define PORT1_NUM PORT_NUM1
#define PORT2_NUM PORT_NUM1
#define PORT3_NUM PORT_NUM1
#define PORT4_NUM PORT_NUM2
#define PORT5_NUM 0

/*************** 8、用到的接口总数，当前为5，不要修改*********/
#define MODULE_NUM 5 
/*************** 9、用到温度传感器的地址请设置温度档位阈值*********/
//使用场景，高于100度会执行一个功率档，低于70，会执行一个温度档，在70-100间档位保持不变

//第一个温度档
#define TMPER_GEAR_1	75
//第二个温度档
#define TMPER_GEAR_2	85
//第三个温度档
#define TMPER_GEAR_3	95

/*************** 10、如果用到联网设备，需要设置网络地址与实际存储地址的偏移量，假设网络设备是连续排列。*********/

/*************** 11、总的端口数量，C口和A口的数量和*********/
#define PORT_SUM	4  //用户定义端口数量


/*************** 11、设置端口读取方式*********/

////通过I2C连接其它3566设备，请务必设置获取模式为直接获取!!!!!!!!


#define GET_PORT1_STATE_MODE	DIRECT_GET

#if GET_PORT1_STATE_MODE

	#define  P1_UNLOAD_CURRENT_MA	50//空载电流
	#define  P1_UNLOAD_DEBOUNCE_MS 	2000//小于空载电流多久认为拔出
	
	#define  P1_LOAD_CURRENT_MA     250//带载电流	
	#define  P1_LOAD_DEBUOUNCE_MS	1000//大于带载电流多久认为插入
#else

	#define  P1_UNLOAD_DEBOUNCE_MS 	0//设备拔出多久认为拔出
	#define  P1_LOAD_DEBUOUNCE_MS	0//设备在线多久认为在线

#endif


#define GET_PORT2_STATE_MODE	DIRECT_GET

#if GET_PORT2_STATE_MODE

	#define  P2_UNLOAD_CURRENT_MA	50//空载电流
	#define  P2_UNLOAD_DEBOUNCE_MS 	2000//小于空载电流多久认为拔出
	
	#define  P2_LOAD_CURRENT_MA     250//带载电流	
	#define  P2_LOAD_DEBUOUNCE_MS	1000//大于带载电流多久认为插入
	
#else

	#define  P2_UNLOAD_DEBOUNCE_MS 	0//设备拔出多久认为拔出
	#define  P2_LOAD_DEBUOUNCE_MS	0//设备在线多久认为在线
	
#endif


#define GET_PORT3_STATE_MODE	DIRECT_GET

#if GET_PORT3_STATE_MODE

	#define  P3_UNLOAD_CURRENT_MA	50//空载电流
	#define  P3_UNLOAD_DEBOUNCE_MS 	2000//小于空载电流多久认为拔出
	
	#define  P3_LOAD_CURRENT_MA     250//带载电流	
	#define  P3_LOAD_DEBUOUNCE_MS	1000//大于带载电流多久认为插入

#else

	#define  P3_UNLOAD_DEBOUNCE_MS 	0//设备拔出多久认为拔出
	#define  P3_LOAD_DEBUOUNCE_MS	0//设备在线多久认为在线
	
#endif

#define GET_PORT4_STATE_MODE	DIRECT_GET

#if GET_PORT4_STATE_MODE

	#define  P4_UNLOAD_CURRENT_MA	50//空载电流
	#define  P4_UNLOAD_DEBOUNCE_MS 	2000//小于空载电流多久认为拔出
	
	#define  P4_LOAD_CURRENT_MA     250//带载电流	
	#define  P4_LOAD_DEBUOUNCE_MS	1000//大于带载电流多久认为插入
	
#else

	#define  P4_UNLOAD_DEBOUNCE_MS 	0//设备拔出多久认为拔出
	#define  P4_LOAD_DEBUOUNCE_MS	0//设备在线多久认为在线
		
	
#endif

#endif

