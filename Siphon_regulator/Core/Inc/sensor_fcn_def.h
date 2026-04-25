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
/** How much calibration measurement is performed. */
#define CAL_CYC_NUM             100

/** How often sensor is readed in milliseconds. */
#define MEAS_TIME                10

/** How often sensor is readed during calibration in milliseconds. */
#define CAL_TIME                 10

/* Static functions declaration ---------------------------*/
/* Function prototypes ------------------------------------*/
/**
 * @brief  It performs the computation of the gyroscope offset.
 * @param  gyr_offset    gyroscope offset obtained repeated measurement
 *
 * It measures the angular velocity 100 times with a 10 ms period.
 * The result is then averaged. During this procedure, the satellite
 * must remain perfectly stationary.
 *
 */
static void sensor_calibration(float * gyr_offset);

/** @} */
#endif /* INC_SENSOR_FCN_DEF_H_ */
