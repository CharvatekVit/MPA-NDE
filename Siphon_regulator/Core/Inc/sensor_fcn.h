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


void sensor_get_value(float * gyr_offset);

void sensor_deg_limit(float * p_deg);

/** @} */
#endif /* INC_SENSOR_FCN_H_ */
