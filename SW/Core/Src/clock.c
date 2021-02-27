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

volatile uint8_t ESP01_state = 0;
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

	//Scheduled Reset
	if(sTime1.Hours == CLOCK_RESET_TIME_H && sTime1.Minutes == CLOCK_RESET_TIME_M && sTime1.Seconds == 0)
	{
		HAL_NVIC_SystemReset();
	}

	//Display Time
	nixie_set_time(sTime1.Hours, sTime1.Minutes, sTime1.Seconds);
}

void ESP01_init()
{
	HAL_TIM_Base_Start(&htim17);

}

//get time from ESP01 Board
void OWN_LL_EXTI(uint8_t ulGpioState)
{
	uint16_t minutes = 0;

	if(ulGpioState)
	{
		//Read Time on rising edge

		minutes = (__HAL_TIM_GET_COUNTER(&htim17) / 10);
		sTime1.Hours = minutes / 60;
		minutes -= (sTime1.Hours * 60);
		sTime1.Minutes = minutes;

		HAL_RTC_SetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &sDate1, RTC_FORMAT_BIN);
	}
	else
	{
		//Set Counter to 0 on falling edge
		__HAL_TIM_SetCounter(&htim17, 0); //TIM17 counts with 1000 Hz
	}
}
