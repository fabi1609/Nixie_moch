/*
 * nixie_display.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Fabian
 */

#ifndef INC_NIXIE_DISPLAY_H_
#define INC_NIXIE_DISPLAY_H_

#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"


void nixie_display(uint8_t hours, uint8_t minutes, uint8_t seconds);
static void nixie_display_reset_all();
static void nixie_display_set_anode(uint8_t n);

#endif /* INC_NIXIE_DISPLAY_H_ */
