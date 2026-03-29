/*
 * loop_fcn.c
 *
 *  Created on: Mar 12, 2026
 *      Author: Antonin Putala
 */

/* Includes */
#include "loop_fcn.h"
#include "main.h"
#include "sensor_fcn.h"

#include <math.h>
#include <stdio.h>

/* Defines */
#define ANGLE_TOL           5   // Precision setting of position
#define STOP_CONST_LEFT     1   // For stop pulse calculation
#define STOP_CONST_RIGHT    1
#define GYR_TOL          1.0f   // Ignored motion

/* Function declaration*/
static void valve_close(void);

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
				printf("A: X=%d, Y=%d, Z=%d\n", _float2int(measured_data.acc[0]),
						_float2int(measured_data.acc[1]), _float2int(measured_data.acc[2]));
			}

			if (_read_BV(reg, AUTOREAD_GYR_BIT))
			{
				printf("G: X=%d, Y=%d, Z=%d\n", _float2int(measured_data.gyr[0]),
						_float2int(measured_data.gyr[1]), _float2int(measured_data.gyr[2]));
			}

			if (_read_BV(reg, AUTOREAD_POS_BIT))
			{
				printf("P: X=%ld, Y=%ld, Z=%ld\n", (int32_t)(measured_data.pos[0]),
						(int32_t)(measured_data.pos[1]), (int32_t)(measured_data.pos[2]));
			}

			/*
			if (_read_BV(reg, AUTOREAD_MAG_BIT))
			{
				printf("M: X=%d, Y=%d, Z=%d\n", _float2int(measured_data.mag[0]),
						_float2int(measured_data.mag[1]), _float2int(measured_data.mag[2]));
			}
			 */
		}
	}
}

/* Function for handling valves */
void valve_fcn(uint32_t * p_reg, float * p_set_pos)
{
	static uint32_t ticks = 0;
	static float stable_pos = 0; // Default measured_data.pos is 0 0 0
	static float required_pos = 0;

	if (_read_BV(*p_reg, RUN_BIT))
	{
		if (_read_BV(*p_reg, PULSE_BIT) || _read_BV(*p_reg, TURN_BIT) || _read_BV(*p_reg, HOME_BIT))
		{
			// Run is handled
			_clr_BV(*p_reg, RUN_BIT);

			// Duration of pulse
			ticks = HAL_GetTick();
			ticks += (_read_BV(*p_reg, DIR_BIT)) ? _read_time_r(*p_reg) : _read_time_l(*p_reg);

			/* 1 - right direction, 0 - left direction */
			HAL_GPIO_WritePin(VALVE_R_GPIO_Port, VALVE_R_Pin, _read_BV(*p_reg, DIR_BIT));
			HAL_GPIO_WritePin(VALVE_L_GPIO_Port, VALVE_L_Pin, !(_read_BV(*p_reg, DIR_BIT)));

			/* Update stable position */
			stable_pos = measured_data.pos[2];
			/* For regulator */
			*p_set_pos = stable_pos;

			printf("stable_pos: %d\n", (int16_t)(stable_pos));
		}
		else
		{
			/* Satellite is moving, waiting for required position */
			if ((fabsf(required_pos - measured_data.pos[2]) < ANGLE_TOL))
			{
				// Create opposite pulse
				_tog_BV(*p_reg, DIR_BIT);
				_set_BV(*p_reg, PULSE_BIT);

				// Redefine time according to angle speed
				_clr_time(*p_reg);
				uint8_t time[2];
				// Positive velocity => right rotation => left valve for stop
				time[0] = !(_read_BV(*p_reg, DIR_BIT)) ? (uint8_t)(STOP_CONST_LEFT  * fabsf(measured_data.gyr[2])) : set_data.time_l;
				time[1] = (_read_BV(*p_reg, DIR_BIT))  ? (uint8_t)(STOP_CONST_RIGHT * measured_data.gyr[2]) : set_data.time_r;
				_set_time(*p_reg, time[0], time[1]);

				printf("gyroscope: %d\n", _float2int(measured_data.gyr[2]));
				printf("break time: %d %d\n", time[0], time[1]);
			}
		}
	}
	else if ((HAL_GetTick() > ticks))
	{
		if (_read_BV(*p_reg, PULSE_BIT))
		{
			_clr_BV(*p_reg, PULSE_BIT);

			valve_close();
		}
		else if (_read_BV(*p_reg, TURN_BIT))
		{
			_clr_BV(*p_reg, TURN_BIT);
			/* When run is set, and pulse and turn bit clear, satellite is moving. */
			/* Waiting for stopping pulse */
			_set_BV(*p_reg, RUN_BIT);

			valve_close();

			// Duration of fly
			required_pos = stable_pos;
			required_pos += (_read_BV(*p_reg, DIR_BIT)) ? (-90) : 90;
			sensor_deg_limit(&required_pos);

			printf("required_pos: %d\n", (int16_t)(required_pos));

		}
		else if (_read_BV(*p_reg, HOME_BIT))
		{
			_clr_BV(*p_reg, HOME_BIT);
			_set_BV(*p_reg, RUN_BIT);

			valve_close();

			required_pos = (float)set_data.home;

			printf("required_pos: %d\n", (int16_t)(required_pos));
		}
	}
	/* For regulation, it helps transfer regulation to home setting */
	else if ((_read_BV(*p_reg, HOME_BIT)) && (_read_BV(*p_reg, REG_BIT)))
	{
		/* Now I am flying */
		_clr_BV(*p_reg, REG_BIT);
		_clr_BV(*p_reg, HOME_BIT);
		_set_BV(*p_reg, RUN_BIT);

		required_pos = *p_set_pos;

		printf("required_pos: %d\n", (int16_t)(required_pos));
	}
}

