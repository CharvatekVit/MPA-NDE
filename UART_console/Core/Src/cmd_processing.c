
/* Includes */
#include "cmd_processing.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines */
#define PULSE_LEN_MIN 10
#define PULSE_LEN_MAX 100

/* Static functions declaration */
static void process_cmd(char *, uint32_t *);
static void process_cmd_read(char *);
static void process_cmd_turn(char *, uint32_t *);
static void process_cmd_home(uint32_t *);
static void process_cmd_pulse(char *, uint32_t *);
static void process_cmd_set(char *, uint32_t *);
static void process_cmd_ncmd(char *, uint32_t *);
static void process_cmd_autoread(char *, uint32_t *);
static void process_cmd_set_read(char *, uint32_t *);


/* Global functions */
void uart_byte_available(uint8_t c, uint32_t * p_reg)
{
    static uint16_t cnt;
    static char data[CMD_BUFFER_LEN];

    if (cnt < CMD_BUFFER_LEN && c >= 32 && c <= 126)
    {
    	data[cnt++] = c;
    }

    if ((c == '\n' || c == '\r') && cnt > 0)
    {
        data[cnt] = '\0';
        process_cmd(data, p_reg);
        cnt = 0;
    }
}

/* Static functions */
static void process_cmd(char * cmd, uint32_t * p_reg)
{
	char *token;
	token = strtok(cmd, " ");

	/* Communication test */
	if (strcasecmp(token, "HELLO") == 0)
	{
	    printf("Communication OK\n");
	}
	/* Set default position */
	else if (strcasecmp(token, "HOME") == 0)
	{
		process_cmd_home(p_reg);
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
	/* Enables configurate cube */
	else if (strcasecmp(token, "SET") == 0)
	{
		process_cmd_set(token, p_reg);
	}
	/* Once read data from sensor */
	else if (strcasecmp(token, "CMD") == 0)
	{
		process_cmd_ncmd(token, p_reg);
	}
	else
	{
		printf("Invalid cmd\n");
	}
}

/* TURN cmd callback */
static void process_cmd_turn(char * token, uint32_t * p_reg)
{
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

    _set_BV(*p_reg, RUN_BIT);
    _set_BV(*p_reg, TURN_BIT);
}

/* READ cmd callback */
static void process_cmd_read(char * token)
{
    token = strtok(NULL, " ");

    if (strcasecmp(token, "MAG") == 0)
    {
    	printf("M: X=%d, Y=%d, Z=%d\n", measured_data.mag[0], measured_data.mag[1], measured_data.mag[2]);
    }
    else if (strcasecmp(token, "GYR") == 0)
    {
    	printf("G: X=%d, Y=%d, Z=%d\n", measured_data.gyr[0], measured_data.gyr[1], measured_data.gyr[2]);
    }
    else if (strcasecmp(token, "ACC") == 0)
    {
    	printf("A: X=%d, Y=%d, Z=%d\n", measured_data.acc[0], measured_data.acc[1], measured_data.acc[2]);
    }
    else if (strcasecmp(token, "ALL") == 0)
    {
    	printf("M: X=%d, Y=%d, Z=%d\n", measured_data.mag[0], measured_data.mag[1], measured_data.mag[2]);
    	printf("G: X=%d, Y=%d, Z=%d\n", measured_data.gyr[0], measured_data.gyr[1], measured_data.gyr[2]);
    	printf("A: X=%d, Y=%d, Z=%d\n", measured_data.acc[0], measured_data.acc[1], measured_data.acc[2]);
    }
    else
    {
    	printf("Unknown\n");
    }
}

/* PULSE cmd callback */
static void process_cmd_pulse(char * token, uint32_t * p_reg)
{
	uint8_t pulse_len;
	token = strtok(NULL, " ");

	_clr_time(*p_reg);

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

	token = strtok(NULL, " ");

	pulse_len = atoi(token);

	if ((pulse_len >= PULSE_LEN_MIN) && (pulse_len <= PULSE_LEN_MAX))
	{
		_set_time(*p_reg, pulse_len);
	}
	else
	{
		printf("Invalid time\n");
	  	return;
	}

    _set_BV(*p_reg, RUN_BIT);
    _set_BV(*p_reg, PULSE_BIT);
}

/* Autoreading settings */
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

static void process_cmd_set(char * token, uint32_t * p_reg)
{
	token = strtok(NULL, " ");

	if (strcasecmp(token, "READ") == 0)
	{
		process_cmd_set_read(token, p_reg);
	}
	else
	{
		printf("Invalid cmd\n");
		return;
	}
}

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
	else if (strcasecmp(token, "MAG") == 0)
	{
		bit = AUTOREAD_MAG_BIT;
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

/* Go to home position */
static void process_cmd_home(uint32_t * p_reg)
{
    _set_BV(*p_reg, RUN_BIT);
    _set_BV(*p_reg, HOME_BIT);
}

/* Reserve commands */
static void process_cmd_ncmd(char * token, uint32_t * p_reg)
{
	uint8_t ncmd;

	token = strtok(NULL, " ");
	ncmd = atoi(token);

	switch (ncmd)
	{
		case 0:
			_set_BV(*p_reg, CMD0_BIT);
			break;
		case 1:
			_set_BV(*p_reg, CMD1_BIT);
			break;
		case 2:
			_set_BV(*p_reg, CMD2_BIT);
			break;
		case 3:
			_set_BV(*p_reg, CMD3_BIT);
			break;
		default:
			printf("Invalid cmd\n");
			break;
	}
}


