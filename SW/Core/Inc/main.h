/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define S2_Pin LL_GPIO_PIN_13
#define S2_GPIO_Port GPIOC
#define NRST_Pin LL_GPIO_PIN_2
#define NRST_GPIO_Port GPIOF
#define GN1_Pin LL_GPIO_PIN_0
#define GN1_GPIO_Port GPIOA
#define GN2_Pin LL_GPIO_PIN_1
#define GN2_GPIO_Port GPIOA
#define DCF77_Pin LL_GPIO_PIN_2
#define DCF77_GPIO_Port GPIOA
#define DCF77_EXTI_IRQn EXTI2_3_IRQn
#define FB_170V_Pin LL_GPIO_PIN_3
#define FB_170V_GPIO_Port GPIOA
#define G1_Pin LL_GPIO_PIN_4
#define G1_GPIO_Port GPIOA
#define GDOT_Pin LL_GPIO_PIN_5
#define GDOT_GPIO_Port GPIOA
#define G3_Pin LL_GPIO_PIN_6
#define G3_GPIO_Port GPIOA
#define S1_Pin LL_GPIO_PIN_0
#define S1_GPIO_Port GPIOD
#define GN3_Pin LL_GPIO_PIN_1
#define GN3_GPIO_Port GPIOD
#define G2_Pin LL_GPIO_PIN_2
#define G2_GPIO_Port GPIOD
#define G4_Pin LL_GPIO_PIN_3
#define G4_GPIO_Port GPIOD
#define G5_Pin LL_GPIO_PIN_3
#define G5_GPIO_Port GPIOB
#define G6_Pin LL_GPIO_PIN_4
#define G6_GPIO_Port GPIOB
#define GN4_Pin LL_GPIO_PIN_5
#define GN4_GPIO_Port GPIOB
#define G0_Pin LL_GPIO_PIN_6
#define G0_GPIO_Port GPIOB
#define G9_Pin LL_GPIO_PIN_7
#define G9_GPIO_Port GPIOB
#define G8_Pin LL_GPIO_PIN_8
#define G8_GPIO_Port GPIOB
#define G7_Pin LL_GPIO_PIN_9
#define G7_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
