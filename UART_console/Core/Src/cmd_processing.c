
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
static uint32_t process_cmd(char *, uint32_t);
static void process_cmd_read(char *);
static uint32_t process_cmd_turn(char *, uint32_t);
static uint32_t process_cmd_pulse(char *, uint32_t);
static uint32_t process_cmd_ncmd(char *, uint32_t);


/* Global functions */
uint32_t uart_byte_available(uint8_t c, uint32_t cmd_reg)
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
        cmd_reg = process_cmd(data, cmd_reg);
        cnt = 0;
    }

    return cmd_reg;
}

/* Static functions */
static uint32_t process_cmd(char * cmd, uint32_t cmd_reg)
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
		printf("Home\n");
	}
	/* Perform 1 step */
	else if (strcasecmp(token, "TURN") == 0)
	{
		cmd_reg = process_cmd_turn(token, cmd_reg);
	}
	/* Perform 1 pulse */
	else if (strcasecmp(token, "PULSE") == 0)
	{
		cmd_reg = process_cmd_pulse(token, cmd_reg);
	}
	/* Once read data from sensor */
	else if (strcasecmp(token, "READ") == 0)
	{
		process_cmd_read(token);
	}
	/* Once read data from sensor */
	else if (strcasecmp(token, "CMD") == 0)
	{
		cmd_reg = process_cmd_ncmd(token, cmd_reg);
	}

	return cmd_reg;
}

/* TURN cmd callback */
static uint32_t process_cmd_turn(char * token, uint32_t cmd_reg)
{
	token = strtok(NULL, " ");
    if (strcasecmp(token, "RIGHT") == 0)
    {
    	_set_BV(cmd_reg, DIR_BIT);
    }
    else if (strcasecmp(token, "LEFT") == 0)
    {
    	_clr_BV(cmd_reg, DIR_BIT);
    }
    else
    {
    	return cmd_reg;
    }

    _set_BV(cmd_reg, RUN_BIT);
    _set_BV(cmd_reg, TURN_BIT);

    return cmd_reg;
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
static uint32_t process_cmd_pulse(char * token, uint32_t cmd_reg)
{
	uint8_t pulse_len;
	token = strtok(NULL, " ");

	if (strcasecmp(token, "RIGHT") == 0)
	{
		_set_BV(cmd_reg, DIR_BIT);
	}
	else if (strcasecmp(token, "LEFT") == 0)
	{
		_clr_BV(cmd_reg, DIR_BIT);
	}
	else
	{
		return cmd_reg;
	}

	token = strtok(NULL, " ");

	pulse_len = atoi(token);

	if ((pulse_len >= PULSE_LEN_MIN) && (pulse_len <= PULSE_LEN_MAX))
	{
	   	printf("%s ms\n", token);
	}
	else
	{
	  	printf("Invalid time\n");
	}

    _set_BV(cmd_reg, RUN_BIT);
    _set_BV(cmd_reg, PULSE_BIT);

    return cmd_reg;
}

/* Reserve commands */
static uint32_t process_cmd_ncmd(char * token, uint32_t cmd_reg)
{
	uint8_t ncmd;

	token = strtok(NULL, " ");
	ncmd = atoi(token);

	switch (ncmd)
	{
		case 0:
			_set_BV(cmd_reg, CMD0_BIT);
			break;
		case 1:
			_set_BV(cmd_reg, CMD1_BIT);
			break;
		case 2:
			_set_BV(cmd_reg, CMD2_BIT);
			break;
		case 3:
			_set_BV(cmd_reg, CMD3_BIT);
			break;
		default:
			printf("Invalid command\n");
			break;
	}

	return cmd_reg;
}


