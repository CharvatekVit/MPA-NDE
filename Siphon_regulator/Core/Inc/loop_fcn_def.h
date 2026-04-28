/*
 * Loop functions library
 * (c) Antonin_Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/**
 * @file      Autoread and motion library <loop_fcn_def.h>
 * @defgroup  Loop
 * @code #include <loop_fcn_def.h> @endcode
 *
 * Necessary definition and function declaration.
 *
 * @copyright (c) Antonin Putala 2026
 * @{
 */

#ifndef INC_LOOP_FCN_DEF_H_
#define INC_LOOP_FCN_DEF_H_

/* Defines ------------------------------------------------*/
/** Precision setting of position. */
#define ANGLE_TOL           5

/* Constant for stop pulse calculation - slope, left valve. */
#define STOP_CONST_LEFT    (1000.0f/7.808f)

/* Constant for stop pulse calculation - slope, right valve. */
#define STOP_CONST_RIGHT   (1000.0f/11.7f)

/* Constant for stop pulse calculation - offset, left valve. */
#define STOP_CONST_OFF_LEFT    1.552f

/* Constant for stop pulse calculation - offset, right valve. */
#define STOP_CONST_OFF_RIGHT   0.047f

/** Ignored motion, it accounts for sensor fluctuation. */
#define GYR_TOL          1.0f

/** Ignored pulse duration in milliseconds, too short pulse for performing */
#define PULSE_IGNORE       50

/* Static functions declaration ---------------------------*/
/* Function prototypes ------------------------------------*/
/**
 * @brief  Closes all valves.
 */
static void valve_close(void);

/**
 * @brief It provides the stopping pulse.
 * @param p_reg   command register, involves
 * 			      information about system state and
 * 			      switching time of valves
 * @param p_ticks number of ticks until the pulse
 *                is stopped
 *
 * Based on the sign of the polarization value,
 * one of the valves is opened: the right valve
 * for a positive value and the left valve for
 * a negative value. The required pulse duration
 * is then computed from the measured angular velocity.
 * Pulses that are too short are ignored.
 * By setting the REG_BIT flag, the system activates
 * the condition that closes the valve once the computed
 * time has elapsed.
 *
 */
static void regul_stop(uint32_t * p_reg, uint32_t * p_ticks);

/**
 * @brief It initiates the return to the stable orientation.
 * @param p_reg    command register, involves
 * 			       information about system state and
 * 			       switching time of valves
 * @param p_ticks  number of ticks until the pulse
 *                 is stopped
 * @param set_pos  the orientation is maintained by
 *                 the regulation loop.
 *
 * It triggers the first pulse of the RETURN method.
 * It sets the REG_BIT and RETURN_BIT flags. The appropriate
 * valve is selected so that the correction path is as short
 * as possible. The DIR_BIT flag is also configured to ensure
 * proper stopping behavior.The pulse duration is taken
 * directly from the global variable set_data.
 * The REG_BIT flag ensures that, once the pulse
 * duration expires, the valves are closed.
 *
 */
static void regul_return(uint32_t * p_reg, uint32_t * p_ticks, float set_pos);

/** @} */
#endif /* INC_LOOP_FCN_DEF_H_ */
