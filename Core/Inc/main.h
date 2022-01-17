/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <SintMate.h>

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUZZER_Pin GPIO_PIN_2
#define BUZZER_GPIO_Port GPIOE
#define FLASH_SS_Pin GPIO_PIN_4
#define FLASH_SS_GPIO_Port GPIOA
#define TOUCH_IRQ_Pin GPIO_PIN_0
#define TOUCH_IRQ_GPIO_Port GPIOB
#define TOUCH_IRQ_EXTI_IRQn EXTI0_IRQn
#define NFC1_Pin GPIO_PIN_1
#define NFC1_GPIO_Port GPIOB
#define NFC0_Pin GPIO_PIN_2
#define NFC0_GPIO_Port GPIOB
#define TOUCH_CS_Pin GPIO_PIN_10
#define TOUCH_CS_GPIO_Port GPIOE
#define NFC2_Pin GPIO_PIN_11
#define NFC2_GPIO_Port GPIOE
#define DIRSTEP_Pin GPIO_PIN_9
#define DIRSTEP_GPIO_Port GPIOD
#define ENASTEP_Pin GPIO_PIN_10
#define ENASTEP_GPIO_Port GPIOD
#define DEBUG_Pin GPIO_PIN_15
#define DEBUG_GPIO_Port GPIOA
#define RST_Pin GPIO_PIN_3
#define RST_GPIO_Port GPIOD
#define DC_Pin GPIO_PIN_4
#define DC_GPIO_Port GPIOD
#define CS_Pin GPIO_PIN_6
#define CS_GPIO_Port GPIOD
#define VL53L0X_RST_Pin GPIO_PIN_0
#define VL53L0X_RST_GPIO_Port GPIOE
#define VL53L0X_IRQ_Pin GPIO_PIN_1
#define VL53L0X_IRQ_GPIO_Port GPIOE
#define VL53L0X_IRQ_EXTI_IRQn EXTI1_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
