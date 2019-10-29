/*
 * nixie_display.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Fabian
 */

#include "nixie_display.h"
#include "main.h"

volatile uint8_t digit=0;

//TODO Timer Interrupt für Multiplex

void nixie_display(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	nixie_display_reset_all();
	switch(digit)
	{
	case 0:
		nixie_display_set_anode(hours/10);
		HAL_GPIO_WritePin(GN1_GPIO_Port, GN1_Pin, GPIO_PIN_SET);
		break;
	case 1:
		nixie_display_set_anode(hours%10);
		HAL_GPIO_WritePin(GN2_GPIO_Port, GN2_Pin, GPIO_PIN_SET);
		break;
	case 2:
		nixie_display_set_anode(minutes/10);
		HAL_GPIO_WritePin(GN3_GPIO_Port, GN3_Pin, GPIO_PIN_SET);
		break;
	case 3:
		nixie_display_set_anode(minutes%10);
		HAL_GPIO_WritePin(GN4_GPIO_Port, GN4_Pin, GPIO_PIN_SET);
		break;
	}
	if(seconds%2)
		HAL_GPIO_WritePin(GDOT_GPIO_Port, GDOT_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GDOT_GPIO_Port, GDOT_Pin, GPIO_PIN_RESET);
}

static void nixie_display_reset_all()
{
	HAL_GPIO_WritePin(GN1_GPIO_Port, GN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GN2_GPIO_Port, GN2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GN3_GPIO_Port, GN3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GN4_GPIO_Port, GN4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G3_GPIO_Port, G3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G4_GPIO_Port, G4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G5_GPIO_Port, G5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G6_GPIO_Port, G6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G7_GPIO_Port, G7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G8_GPIO_Port, G8_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(G9_GPIO_Port, G9_Pin, GPIO_PIN_RESET);
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
