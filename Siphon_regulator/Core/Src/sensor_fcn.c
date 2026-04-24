/*
 * sensor_fcn library
 * (c) Antonin Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/* Includes -----------------------------------------------*/
#include "sensor_fcn.h"
#include "sensor_fcn_def.h"
#include "main.h"
#include "driver_mpu9250_basic.h"
#include <stdio.h>

/* Global functions ---------------------------------------*/
/*
 * Function: sensor_init
 * Purpose:  It initiates communication with the sensor.
 * Input(s): gyr_offset       - gyroscope offset obtained
 *                              repeated measurement
 * Returns:  none
 */
void sensor_init(float * gyr_offset)
{
	/*   For SPI interface set MPU9250_INTERFACE_SPI.
	 *   Address only for I2C, library requires it.
	 */
	uint8_t res = mpu9250_basic_init(MPU9250_INTERFACE_SPI, 0);
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

				/* The gyroscope data are corrected using the calibration constant. */
				measured_data.gyr[i] = gyr[i] - gyr_offset[i];

				measured_data.mag[i] = mag[i];

				/* CAL_MUL enables manual calibration of orientation measurement. */
				measured_data.pos[i] += (measured_data.gyr[i] * MEAS_TIME * CAL_MUL) / 1000;

				/* Get format -180° ÷ 180°. */
				sensor_deg_limit(&measured_data.pos[i]);
			}
		}
	}
}

/* Arrange position data to degree format*/
void sensor_deg_limit(float * p_deg)
{
    *p_deg += (*p_deg < -180) ? 360: 0;
    *p_deg -= (*p_deg > 180)  ? 360: 0;
}

/* Static functions ---------------------------------------*/
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

