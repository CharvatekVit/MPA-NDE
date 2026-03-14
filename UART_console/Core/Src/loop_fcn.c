/*
 * loop_fcn.c
 *
 *  Created on: Mar 12, 2026
 *      Author: Antonin Putala
 */

/* Includes */
#include "loop_fcn.h"
#include "main.h"

#include <stdio.h>

/* Defines */
#define FLY_TIME 5000 // five sec

/* Global functions */

/* Automatic writing sensor data to serial */
void autoread_fcn(uint32_t reg)
{
	if (_read_BV(reg, AUTOREAD_BIT))
	{
		// Previous event time
		static uint32_t ticks = 0;

		if (HAL_GetTick() > (ticks + set_data.time_a * 100))
		{
			ticks = HAL_GetTick();

			if (_read_BV(reg, AUTOREAD_ACC_BIT))
			{
		    	printf("A: X=%d, Y=%d, Z=%d\n", measured_data.acc[0], measured_data.acc[1], measured_data.acc[2]);
			}

			if (_read_BV(reg, AUTOREAD_GYR_BIT))
			{
				printf("G: X=%d, Y=%d, Z=%d\n", measured_data.gyr[0], measured_data.gyr[1], measured_data.gyr[2]);
			}

			if (_read_BV(reg, AUTOREAD_MAG_BIT))
			{
				printf("M: X=%d, Y=%d, Z=%d\n", measured_data.mag[0], measured_data.mag[1], measured_data.mag[2]);
			}
		}
	}
}

/* Function for handling valves */
void valve_fcn(uint32_t * p_reg)
{
	static uint32_t ticks = 0;

	if (_read_BV(*p_reg, RUN_BIT))
	{
		if (_read_BV(*p_reg, PULSE_BIT) || _read_BV(*p_reg, TURN_BIT))
		{
			// Run is handled
			_clr_BV(*p_reg, RUN_BIT);

			// Duration of pulse
			ticks = HAL_GetTick() + _read_time(*p_reg);

			/* 1 - right direction, 0 - left direction */
			printf("%s valve was turned on\n", _read_BV(*p_reg, DIR_BIT) ? "Right" : "Left");
			// Open valve

			// printf("%lx\n",*p_reg);
		}
		else
		{
			/* Satellite is moving, waiting for required position */
			if (HAL_GetTick() > ticks)
			{
				// Create opposite pulse
				_tog_BV(*p_reg, DIR_BIT);
				_set_BV(*p_reg, PULSE_BIT);

				// printf("%lx\n",*p_reg);
			}
		}
	}
	else if ((HAL_GetTick() > ticks))
	{
		if (_read_BV(*p_reg, PULSE_BIT))
		{
			_clr_BV(*p_reg, PULSE_BIT);
			printf("Valves were closed\n");

			// printf("%lx\n",*p_reg);
		}
		else if (_read_BV(*p_reg, TURN_BIT))
		{
			_clr_BV(*p_reg, TURN_BIT);
			/* When run is set, and pulse and turn bit clear, satellite is moving. */
			/* Waiting for stopping pulse */
			_set_BV(*p_reg, RUN_BIT);

			// Duration of fly
			ticks = HAL_GetTick() + FLY_TIME;
			printf("Valves were closed\n");

			// printf("%lx\n",*p_reg); // CMD reg
		}
	}

	// // Testing time between valving controling
	//static uint32_t previous_tick = 0;

	//if (HAL_GetTick() > (previous_tick + 1000))
	//{
	//	printf("%ld\n", HAL_GetTick());
	//	previous_tick = HAL_GetTick();
	//}

}


