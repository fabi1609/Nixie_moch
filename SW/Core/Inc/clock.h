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

void ESP01_init();
void OWN_LL_EXTI(uint8_t ulGpioState);

#endif /* INC_CLOCK_H_ */
