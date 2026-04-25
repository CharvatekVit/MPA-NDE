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

/** Constant for stop pulse calculation, left valve. */
#define STOP_CONST_LEFT    10

/** Constant for stop pulse calculation, right valve. */
#define STOP_CONST_RIGHT   10

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


static void regul_stop(uint32_t * p_reg, uint32_t * p_ticks);
static void regul_return(uint32_t * p_reg, uint32_t * p_ticks, float set_pos);

/** @} */
#endif /* INC_LOOP_FCN_DEF_H_ */
