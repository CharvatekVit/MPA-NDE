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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
    float acc[3]; // Accelerometer X, Y, Z
    float gyr[3]; // Gyroscope X, Y, Z
    float mag[3]; // Magnetometer X, Y, Z
} sensor_data_t;

typedef struct {
    uint8_t angle;  // Angle [deg], cube rotate after turn cmd
	uint8_t time_r; // Time [ms], right valve is open
	uint8_t time_l; // Time [ms], left valve is open
	uint8_t time_a; // Period [s*0.1], sensor report will be sended
	int8_t  home[2]; // X, Y coordinates default position
} settings_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define _read_BV(reg, bit)   reg & (1 << bit)
#define _set_BV(reg, bit)    reg = reg | (1 << bit)
#define _clr_BV(reg, bit)    reg = reg & ~(1 << bit)
#define _tog_BV(reg, bit)    reg = reg ^ (1 << bit)

// Time data are included in bites 15 to 8 (right valve) and  in 23 to 16 (left valve)
#define _clr_time(reg)	                reg = reg & ~0x00FFFF00
#define _set_time(reg, time_l, time_r)  reg = reg | (time_r << 8) | (time_l << 16)
#define _read_time_r(reg)               ((reg & 0x0000FF00) >> 8)
#define _read_time_l(reg)               ((reg & 0x00FF0000) >> 16)

// Display float interpretation
#define _float2int(data_fl)             ((int16_t)(data_fl*1000))

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BOARD_LED_Pin GPIO_PIN_13
#define BOARD_LED_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOA
#define LED_Y_Pin GPIO_PIN_6
#define LED_Y_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOA
#define VALVE_R_Pin GPIO_PIN_8
#define VALVE_R_GPIO_Port GPIOA
#define VALVE_L_Pin GPIO_PIN_9
#define VALVE_L_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_9
#define SPI2_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RUN_BIT           0 // movement was required
#define DIR_BIT           1 // 0 - left, 1 - right
#define PULSE_BIT         2 // only one pulse
#define TURN_BIT          3 // change attitude, 2 pulses
#define REG_BIT           4 // active if regulation is performed
#define HOME_BIT          7 // return to home position
#define AUTOREAD_BIT     24 // periodic sending messages through serial
#define AUTOREAD_ACC_BIT 25 // autoread includes accelerometer data
#define AUTOREAD_GYR_BIT 26 // autoread includes gyroscope data
#define AUTOREAD_MAG_BIT 27 // autoread includes magnetometer data

#define MEAS_TIME        10 // how often sensor is readed

/* Macros */
#define _read_valve(reg) reg & ((1 << RUN_BIT) | (1 << TURN_BIT) | (1 << PULSE_BIT) | (1 << HOME_BIT) | (1 << REG_BIT))
/* Reserved */
#define CMD0_BIT         28
#define CMD1_BIT         29
#define CMD2_BIT         30
#define CMD3_BIT         31
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
