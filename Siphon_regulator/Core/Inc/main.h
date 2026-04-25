/* USER CODE BEGIN Header */
/*
 * Siphon Regulator library
 * (c) Antonin Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/**
 * @mainpage Siphon Regulator
 *
 * Attitude determination and control system (ADCS), together
 * with on-board computer (OBC), communication system (COM)
 * and electrical power system (EPS), represents one of
 * the fundamental building blocks of nanosatellites.Attitude
 * control is essential when the satellite carries, for example,
 * a camera or a high-gain antenna. For CubeSats, reaction wheels
 * or magnetorquers are typical components that enable the
 * CubeSat to adjust its orientation.
 *
 * Using reaction-based pneumatic propulsion system (RPPS)
 * is not a conventional approach; however, in addition to adjusting
 * the orientation, they can also induce linear motion, thereby
 * influencing the orbit of the satellite.
 *
 * The goal of the project was to demonstrate the use of an RPPS
 * to control the orientation of a CubeSat. For the demonstration,
 * a single axis was selected due to limited financial resources,
 * time constraints, and the relative ease of testing under terrestrial
 * conditions. An integral part of the ADCS is also gyroscope,
 * which is used to determine angular position data.
 *
 * @author    Antonin Putala,
 *            Dept. of Radio Electronics, Brno University of Technology, Czechia
 * @copyright (c) 2026 Antonin Putala,
 *            This work is licensed under the terms of the MIT license
 */
/* USER CODE END Header */

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
/**  Current sensor measured data. */
typedef struct {
    float acc[3];   ///< Accelerometer X, Y, Z [g]
    float gyr[3];   ///< Gyroscope     X, Y, Z [dps]
    float mag[3];   ///< Magnetometer  X, Y, Z [uT]
    float pos[3];   ///< Orientation   X, Y, Z [deg]; related to initial position
} sensor_data_t;

/**  Main system settings. */
typedef struct {
    uint8_t angle;  ///< Angle     [deg], cube rotate after turn cmd
	uint8_t time_r; ///< Time    [10*ms], right valve is open
	uint8_t time_l; ///< Time    [10*ms], left valve is open
	uint8_t time_a; ///< Period [100*ms], sensor report will be sended
	int16_t home;   ///< Angle     [deg], default orientation
} settings_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/** Check value of single bit in register. */
#define _read_BV(reg, bit)   reg & (1 << bit)

/** Set single bit in register. */
#define _set_BV(reg, bit)    reg = reg | (1 << bit)

/** Clear single bit in register. */
#define _clr_BV(reg, bit)    reg = reg & ~(1 << bit)

/** Toggle single bit in register. */
#define _tog_BV(reg, bit)    reg = reg ^ (1 << bit)

/** Time data are included in bites 15 to 8 (right valve)
 *  and  in 23 to 16 (left valve).
 *  Set time to null.
 */
#define _clr_time(reg)	                reg = reg & ~0x00FFFF00

/** Set open-valve time to the defined values, separately for left and
 * right valve. */
#define _set_time(reg, time_l, time_r)  reg = reg | (time_r << 8) | (time_l << 16)

/** Read time settings for right valve. */
#define _read_time_r(reg)               ((reg & 0x0000FF00) >> 8)

/** Read time settings for left valve. */
#define _read_time_l(reg)               ((reg & 0x00FF0000) >> 16)

/** Float value is converted to integer. For better resolution
 * is multiplied by 100.
 */
#define _float2int(data_fl)             ((int16_t)(data_fl*1000))

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BOARD_LED_Pin GPIO_PIN_13
#define BOARD_LED_GPIO_Port GPIOC
#define VALVE_R_Pin GPIO_PIN_2
#define VALVE_R_GPIO_Port GPIOA
#define VALVE_L_Pin GPIO_PIN_3
#define VALVE_L_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOA
#define LED_Y_Pin GPIO_PIN_6
#define LED_Y_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_9
#define SPI2_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/** If active, a new action is requested. */
#define RUN_BIT           0

/** 0 - left, 1 - right */
#define DIR_BIT           1

/** If active, the pulse command is being executed. */
#define PULSE_BIT         2

/** If active, the turn command is being executed. */
#define TURN_BIT          3

/** If active, the satellite counteracts external deflection. */
#define REG_BIT           4

/** If active, the satellite returns to the position prior to deflection. */
#define RETURN_BIT        5

/** If active, the home command is being executed. */
#define HOME_BIT          7

/** If active, global sending of the automatic reading is enabled. */
#define AUTOREAD_BIT     24

/** If active, the automatic message includes the accelerometer reading. */
#define AUTOREAD_ACC_BIT 25

/** If active, the automatic message includes the gyroscope reading. */
#define AUTOREAD_GYR_BIT 26

/** If active, the automatic message includes the orientation data. */
#define AUTOREAD_POS_BIT 27

/** Valve time is multiplied by 10. */
#define PULSE_REPRE      10 //


/* Macros */
/** Check if any command or method is performed. */
#define _read_valve(reg) reg & ((1 << RUN_BIT) | (1 << TURN_BIT) | (1 << PULSE_BIT) | (1 << HOME_BIT) | (1 << REG_BIT) | (1 << RETURN_BIT))

/* Calibration constant */
/** Calibration constant, get from comparison required angle for turn and measured */
#define CAL_MUL     ((360.0f/328.0f)*(360.0f/349.0f))

/* Reserved */
/** 0 - disable regulation, 1 - enable regulation */
#define CMD0_BIT         28

/** Reserved */
#define CMD1_BIT         29

/** Reserved */
#define CMD2_BIT         30

/** Reserved */
#define CMD3_BIT         31
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
