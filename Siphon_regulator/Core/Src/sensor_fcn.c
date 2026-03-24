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

#define MEAS_TIME        10 // how often sensor is readed
#define CAL_TIME          10 // how often sensor is readed during calibration

/* Function declaration */
//static uint8_t sensor_read(float *, float *, float *);

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

	/* Temporary variables for calibration */
	float acc[3];
	float gyr[3];
	float mag[3];

	float offset[3] = {0};
	uint8_t i;

	/* Calibration */
	for (i = 0; i < CAL_CYC_NUM; i++)
	{
		if (mpu9250_basic_read(acc, gyr, mag) == 0)
		{
			printf("M: %d %d %d\n", _float2int(gyr[0]), _float2int(gyr[1]), _float2int(gyr[2]));

			for (uint8_t j = 0; j < 3; j++)
			{
				offset[j] += gyr[j];
			}
		}

		HAL_Delay(CAL_TIME);
	}
	// printf("%d\n", i);

	for (uint8_t j = 0; j < 3; j++)
	{
		gyr_offset[j] = (offset[j] / i);
	}

	/*
	printf("Calibration\n");
	printf("%d\n", _float2int(gyr_offset[0]));
	printf("%d\n", _float2int(gyr_offset[1]));
	printf("%d\n", _float2int(gyr_offset[2]));
	 */
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
			}
		}
	}

	/*
	printf("Measured\n");
	printf("%d\n", _float2int(measured_data.gyr[0]));
	printf("%d\n", _float2int(measured_data.gyr[1]));
	printf("%d\n", _float2int(measured_data.gyr[2]));
	*/

	/*
	printf("Read\n");
	printf("%d\n", _float2int(gyr_offset[0]));
	printf("%d\n", _float2int(gyr_offset[1]));
	printf("%d\n", _float2int(gyr_offset[2]));
	 */
}

/* Local function */
/*
uint8_t sensor_read(float * acc, float * gyr, float * mag)
{
	if (mpu9250_basic_read(acc, gyr, mag) == 0)
	{
		//printf("Sensor reading failed!\n");
		return 0;
	}
	return 1;
}
*/
