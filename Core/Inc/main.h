/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_0
#define LED2_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_1
#define LED4_GPIO_Port GPIOA
#define ETH_CS_Pin GPIO_PIN_4
#define ETH_CS_GPIO_Port GPIOA
#define RL1_Pin GPIO_PIN_4
#define RL1_GPIO_Port GPIOC
#define RL2_Pin GPIO_PIN_5
#define RL2_GPIO_Port GPIOC
#define RL3_Pin GPIO_PIN_0
#define RL3_GPIO_Port GPIOB
#define RL4_Pin GPIO_PIN_1
#define RL4_GPIO_Port GPIOB
#define RL5_Pin GPIO_PIN_10
#define RL5_GPIO_Port GPIOB
#define RL6_Pin GPIO_PIN_11
#define RL6_GPIO_Port GPIOB
#define RL7_Pin GPIO_PIN_12
#define RL7_GPIO_Port GPIOB
#define RL8_Pin GPIO_PIN_13
#define RL8_GPIO_Port GPIOB
#define IO1_Pin GPIO_PIN_14
#define IO1_GPIO_Port GPIOB
#define IO2_Pin GPIO_PIN_15
#define IO2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_6
#define LED3_GPIO_Port GPIOC
#define POS1_Pin GPIO_PIN_8
#define POS1_GPIO_Port GPIOC
#define POS2_Pin GPIO_PIN_9
#define POS2_GPIO_Port GPIOC
#define SPD1_Pin GPIO_PIN_8
#define SPD1_GPIO_Port GPIOA
#define TXEN_Pin GPIO_PIN_11
#define TXEN_GPIO_Port GPIOA
#define SPD2_Pin GPIO_PIN_12
#define SPD2_GPIO_Port GPIOA
#define ETH_RST_Pin GPIO_PIN_10
#define ETH_RST_GPIO_Port GPIOC
#define INT_Pin GPIO_PIN_11
#define INT_GPIO_Port GPIOC
#define IO4_Pin GPIO_PIN_12
#define IO4_GPIO_Port GPIOC
#define IO5_Pin GPIO_PIN_2
#define IO5_GPIO_Port GPIOD
#define IO6_Pin GPIO_PIN_4
#define IO6_GPIO_Port GPIOB
#define WP_Pin GPIO_PIN_5
#define WP_GPIO_Port GPIOB
#define IO7_Pin GPIO_PIN_8
#define IO7_GPIO_Port GPIOB
#define IO8_Pin GPIO_PIN_9
#define IO8_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