/* Regulation when cmd is not received */
///////// Add suspending of very small opening time
void regul_fcn(uint32_t * p_reg, float set_pos)
{
	static uint32_t ticks = 0;

	/* No motion is processed */
	if (!(_read_valve(*p_reg)))
	{
		/* Condition of running */
		if (!(_read_BV(*p_reg, CMD0_BIT)))
		{
			/* Create function */
			if (fabsf(measured_data.gyr[2]) > GYR_TOL)
			{
				_set_BV(*p_reg, REG_BIT);

				if (measured_data.gyr[2] > 0)
				{
					HAL_GPIO_WritePin(VALVE_R_GPIO_Port, VALVE_R_Pin, 1);
					ticks = HAL_GetTick() + (uint32_t)(STOP_CONST_RIGHT * measured_data.gyr[2]);
					printf("Right valve was open\n");
					printf("break time: %ld\n", (uint32_t)(STOP_CONST_RIGHT * measured_data.gyr[2]));
				}
				else
				{
					HAL_GPIO_WritePin(VALVE_L_GPIO_Port, VALVE_L_Pin, 1);
					ticks = HAL_GetTick() + (uint32_t)(-1 * STOP_CONST_LEFT * measured_data.gyr[2]); // gyr data are negative!!!
					printf("Left valve was open\n");
					printf("break time: %ld\n", (uint32_t)(-1 * STOP_CONST_LEFT * measured_data.gyr[2]));
				}
			}
			/* Cube is stable but position is different from required */
			/* Create function */
			else if (fabsf(measured_data.pos[2] - set_pos) > ANGLE_TOL)
			{
				/* Unique state */
				/* Helps to regulate required destination */
				_set_BV(*p_reg, REG_BIT);
				_set_BV(*p_reg, HOME_BIT);

				ticks = HAL_GetTick();

				/* Choose direction */
				if (((measured_data.pos[2] - set_pos) > 0) && ((measured_data.pos[2] - set_pos) < 180))
				{
					_set_BV(*p_reg, DIR_BIT);

					ticks += set_data.time_r;

					HAL_GPIO_WritePin(VALVE_R_GPIO_Port, VALVE_R_Pin, 1);
					printf("Right valve was open\n");
				}
				else
				{
					_clr_BV(*p_reg, DIR_BIT);

					ticks += set_data.time_l;

					HAL_GPIO_WritePin(VALVE_L_GPIO_Port, VALVE_L_Pin, 1);
					printf("Left valve was open\n");
				}
			}
		}
	}
	else if (_read_BV(*p_reg, REG_BIT))
	{
		/* Condition of stopping */
		if (HAL_GetTick() > ticks)
		{
			if (!(_read_BV(*p_reg, HOME_BIT)))
			{
				/* Only stop cube */
				_clr_BV(*p_reg, REG_BIT);
			}
			else
			{
				/* Continue as home cmd */
				printf("Continue as home\n");
			}

			valve_close();
			printf("Valve was closed\n");
		}
	}
}

/* Local function */
static void valve_close(void)
{
	// Close valves
	HAL_GPIO_WritePin(VALVE_R_GPIO_Port, VALVE_R_Pin, 0);
	HAL_GPIO_WritePin(VALVE_L_GPIO_Port, VALVE_L_Pin, 0);
}


