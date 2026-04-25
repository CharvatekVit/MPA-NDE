/*
 * Loop functions library
 * (c) Antonin_Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/**
 * @file      Autoread and motion library <loop_fcn.h>
 * @defgroup  Loop
 * @code #include <loop_fcn.h> @endcode
 *
 * @brief Process commands send to MCU through serial port.
 *
 * It contains the functions executed within the main loop.
 * It enables automatic sensor reading, motion commands performing,
 * and attitude regulating.
 *
 * @{
 */

#ifndef INC_LOOP_FCN_H_
#define INC_LOOP_FCN_H_

/* Includes -----------------------------------------------*/
#include "main.h"

/* Extern global variables --------------------------------*/
extern sensor_data_t measured_data; ///< Current sensor measured data.
extern settings_t    set_data;      ///< Main system settings.

/* Function prototypes ------------------------------------*/
void autoread_fcn(uint32_t);
void valve_fcn(uint32_t *, float *);
void regul_fcn(uint32_t *, float);

/** @} */
#endif /* INC_LOOP_FCN_H_ */
