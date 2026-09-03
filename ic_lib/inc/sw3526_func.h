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

#ifndef __SW3526_FUNC_H__
#define __SW3526_FUNC_H__


#include "sw3526_reg.h"
#include "ic_common.h"



#define OPEN_PD_GEAR(tmpValue, gear)   (tmpValue &= ~(1 << gear))
#define CLOSE_PD_GEAR(tmpValue, gear)  (tmpValue |=  (1 << gear))

#endif	
