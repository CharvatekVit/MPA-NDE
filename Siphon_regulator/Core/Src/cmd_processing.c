/*
 * cmd_processing library
 * (c) Antonin Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/* Includes -----------------------------------------------*/
#include "cmd_processing.h"
#include "cmd_processing_def.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global functions ---------------------------------------*/
/*
 * Function: uart_byte_available
 * Purpose:  Recognizes commands in the received characters and
 * 	         sets the cmd register accordingly.
 * Input(s): c       - processed character
 *           p_reg   - command register, involves
 * 			           information about system state and
 * 			           switching time of valves
 * Returns:  none
 */
void uart_byte_available(uint8_t c, uint32_t * p_reg)
{
    static uint16_t cnt;
    static char data[CMD_BUFFER_LEN];

    if (cnt < CMD_BUFFER_LEN && c >= 32 && c <= 126)
    {
    	data[cnt++] = c;
    }

    /* Checking terminating characters */
    if ((c == '\n' || c == '\r') && cnt > 0)
    {
        data[cnt] = '\0';
        process_cmd(data, p_reg);
        cnt = 0;
    }
}

/* Static functions ---------------------------------------*/
/*
 * Function: process_cmd
 * Purpose:  Executes the corresponding commands are stores
 *           the result in the command register.
 * Input(s): cmd     - pointer to the string with processed command
 *           p_reg   - command register, involves
 * 			           information about system state and
 * 			           switching time of valves
 * Returns:  none
 */
static void process_cmd(char * cmd, uint32_t * p_reg)
{
	char *token;
	token = strtok(cmd, " ");

	/* Communication test */
	if (strcasecmp(token, "HELLO") == 0)
	{
	    printf("Communication OK\n");
	}
	/* Return default position */
	else if (strcasecmp(token, "HOME") == 0)
	{
		process_cmd_home(p_reg);
	}
	/* Read status register */
	else if (strcasecmp(token, "STATUS") == 0)
	{
		printf("reg:%08lx\n", *p_reg);
	}
	/* Reset status register */
	else if (strcasecmp(token, "RESET") == 0)
	{
		process_cmd_reset(p_reg);
	}
	/* Perform 1 step */
	else if (strcasecmp(token, "TURN") == 0)
	{
		process_cmd_turn(token, p_reg);
	}
	/* Perform 1 pulse */
	else if (strcasecmp(token, "PULSE") == 0)
	{
		process_cmd_pulse(token, p_reg);
	}
	/* Once read data from sensor */
	else if (strcasecmp(token, "READ") == 0)
	{
		process_cmd_read(token);
	}
	/* Periodic reading data from sensors */
	else if (strcasecmp(token, "AUTOREAD") == 0)
	{
		process_cmd_autoread(token, p_reg);
	}
	/* Enables configurate the cube */
	else if (strcasecmp(token, "SET") == 0)
	{
		process_cmd_set(token, p_reg);
	}
	/* Reserved */
	else if (strcasecmp(token, "CMD") == 0)
	{
		process_cmd_ncmd(token, p_reg);
	}
	else
	{
		printf("Invalid cmd\n");
	}
}

/*
* Function: process_cmd_turn
* Purpose:  Triggers a rotation by a defined angle.
* Input(s): cmd     - pointer to the string with processed command
*           p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_turn(char * token, uint32_t * p_reg)
{
	/* During performing another cmd is disable */
	if (_read_valve(*p_reg))
	{
		return;
	}

	token = strtok(NULL, " ");
    if (strcasecmp(token, "RIGHT") == 0)
    {
    	_set_BV(*p_reg, DIR_BIT);
    }
    else if (strcasecmp(token, "LEFT") == 0)
    {
    	_clr_BV(*p_reg, DIR_BIT);
    }
    else
    {
    	printf("Invalid dir\n");
    	return;
    }

    /* Update time setting */
    _clr_time(*p_reg);
    _set_time(*p_reg, set_data.time_l, set_data.time_r);

    /* Save turn request */
    _set_BV(*p_reg, RUN_BIT);
    _set_BV(*p_reg, TURN_BIT);
}

