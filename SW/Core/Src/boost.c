/*
 * boost.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Fabian
 */

#include "boost.h"
#include <stdbool.h>
#include "comp.h"
#include "tim.h"
#include "dac.h"

volatile bool poll_comp = true;

void boost_op()
{
	//TODO DAC startup
	if(poll_comp)
	{
		if(HAL_COMP_GetOutputLevel(&hcomp2)==COMP_OUTPUT_LEVEL_LOW)
		{
			HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
			poll_comp=false;
		}
	}
}

//TODO Timer Interrupt, Timer anhalten und poll_comp true setzen
