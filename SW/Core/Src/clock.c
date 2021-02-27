/*
 * clock.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Fabian
 */

#include "clock.h"
#include "nixie.h"
#include "settings.h"
#include "gpio.h"
#include "tim.h"
#include "rtc.h"

RTC_TimeTypeDef sTime1;
RTC_DateTypeDef sDate1;

volatile uint8_t ESP01_state = CL_STATE_INIT;
volatile uint8_t ESP01_h;
volatile uint8_t ESP01_m;
volatile uint8_t ESP01_s;

//Called every second
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_RTC_GetTime(hrtc, &sTime1, RTC_FORMAT_BIN); // Get Time
	HAL_RTC_GetDate(hrtc, &sDate1, RTC_FORMAT_BIN); // Get Date

	//Set clock
	if(!HAL_GPIO_ReadPin(S2_GPIO_Port, S2_Pin))
	{
		if(HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin))
		{
			sTime1.SecondFraction = 0;
			sTime1.Seconds = 0;
			if(sTime1.Minutes < 59)
				sTime1.Minutes = sTime1.Minutes + 1;
			else
				sTime1.Minutes = 0;
		}
		else
		{
			if(sTime1.Hours < 23)
				sTime1.Hours = sTime1.Hours + 1;
			else
				sTime1.Hours = 0;
		}
		HAL_RTC_SetTime(hrtc, &sTime1, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(hrtc, &sDate1, RTC_FORMAT_BIN);
	}

	//Display Time
	nixie_set_time(sTime1.Hours, sTime1.Minutes, sTime1.Seconds);

	//Scheduled reboot
	if(sTime1.Hours == REBOOT_H && sTime1.Minutes == REBOOT_M && sTime1.Seconds == 0)
	{
		HAL_NVIC_SystemReset();
	}
}

void ESP01_init()
{
	HAL_TIM_Base_Start(&htim17);

}

//get time from ESP01 Board
//trigger on falling edge
void OWN_LL_EXTI()
{
	switch(ESP01_state)
	{
	case CL_STATE_INIT:
		ESP01_h = 0;
		ESP01_m = 0;
		ESP01_s = 0;
		__HAL_TIM_SetCounter(&htim17, 0); //TIM17 counts with 1000 Hz
		ESP01_state = CL_STATE_READ_START;
		break;
	case CL_STATE_READ_START:
		if(__HAL_TIM_GET_COUNTER(&htim17) > 22 && __HAL_TIM_GET_COUNTER(&htim17) < 28)
		{
			ESP01_state = CL_STATE_READ_H; //read h when pause 25 ms was send.
			__HAL_TIM_SetCounter(&htim17, 0);
		}
		else if(__HAL_TIM_GET_COUNTER(&htim17) > 12 && __HAL_TIM_GET_COUNTER(&htim17) < 18)
		{
			ESP01_state = CL_STATE_READ_M; //read m when pause 15 ms was send.
			__HAL_TIM_SetCounter(&htim17, 0);
		}
		else if(__HAL_TIM_GET_COUNTER(&htim17) > 2 && __HAL_TIM_GET_COUNTER(&htim17) < 8)
		{
			ESP01_state = CL_STATE_READ_S; //read s when pause 5 ms was send.
			__HAL_TIM_SetCounter(&htim17, 0);
		}
		else
			HAL_NVIC_SystemReset();
		break;
	case CL_STATE_READ_H: //read h
		//read h finished when pause 25 ms was send and value plausible.
		if(ESP01_h < 25 && __HAL_TIM_GET_COUNTER(&htim17) > 22 && __HAL_TIM_GET_COUNTER(&htim17) < 28)
		{
			sTime1.Hours = ESP01_h - 1;
			HAL_RTC_SetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
			HAL_RTC_SetDate(&hrtc, &sDate1, RTC_FORMAT_BIN);
			ESP01_state = CL_STATE_INIT;
		}
		else if(ESP01_h < 25 && __HAL_TIM_GET_COUNTER(&htim17) <= 22)
		{
			ESP01_h++;
			__HAL_TIM_SetCounter(&htim17, 0);
		}
		else
		{
			// Error
			HAL_NVIC_SystemReset();
		}
		break;
	case CL_STATE_READ_M: //read m
		//read m finished when pause 15 ms was send and value plausible.
		if(ESP01_m < 61 && __HAL_TIM_GET_COUNTER(&htim17) > 12 && __HAL_TIM_GET_COUNTER(&htim17) < 18)
		{
			sTime1.Minutes = ESP01_m - 1;
			HAL_RTC_SetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
			HAL_RTC_SetDate(&hrtc, &sDate1, RTC_FORMAT_BIN);
			ESP01_state = CL_STATE_INIT;
		}
		else if(ESP01_m < 61 && __HAL_TIM_GET_COUNTER(&htim17) <= 12)
		{
			ESP01_m++;
			__HAL_TIM_SetCounter(&htim17, 0);
		}
		else
		{
			// Error
			HAL_NVIC_SystemReset();
		}
		break;
	case CL_STATE_READ_S: //read s
		//read s finished when pause 5 ms was send and value plausible.
		if(ESP01_s < 61 && __HAL_TIM_GET_COUNTER(&htim17) > 2 && __HAL_TIM_GET_COUNTER(&htim17) < 8)
		{
			sTime1.Seconds = ESP01_s - 1;
			sTime1.SecondFraction = 1;
			HAL_RTC_SetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
			HAL_RTC_SetDate(&hrtc, &sDate1, RTC_FORMAT_BIN);
			ESP01_state = CL_STATE_INIT;
		}
		else if(ESP01_s < 61 && __HAL_TIM_GET_COUNTER(&htim17) <= 2) //timeout
		{
			ESP01_s++;
			__HAL_TIM_SetCounter(&htim17, 0);
		}
		else
		{
			// Error
			HAL_NVIC_SystemReset();
		}
		break;
	}
}
