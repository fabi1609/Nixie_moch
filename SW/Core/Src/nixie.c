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

//DCF77
uint32_t dcf77_1ms_value=0;
uint32_t dcf77_ms_counter=0;
DCF77_interal_t dcf77_int;

static void nixie_display();
static void nixie_display_reset_all();
static void nixie_display_set_anode(uint8_t n);

//DCF77
void P_Store_Value(uint8_t value, uint8_t pos);

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

	//DCF77
	else if(htim->Instance == htim1.Instance)
	{
		dcf77_ms_counter++;
		if(dcf77_ms_counter>=1000) {
			dcf77_ms_counter=0;
			if(DCF77_TIME.sek<59) DCF77_TIME.sek++;
		}

		dcf77_1ms_value++;
		if(dcf77_int.mode==M77_Init) {
			dcf77_1ms_value=0;
			dcf77_int.mode=M77_Wait4Signal;
		}
		else if(dcf77_int.mode==M77_Wait4Signal) {
		// nothing to do
		}
		else if(dcf77_int.mode==M77_Wait4Sync) {
			if(dcf77_1ms_value>DCF77_SYNC) {
				dcf77_1ms_value=0;
				dcf77_int.mode=M77_SyncOk;
			}
		}
		else if(dcf77_int.mode==M77_SyncOk) {
			if(dcf77_1ms_value>DCF77_TIMOUT) dcf77_int.mode=M77_Error;
		}
		else if(dcf77_int.mode==M77_Wait4Time) {
			if(dcf77_1ms_value>DCF77_TIMOUT) dcf77_int.mode=M77_Error;
		}
		else if(dcf77_int.mode==M77_Ready) {
			if(dcf77_1ms_value>DCF77_TIMOUT) dcf77_int.mode=M77_Error;
		}
		else if(dcf77_int.mode==M77_Error) {
			dcf77_int.ok=0;
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

//--------------------------------------------------------------
// init vom DCF-77 Modul
//--------------------------------------------------------------
void UB_DCF77_Init(void)
{
  dcf77_1ms_value=0;
  dcf77_ms_counter=0;

  dcf77_int.mode=M77_Init;
  dcf77_int.ok=0;
  dcf77_int.min=0;
  dcf77_int.std=0;
  dcf77_int.tag=0;
  dcf77_int.monat=0;
  dcf77_int.jahr=0;

  DCF77_TIME.sek=0;
  DCF77_TIME.min=0;
  DCF77_TIME.std=0;
  DCF77_TIME.tag=0;
  DCF77_TIME.monat=0;
  DCF77_TIME.jahr=0;

  HAL_TIM_Base_Start_IT(&htim1);
}


//--------------------------------------------------------------
// aktuelle Zeit vom DCF77-Modul auslesen
// ret_wert :
//   DCF77_NO_SIGNAL  = es wird kein Signal empfangen
//   DCF77_READING    = signal wird empfangen, warte auf uhrzeit
//   DCF77_TIME_ERROR = fehler beim empfang
//   DCF77_TIME_OK    = ok, Uhrzeit steht in "DCF77_TIME"
//--------------------------------------------------------------
DCF77_Status_t UB_DCF77_ReadTime(void)
{
  DCF77_Status_t ret_wert=DCF77_NO_SIGNAL;

  if(dcf77_int.ok==1) {
    ret_wert=DCF77_TIME_OK;
  }
  else {
    if((dcf77_int.mode==M77_Init) || (dcf77_int.mode==M77_Wait4Signal)) {
      ret_wert=DCF77_NO_SIGNAL;
    }
    else if(dcf77_int.mode==M77_Error) {
      ret_wert=DCF77_TIME_ERROR;
    }
    else {
      ret_wert=DCF77_READING;
    }
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// bearbeite ein bit der uhrzeit
//--------------------------------------------------------------
void P_Store_Value(uint8_t value, uint8_t pos)
{
  static uint8_t bytewert=0;

  switch(pos) {
    // minuten
    case 21 : if(value==1) bytewert+=1;break;
    case 22 : if(value==1) bytewert+=2;break;
    case 23 : if(value==1) bytewert+=4;break;
    case 24 : if(value==1) bytewert+=8;break;
    case 25 : if(value==1) bytewert+=10;break;
    case 26 : if(value==1) bytewert+=20;break;
    case 27 :
      if(value==1) bytewert+=40;
      dcf77_int.min=bytewert;
      bytewert=0;
      break;
    // stunden
    case 29 : if(value==1) bytewert+=1;break;
    case 30 : if(value==1) bytewert+=2;break;
    case 31 : if(value==1) bytewert+=4;break;
    case 32 : if(value==1) bytewert+=8;break;
    case 33 : if(value==1) bytewert+=10;break;
    case 34 :
      if(value==1) bytewert+=20;
      dcf77_int.std=bytewert;
      bytewert=0;
      break;
    // tag
    case 36 : if(value==1) bytewert+=1;break;
    case 37 : if(value==1) bytewert+=2;break;
    case 38 : if(value==1) bytewert+=4;break;
    case 39 : if(value==1) bytewert+=8;break;
    case 40 : if(value==1) bytewert+=10;break;
    case 41 :
      if(value==1) bytewert+=20;
      dcf77_int.tag=bytewert;
      bytewert=0;
      break;
    // monat
    case 45 : if(value==1) bytewert+=1;break;
    case 46 : if(value==1) bytewert+=2;break;
    case 47 : if(value==1) bytewert+=4;break;
    case 48 : if(value==1) bytewert+=8;break;
    case 49 :
      if(value==1) bytewert+=10;
      dcf77_int.monat=bytewert;
      bytewert=0;
      break;
    // jahr
    case 50 : if(value==1) bytewert+=1;break;
    case 51 : if(value==1) bytewert+=2;break;
    case 52 : if(value==1) bytewert+=4;break;
    case 53 : if(value==1) bytewert+=8;break;
    case 54 : if(value==1) bytewert+=10;break;
    case 55 : if(value==1) bytewert+=20;break;
    case 56 : if(value==1) bytewert+=40;break;
    case 57 :
      if(value==1) bytewert+=80;
      dcf77_int.jahr=bytewert;
      bytewert=0;
      break;
    // ende
    case 58 :
      bytewert=0;
      dcf77_ms_counter=0; // syncronisiere sek_counter
      DCF77_TIME.sek=0;
      DCF77_TIME.min=dcf77_int.min;
      DCF77_TIME.std=dcf77_int.std;
      DCF77_TIME.tag=dcf77_int.tag;
      DCF77_TIME.monat=dcf77_int.monat;
      DCF77_TIME.jahr=dcf77_int.jahr;
      dcf77_int.mode=M77_Ready;
      dcf77_int.ok=1; // set flag
      break;
  }
}

//--------------------------------------------------------------
// ISR (Exti-0) [Lo+Hi-Flanke]
//--------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  GPIO_PinState wert;
  uint32_t signal_value=0;
  static uint8_t pos=0;

  wert=HAL_GPIO_ReadPin(DCF77_GPIO_Port, DCF77_Pin);
  if(wert==GPIO_PIN_RESET) {
    // lo Flanke
    signal_value=dcf77_1ms_value;
    if(dcf77_int.mode==M77_Wait4Time) {
      if((signal_value>=DCF77_LO_MIN) && (signal_value<=DCF77_LO_MAX)) {
        P_Store_Value(0,pos);
        pos++;
        if(dcf77_int.mode==M77_Ready) dcf77_int.mode=M77_Wait4Sync;
      }
      else if((signal_value>=DCF77_HI_MIN) && (signal_value<=DCF77_HI_MAX)) {
        P_Store_Value(1,pos);
        pos++;
        if(dcf77_int.mode==M77_Ready) dcf77_int.mode=M77_Wait4Sync;
      }
      else dcf77_int.mode=M77_Error;
    }
  }
  else {
    // hi Flanke
    dcf77_1ms_value=0;
    if(dcf77_int.mode==M77_SyncOk) {
      pos=0;
      dcf77_int.mode=M77_Wait4Time;
    }
    if((dcf77_int.mode==M77_Wait4Signal) || (dcf77_int.mode==M77_Error)) dcf77_int.mode=M77_Wait4Sync;
  }
}
