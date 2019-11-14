/*
 * clock.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Fabian
 */

#include "clock.h"
#include "nixie.h"
#include "gpio.h"

RTC_TimeTypeDef sTime1;
RTC_DateTypeDef sDate1;

//Called every second
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_RTC_GetTime(hrtc, &sTime1, RTC_FORMAT_BIN); // Get Time
	HAL_RTC_GetDate(hrtc, &sDate1, RTC_FORMAT_BIN); // Get Date

	//Set clock, when button pushed
	if(!HAL_GPIO_ReadPin(S2_GPIO_Port, S2_Pin))
	{
		sTime1.SecondFraction = 0;
		sTime1.Seconds = 0;
		if(sTime1.Minutes < 59)
		{
			sTime1.Minutes = sTime1.Minutes + 1;
		}
		else
		{
			sTime1.Minutes = 0;
			if(sTime1.Hours < 23)
			{
				sTime1.Hours = sTime1.Hours + 1;
			}
			else
			{
				sTime1.Hours = 0;
			}
		}
		HAL_RTC_SetTime(hrtc, &sTime1, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(hrtc, &sDate1, RTC_FORMAT_BIN);
	}

	nixie_set_time(sTime1.Hours, sTime1.Minutes, sTime1.Seconds);
}
