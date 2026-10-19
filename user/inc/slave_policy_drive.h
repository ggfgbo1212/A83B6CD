#include "system.h"

void ATM_Init_Normal(void);
u32 My_GetSystemTimePass(u32 tm);
u32 My_GetSystemTimeMark(void);
u32 My_DieDleay(u32 time);
void ATM_Init_Disable(void);
void ADC_Config_Init(void);
void GPIO_Conifg_Init(void);
void I2C_Drive_Init(void);
u8 Read_Fifo(u8 *tmpAddr);
u8 Write_Fifo(u8 tmpAddr);
void Sys_Sleep_Policy(void);