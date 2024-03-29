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

void nixie_init();
void boost_op();
void nixie_set_time(uint8_t h, uint8_t m, uint8_t s);

#endif /* INC_NIXIE_H_ */
