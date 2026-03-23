/*
 * snesor_fcn.c
 *
 *  Created on: Mar 23, 2026
 *      Author: Antonin Putala
 */

/* Includes */
#include "sensor_fcn.h"
#include "main.h"
#include "driver_mpu9250_basic.h"

#include <stdio.h>

/* Defines */

/* Function declaration */


/* Global functions */

/* Inicialization */
void sensor_init(void)
{

	 /* For SPI interface set MPU9250_INTERFACE_SPI.
	    Address only for I2C, library requires it.
	 */
	uint8_t res = mpu9250_basic_init(MPU9250_INTERFACE_SPI, 0); // MPU9250_ADDRESS_AD0_LOW
	if (res != 0)
	{
		printf("Sensor error!\n");
	}
	else
	{
		printf("Sensor is ready.\n");
	}
	printf("%x\n", res);

	/* Pause for stabilization */
	HAL_Delay(100);
}

void sensor_read(void)
{
	static uint32_t tick = 0;

	/* * 2 READING
	 */
	if (HAL_GetTick() > tick)
	{
		tick = HAL_GetTick() + MEAS_TIME;

		if (mpu9250_basic_read(measured_data.acc, measured_data.gyr, measured_data.mag) != 0)
		{
			printf("Sensor reading failed!\n");
		}
	}
}

