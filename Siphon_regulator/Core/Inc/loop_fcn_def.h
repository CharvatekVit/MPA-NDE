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
#define ANGLE_TOL           5   // Precision setting of position
#define STOP_CONST_LEFT    10   // For stop pulse calculation
#define STOP_CONST_RIGHT   10
#define GYR_TOL          1.0f   // Ignored motion
#define PULSE_IGNORE       50   // Too short pulse

/* Static functions declaration ---------------------------*/
/* Function prototypes ------------------------------------*/
static void valve_close(void);
static void regul_stop(uint32_t *, uint32_t *);
static void regul_return(uint32_t *, uint32_t *, float);

/** @} */
#endif /* INC_LOOP_FCN_DEF_H_ */
