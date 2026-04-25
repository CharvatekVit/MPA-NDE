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

/**
 * @brief  It handles the execution of motion commands.
 * @param  p_reg      command register, involves
 * 			          information about system state and
 * 			          switching time of valves
 * @param  p_set_pos  the orientation is maintained by
 *                    the regulation loop.
 *
 * It is a state machine that selects the appropriate
 * function based on the configuration of the command
 * register.
 *
 * Available states:\n
 * | State           | RUN_BIT | PULSE_BIT | TURN_BIT | HOME_BIT | RETURN_BIT |
 * | :---            | :---    | :---      | :---     | :---     | :---       |
 * | **START**       | 1       | 1         | X	      | X        | X          |
 * | **START**       | 1       | X         | 1	      | X        | X          |
 * | **START**       | 1       | X         | X	      | 1        | X          |
 * | **FLY**         | 1       | 0         | 0	      | 0        | X          |
 * | **STOP PULSE**  | 0       | 1         | X	      | X        | X          |
 * | **STOP TURN**   | 0       | 0         | 1	      | X        | X          |
 * | **STOP HOME**   | 0       | 0         | 0	      | 1        | X          |
 * | **STOP RETURN** | 0       | 0         | 0	      | 0        | 1          |
 *
 * The START state indicates that a start pulse is requested. In addition
 * to the corresponding command flag, at least one of the PULSE_BIT,
 * TURN_BIT, or HOME_BIT flags must be set, while the RUN_BIT flag must
 * be cleared. The valve closing time is initialized, and — based on
 * the value of DIR_BIT — the appropriate valve is opened. After each
 * command is executed, the value of set_pos is updated.
 *
 * The STOP states occur after a pulse has been triggered and its
 * duration has elapsed. They are selected based on the command
 * flag that is active at that moment.
 *
 * STOP PULSE is activated by the PULSE_BIT flag. In this state,
 * the valves are simply closed and the PULSE_BIT flag is cleared.
 *
 * STOP TURN is activated by the TURN_BIT flag. The required orientation
 * is computed using set_data.angle. The TURN_BIT flag is cleared,
 * and by setting the RUN_BIT flag, the system transitions into the FLY state.
 *
 * STOP HOME is activated by the HOME_BIT flag. The required orientation
 * is set according to set_data.home. The HOME_BIT flag is cleared,
 * and the system enters the FLY state by setting RUN_BIT.
 *
 * STOP RETURN is activated by the RETURN_BIT flag. The required orientation
 * is set to p_set_home. The RETURN_BIT flag is cleared, and the
 * RUN_BIT flag is set, causing a transition to the FLY state.
 *
 * The FLY state is active when only the RUN_BIT flag is set.
 * The satellite is in free flight, and the system waits until
 * the stop condition is satisfied. Once the condition is met,
 * the DIR_BIT flag is inverted and the duration of the stopping pulse
 * is computed. The system then transitions to the START state
 * with the PULSE_BIT flag set.
 *
 */
void valve_fcn(uint32_t * p_reg, float * p_set_pos);


void regul_fcn(uint32_t * p_reg, float set_pos);

/** @} */
#endif /* INC_LOOP_FCN_H_ */
