/*
 * nixie.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Fabian
 */

#ifndef INC_NIXIE_H_
#define INC_NIXIE_H_

#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"

#define DAC_TARGET 2940 //2940 approx. 160V    4095 -> 223V

void nixie_init();
void boost_op();

#endif /* INC_NIXIE_H_ */
