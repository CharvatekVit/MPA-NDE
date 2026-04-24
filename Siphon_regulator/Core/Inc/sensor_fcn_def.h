/*
 * sensor_fcn library
 * (c) Antonin_Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/**
 * @file      Sensor function library definition <cmd_processing_def.h>
 * @defgroup  Sensors
 * @code #include <sensor_fcn_def.h> @endcode
 *
 * Necessary definition and function declaration.
 *
 * @copyright (c) Antonin Putala 2026
 * @{
 */

#ifndef INC_SENSOR_FCN_DEF_H_
#define INC_SENSOR_FCN_DEF_H_

/* Defines ------------------------------------------------*/
#define CAL_CYC_NUM             100 // How much calibration measurement is performed
#define MEAS_TIME                10 // how often sensor is readed
#define CAL_TIME                 10 // how often sensor is readed during calibration

/* Static functions declaration ---------------------------*/
/* Function prototypes ------------------------------------*/

static void sensor_calibration(float *);

/** @} */
#endif /* INC_SENSOR_FCN_DEF_H_ */
