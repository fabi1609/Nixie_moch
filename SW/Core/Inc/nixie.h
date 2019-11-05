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

#define DAC_TARGET 500 //TODO Anpassen

void nixie_init();
void boost_op();

#endif /* INC_NIXIE_H_ */
