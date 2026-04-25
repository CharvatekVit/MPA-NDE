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
/**
 * @brief  It handles the periodic transmission of the sensor data message.
 * @param  reg    command register, involves
 * 			      information about system state and
 * 			      switching time of valves
 *
 * The command register specifies the requested content of the
 * autoread message. If autoread is enabled, the message is transmitted
 * periodically, with the interval defined in the global variable set_data.
 *
 * The AUTOREAD_BIT in the command register enables the automatic
 * transmission of messages. The AUTOREAD_GYR_BIT, AUTOREAD_POS_BIT,
 * and AUTOREAD_ACC_BIT flags allow the corresponding data to be
 * included in the message.
 *
 */
void autoread_fcn(uint32_t reg);


void valve_fcn(uint32_t * p_reg, float * p_set_pos);
void regul_fcn(uint32_t * p_reg, float set_pos);

/** @} */
#endif /* INC_LOOP_FCN_H_ */