/*
* Function: process_cmd_read
* Purpose:  Sends sensors data based on the command.
* Input(s): cmd     - pointer to the string with processed command
*
* Returns:  none
*/
static void process_cmd_read(char * token)
{
    token = strtok(NULL, " ");

    if (strcasecmp(token, "GYR") == 0)
    {
    	printf("G: X=%d, Y=%d, Z=%d\n", _float2int(measured_data.gyr[0]),
    			_float2int(measured_data.gyr[1]), _float2int(measured_data.gyr[2]));
    }
    else if (strcasecmp(token, "ACC") == 0)
    {
    	printf("A: X=%d, Y=%d, Z=%d\n", _float2int(measured_data.acc[0]),
    			_float2int(measured_data.acc[1]), _float2int(measured_data.acc[2]));
    }
    else if (strcasecmp(token, "POS") == 0)
    {
    	printf("P: X=%ld, Y=%ld, Z=%ld\n", (int32_t)(measured_data.pos[0]),
    			(int32_t)(measured_data.pos[1]), (int32_t)(measured_data.pos[2]));
    }
    else if (strcasecmp(token, "ALL") == 0)
    {
    	printf("G: X=%d, Y=%d, Z=%d\n", _float2int(measured_data.gyr[0]),
    			_float2int(measured_data.gyr[1]), _float2int(measured_data.gyr[2]));
    	printf("A: X=%d, Y=%d, Z=%d\n", _float2int(measured_data.acc[0]),
    			_float2int(measured_data.acc[1]), _float2int(measured_data.acc[2]));
    	printf("P: X=%ld, Y=%ld, Z=%ld\n", (int32_t)(measured_data.pos[0]),
    			(int32_t)(measured_data.pos[1]), (int32_t)(measured_data.pos[2]));
    }
    else
    {
    	printf("Unknown\n");
    }
}

/*
* Function: process_cmd_pulse
* Purpose:  Triggers a pulse with a defined duration.
* Input(s): cmd     - pointer to the string with processed command
*           p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_pulse(char * token, uint32_t * p_reg)
{
	/* During performing another cmd is disable */
	if (_read_valve(*p_reg))
	{
		return;
	}

	/* Data which will be set to time register */
	uint8_t time_array[2] = {set_data.time_l, set_data.time_r};
	uint8_t dir_pointer;

	uint8_t pulse_len;
	token = strtok(NULL, " ");

	_clr_time(*p_reg);

	if (strcasecmp(token, "RIGHT") == 0)
	{
		_set_BV(*p_reg, DIR_BIT);
		dir_pointer = 1;
	}
	else if (strcasecmp(token, "LEFT") == 0)
	{
		_clr_BV(*p_reg, DIR_BIT);
		dir_pointer = 0;
	}
	else
	{
		printf("Invalid dir\n");
		return;
	}

	token = strtok(NULL, " ");

	pulse_len = atoi(token);

	if ((pulse_len >= PULSE_LEN_MIN) && (pulse_len <= PULSE_LEN_MAX))
	{
		/* Valid time */
		/* Arrange time setting only in selected direction */
		time_array[dir_pointer] = pulse_len;
	}
	/* Time opening valve was not specified*/
	else if (token == NULL)
	{
		/* No time setting*/
		/* Use default time settings */
	}
	else
	{
		printf("Invalid time\n");
	  	return;
	}

	/* Update time setting*/
	_set_time(*p_reg, time_array[0], time_array[1]);

	/* Save pulse request */
    _set_BV(*p_reg, RUN_BIT);
    _set_BV(*p_reg, PULSE_BIT);
}

/*
* Function: process_cmd_autoread
* Purpose:  Enables and disables autoread.
* Input(s): cmd     - pointer to the string with processed command
*           p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_autoread(char * token, uint32_t * p_reg)
{
	token = strtok(NULL, " ");

	if (strcasecmp(token, "ON") == 0)
	{
		/* By setting AUTOREAD_BIT, automatic reading will by started */
		_set_BV(*p_reg, AUTOREAD_BIT);
	}
	else if (strcasecmp(token, "OFF") == 0)
	{
		_clr_BV(*p_reg, AUTOREAD_BIT);
	}
	else
	{
	  	printf("Invalid cmd\n");
	  	return;
	}

}

/*
* Function: process_cmd_set
* Purpose:  Allows modifying the settings global variable and
*           changing the contents of the autoread report.
* Input(s): cmd     - pointer to the string with processed command
*           p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_set(char * token, uint32_t * p_reg)
{
	token = strtok(NULL, " ");

	if (strcasecmp(token, "READ") == 0)
	{
		process_cmd_set_read(token, p_reg);
	}
	else if (strcasecmp(token, "AUTO") == 0)
	{
		process_cmd_set_auto(token);
	}
	else if (strcasecmp(token, "TIME") == 0)
	{
		process_cmd_set_time(token);
	}
	else if (strcasecmp(token, "ANGLE") == 0)
	{
		process_cmd_set_angle(token);
	}
	else if (strcasecmp(token, "HOME") == 0)
	{
		process_cmd_set_home(token);
	}
	else
	{
		printf("Invalid cmd\n");
		return;
	}
}

/*
* Function: process_cmd_reset
* Purpose:  Clears command register. Closes valves.
* Input(s): p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_reset(uint32_t * p_reg)
{
	/* Clear cmd register */
	*p_reg = 0;

	/* Close all valves */
	HAL_GPIO_WritePin(VALVE_R_GPIO_Port, VALVE_R_Pin, 0);
	HAL_GPIO_WritePin(VALVE_L_GPIO_Port, VALVE_L_Pin, 0);
}

