/*
 * Command processing library
 * (c) Antonin_Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/**
 * @file      Sensor function library <sensor_fcn.h>
 * @defgroup  Sensors
 * @code #include <sensor_fcn.h> @endcode
 *
 * @brief It is responsible for processing the sensor data.
 *
 * The library provides manual gyroscope calibration, sensor
 * data acquisition, and reformatting of the orientation value
 * into the –180° to +180° range. It serves as an additional
 * layer on top of the mpu9250 library from driverlib.
 * @{
 */

#ifndef INC_SENSOR_FCN_H_
#define INC_SENSOR_FCN_H_

/* Includes -----------------------------------------------*/
#include "main.h"

/* Defines ------------------------------------------------*/


/* Extern global variables --------------------------------*/
extern sensor_data_t measured_data;
extern settings_t    set_data;

/* Function prototypes ------------------------------------*/
/**
 * @brief  It initiates communication with the sensor.
 * @param  gyr_offset    gyroscope offset obtained repeated measurement
 *
 * This function initiates communication with the MPU9250 sensor,
 * using the SPI interface. It then calls the sensor_calibration() function,
 * which determines the gyroscope offset. The resulting offset value
 * is subsequently taken into account throughout the rest of the program execution.
 *
 */
void sensor_init(float * gyr_offset);

/**
 * @brief  It handles reading the value from the sensor.
 * @param  gyr_offset    gyroscope offset obtained repeated measurement
 *
 * The data are read using the mpu9250_basic_read() function from
 * The the mpu9250 library. The values are stored in the global variable
 * measure_data. The gyroscope data are corrected using the previously
 * determined offset, and the orientation is then computed.
 *
 */
void sensor_get_value(float * gyr_offset);

/**
 * @brief  It normalizes the orientation value to the –180° to +180° range.
 * @param  p_deg         gyroscope offset obtained repeated measurement
 *
 * The orientation data are converted into an angular format.
 * For values greater than 180°, 360° is subtracted.
 * For values less than –180°, 360° is added.
 *
 */
void sensor_deg_limit(float * p_deg);

/** @} */
#endif /* INC_SENSOR_FCN_H_ */
