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
#define CAL_CYC_NUM 100

#define MEAS_TIME         10 // how often sensor is readed
#define CAL_TIME          10 // how often sensor is readed during calibration

/* Function declaration */
static void sensor_calibration(float *);

/* Global functions */

/* Initialization */
void sensor_init(float * gyr_offset)
{
	/* For SPI interface set MPU9250_INTERFACE_SPI.
	    Address only for I2C, library requires it.
	 */
	uint8_t res = mpu9250_basic_init(MPU9250_INTERFACE_SPI, 0); // MPU9250_ADDRESS_AD0_LOW
	if (res != 0)
	{
		printf("Sensor error!\n");
	}

	/* Pause for stabilization */
	HAL_Delay(200);

	sensor_calibration(gyr_offset);

}

/* 2 READING */
void sensor_get_value(float * gyr_offset)
{
	static uint32_t tick = 0;

	if (HAL_GetTick() > tick)
	{
		float acc[3];
		float gyr[3];
		float mag[3];

		tick = HAL_GetTick() + MEAS_TIME;

		if (mpu9250_basic_read(acc, gyr, mag) == 0)
		{
			for (uint8_t i = 0; i < 3; i++)
			{
				measured_data.acc[i] = acc[i];
				measured_data.gyr[i] = gyr[i] - gyr_offset[i];
				measured_data.mag[i] = mag[i];
				measured_data.pos[i] += (measured_data.gyr[i] * MEAS_TIME) / 1000;
			}
		}
	}
}

/* Local function */

/* Gyroscope calibration */
static void sensor_calibration(float * gyr_offset)
{
	/* Temporary variables for calibration */
	float acc[3];
	float gyr[3];
	float mag[3];

	float offset[3] = {0};
	uint8_t i;


	for (i = 0; i < CAL_CYC_NUM; i++)
	{
		if (mpu9250_basic_read(acc, gyr, mag) == 0)
		{
			for (uint8_t j = 0; j < 3; j++)
			{
				offset[j] += gyr[j];
			}
		}

		HAL_Delay(CAL_TIME);
	}

	for (uint8_t j = 0; j < 3; j++)
	{
		gyr_offset[j] = (offset[j] / i);
	}
}