/*
* Function: process_cmd_set_read
* Purpose:  Enables turn on/off autoreport from these sensors.
* Input(s): cmd     - pointer to the string with processed command
*           p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_set_read(char * token, uint32_t * p_reg)
{
	token = strtok(NULL, " ");
	uint8_t bit;

	/* Select which sensor */
	if (strcasecmp(token, "ACC") == 0)
	{
		bit = AUTOREAD_ACC_BIT;
	}
	else if (strcasecmp(token, "GYR") == 0)
	{
		bit = AUTOREAD_GYR_BIT;
	}
	else if (strcasecmp(token, "POS") == 0)
	{
		bit = AUTOREAD_POS_BIT;
	}
	else
	{
		printf("Invalid cmd\n");
		return;
	}

	token = strtok(NULL, " ");

	/* Turn on or turn off report */
	if (strcasecmp(token, "ON") == 0)
	{
		_set_BV(*p_reg, bit);
	}
	else if (strcasecmp(token, "OFF") == 0)
	{
		_clr_BV(*p_reg, bit);
	}
	else
	{
	  	printf("Invalid cmd\n");
	  	return;
	}
}

/*
* Function: process_cmd_set_auto
* Purpose:  Enables change the period of autoreading.
* Input(s): cmd     - pointer to the string with processed command
*
* Returns:  none
*/
static void process_cmd_set_auto(char * token)
{
	token = strtok(NULL, " ");
	uint8_t time = atoi(token);

	if ((time != 0) && (time <= MAX_TIME))
	{
		set_data.time_a = time;
	}
}

/*
* Function: process_cmd_set_time
* Purpose:  Allows adjusting the valve-opening time settings.
* Input(s): cmd     - pointer to the string with processed command
*
* Returns:  none
*/
static void process_cmd_set_time(char * token)
{
	token = strtok(NULL, " ");

	char * dir = token;

	token = strtok(NULL, " ");
	uint8_t time = atoi(token);

	if ((time >= PULSE_LEN_MIN) && (time <= PULSE_LEN_MAX))
	{
		if (strcasecmp(dir, "RIGHT") == 0)
		{
			set_data.time_r = time;
		}
		else if (strcasecmp(dir, "LEFT") == 0)
		{
			set_data.time_l = time;
		}
		else if (strcasecmp(dir, "ALL") == 0)
		{
			set_data.time_r = time;
			set_data.time_l = time;
		}
		else
		{
			printf("Invalid dir\n");
			return;
		}
	}
}

/*
* Function: process_cmd_set_angle
* Purpose:  Allows setting the rotation angle for the TURN command.
* Input(s): cmd     - pointer to the string with processed command
*
* Returns:  none
*/
static void process_cmd_set_angle(char * token)
{
	token = strtok(NULL, " ");
	uint8_t angle = atoi(token);

	if ((angle > TURN_ANGLE_MIN) && (angle <= TURN_ANGLE_MAX))
	{
		set_data.angle = angle;
	}
}

/*
* Function: process_cmd_set_home
* Purpose:  Allows setting the default orientation for the HOME command.
* Input(s): cmd     - pointer to the string with processed command
*
* Returns:  none
*/
static void process_cmd_set_home(char * token)
{
	token = strtok(NULL, " ");
	int16_t home = atoi(token);

	if (home >= HOME_ANGLE_MIN && home <= HOME_ANGLE_MAX)
	{
		set_data.home = home;
	}
	else
	{
		printf("Invalid position\n");
		return;
	}
}

/*
* Function: process_cmd_home
* Purpose:  Triggers a home command.
* Input(s): p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_home(uint32_t * p_reg)
{
	/* During performing another cmd is disable */
	if (_read_valve(*p_reg))
	{
		return;
	}

    /* Preper time setting */
    _clr_time(*p_reg);
    _set_time(*p_reg, set_data.time_l, set_data.time_r);

    /* Save home request */
    _set_BV(*p_reg, RUN_BIT);
    _set_BV(*p_reg, HOME_BIT);
}

/*
* Function: process_cmd_ncomd
* Purpose:  Reserved.
* Input(s): cmd     - pointer to the string with processed command
*           p_reg   - command register, involves
* 			          information about system state and
* 			          switching time of valves
* Returns:  none
*/
static void process_cmd_ncmd(char * token, uint32_t * p_reg)
{
	uint8_t ncmd;

	token = strtok(NULL, " ");
	ncmd = atoi(token);

	switch (ncmd)
	{
		case 0:
			_tog_BV(*p_reg, CMD0_BIT);
			break;
		case 1:
			_tog_BV(*p_reg, CMD1_BIT);
			break;
		case 2:
			_tog_BV(*p_reg, CMD2_BIT);
			break;
		case 3:
			_tog_BV(*p_reg, CMD3_BIT);
			break;
		default:
			printf("Invalid cmd\n");
			break;
	}
}


