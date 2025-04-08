/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define ADS1299_NDRDY_Pin GPIO_PIN_5
#define ADS1299_NDRDY_GPIO_Port GPIOA
#define ADS1299_NDRDY_EXTI_IRQn EXTI9_5_IRQn
#define ADS1299_NPWDN_Pin GPIO_PIN_4
#define ADS1299_NPWDN_GPIO_Port GPIOC
#define ADS1299_NRESET_Pin GPIO_PIN_5
#define ADS1299_NRESET_GPIO_Port GPIOC
#define ADS1299_START_Pin GPIO_PIN_0
#define ADS1299_START_GPIO_Port GPIOB
#define PMU_MODE_Pin GPIO_PIN_10
#define PMU_MODE_GPIO_Port GPIOC
#define PMU_ILIM1_Pin GPIO_PIN_11
#define PMU_ILIM1_GPIO_Port GPIOC
#define PMU_ILIM0_Pin GPIO_PIN_12
#define PMU_ILIM0_GPIO_Port GPIOC
#define PMU_NFAULT_Pin GPIO_PIN_0
#define PMU_NFAULT_GPIO_Port GPIOD
#define PMU_EN1_Pin GPIO_PIN_1
#define PMU_EN1_GPIO_Port GPIOD
#define PMU_EN2_Pin GPIO_PIN_2
#define PMU_EN2_GPIO_Port GPIOD
#define PMU_EN3_Pin GPIO_PIN_3
#define PMU_EN3_GPIO_Port GPIOD
#define PMU_EN4_Pin GPIO_PIN_4
#define PMU_EN4_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
