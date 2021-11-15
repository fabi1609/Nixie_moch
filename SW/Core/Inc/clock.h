/*
 * clock.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Fabian
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"

#define CL_STATE_INIT 		0
#define CL_STATE_READ_START	1
#define CL_STATE_READ_H		2
#define CL_STATE_READ_M		3
#define CL_STATE_READ_S		4

void ESP01_init();
void OWN_LL_EXTI();

#endif /* INC_CLOCK_H_ */
