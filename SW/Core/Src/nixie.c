/*
 * nixie.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Fabian
 */

#include "nixie.h"
#include <stdbool.h>
#include "comp.h"
#include "tim.h"
#include "dac.h"
#include "rtc.h"
#include "gpio.h"

volatile uint32_t dac_value = 0;
volatile uint8_t digit=0;
volatile uint8_t hours=0;
volatile uint8_t minutes=0;
volatile uint8_t seconds=0;
volatile bool poll_comp = true;

static void nixie_display();
static void nixie_display_reset_all();
static void nixie_display_set_anode(uint8_t n);

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim2.Instance)
	{
		//End of boost converter pulse. Set flag to poll the feedback comparator of boost converter again
		poll_comp = true;
	}
	else if(htim->Instance == htim14.Instance)
	{
		//perform multiplex for nixie tubes
		nixie_display();

		//ramp up feedback voltage for boost converter
		if(dac_value < DAC_TARGET)
		{
			dac_value = dac_value + 10;
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
		}
	}
}

void nixie_init()
{
	//Start DAC for feedback voltage of boost converter
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
	//Start comparator for boost converter
	HAL_COMP_Start(&hcomp2);
	//Enable one pulse mode for boost converter. Trigger by software trough HAL_TIM_Base_Start
	HAL_TIM_OnePulse_Start(&htim2, TIM_CHANNEL_1);
	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
	//Start timer and period elapsed event of multiplex timer for nixie tubes
	HAL_TIM_Base_Start_IT(&htim14);
}

void boost_op()
{
	if(poll_comp)
	{
		//If voltage lower than feedback, activate next pulse of boost converter and switch off polling the comparator
		if(HAL_COMP_GetOutputLevel(&hcomp2) == COMP_OUTPUT_LEVEL_LOW)
		{
			poll_comp = false;
			__HAL_TIM_ENABLE(&htim2);
		}
	}
}

void nixie_set_time(uint8_t h, uint8_t m, uint8_t s)
{
	hours = h;
	minutes = m;
	seconds = s;
}

static void nixie_display()
{
	nixie_display_reset_all();
	switch(digit)
	{
	case 0:
		nixie_display_set_anode(hours/10);
		HAL_GPIO_WritePin(GN1_GPIO_Port, GN1_Pin, GPIO_PIN_SET);
		digit = 1;
		break;
	case 1:
		nixie_display_set_anode(hours%10);
		HAL_GPIO_WritePin(GN2_GPIO_Port, GN2_Pin, GPIO_PIN_SET);
		digit = 2;
		break;
	case 2:
		nixie_display_set_anode(minutes/10);
		HAL_GPIO_WritePin(GN3_GPIO_Port, GN3_Pin, GPIO_PIN_SET);
		digit = 3;
		break;
	case 3:
		nixie_display_set_anode(minutes%10);
		HAL_GPIO_WritePin(GN4_GPIO_Port, GN4_Pin, GPIO_PIN_SET);
		digit = 0;
		break;
	}

	/*TODO: PA4 and PA5 can’t be used in output mode when DAC1_OUT1 or
	DAC1_OUT2 respectively is connected to on-chip peripherals*/
	if(seconds%2)
	{
		//HAL_GPIO_WritePin(GDOT_GPIO_Port, GDOT_Pin, GPIO_PIN_SET);
		//Workaround: use LL library and use pullup oder -down resistor instaed of output
		LL_GPIO_SetPinPull(GDOT_GPIO_Port, GDOT_Pin, GPIO_PULLUP);
	}
	else
	{
		//HAL_GPIO_WritePin(GDOT_GPIO_Port, GDOT_Pin, GPIO_PIN_RESET);
		//Workaround: use LL library and use pullup oder -down resistor instaed of output
		LL_GPIO_SetPinPull(GDOT_GPIO_Port, GDOT_Pin, GPIO_PULLDOWN);
	}
}

static void nixie_display_reset_all()
{
	HAL_GPIO_WritePin(GPIOA, GN1_Pin|GN2_Pin|G1_Pin|G3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GN3_Pin|G2_Pin|G4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, G5_Pin|G6_Pin|GN4_Pin|G0_Pin|G9_Pin|G8_Pin|G7_Pin, GPIO_PIN_RESET);
}

static void nixie_display_set_anode(uint8_t n)
{
	switch(n)
	{
	case 0:
		HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(G3_GPIO_Port, G3_Pin, GPIO_PIN_SET);
		break;
	case 4:
		HAL_GPIO_WritePin(G4_GPIO_Port, G4_Pin, GPIO_PIN_SET);
		break;
	case 5:
		HAL_GPIO_WritePin(G5_GPIO_Port, G5_Pin, GPIO_PIN_SET);
		break;
	case 6:
		HAL_GPIO_WritePin(G6_GPIO_Port, G6_Pin, GPIO_PIN_SET);
		break;
	case 7:
		HAL_GPIO_WritePin(G7_GPIO_Port, G7_Pin, GPIO_PIN_SET);
		break;
	case 8:
		HAL_GPIO_WritePin(G8_GPIO_Port, G8_Pin, GPIO_PIN_SET);
		break;
	case 9:
		HAL_GPIO_WritePin(G9_GPIO_Port, G9_Pin, GPIO_PIN_SET);
		break;
	}
}
