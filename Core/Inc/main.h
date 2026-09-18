/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

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
#define ENCODER_CS_Pin GPIO_PIN_11
#define ENCODER_CS_GPIO_Port GPIOC
#define PC_SVPWM_Pin GPIO_PIN_9
#define PC_SVPWM_GPIO_Port GPIOA
#define PA_SVPWM_Pin GPIO_PIN_11
#define PA_SVPWM_GPIO_Port GPIOA
#define PB_SVPWM_Pin GPIO_PIN_10
#define PB_SVPWM_GPIO_Port GPIOA
#define Ia_adc_Pin GPIO_PIN_0
#define Ia_adc_GPIO_Port GPIOC
#define Vbus_adc_Pin GPIO_PIN_1
#define Vbus_adc_GPIO_Port GPIOC
#define NTC_Pin GPIO_PIN_0
#define NTC_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_14
#define LED_STATUS_GPIO_Port GPIOB
#define nSleep_Pin GPIO_PIN_13
#define nSleep_GPIO_Port GPIOB
#define Ib_adc_Pin GPIO_PIN_3
#define Ib_adc_GPIO_Port GPIOA
#define nFault_Pin GPIO_PIN_11
#define nFault_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
