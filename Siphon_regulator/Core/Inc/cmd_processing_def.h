/*
 * cmd_processing library
 * (c) Antonin_Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

#ifndef INC_CMD_PROCESSING_DEF_H_
#define INC_CMD_PROCESSING_DEF_H_

/* Defines ------------------------------------------------*/
#define PULSE_LEN_MIN      10
#define PULSE_LEN_MAX     100
#define MAX_TIME          255    // max period of sending autoreport 25,5 s
#define TURN_ANGLE_MIN     10
#define TURN_ANGLE_MAX     90
#define HOME_ANGLE_MAX    180
#define HOME_ANGLE_MIN   -180

/* Static functions declaration ---------------------------*/
/* Function prototypes ------------------------------------*/
/**
 * @brief  Rounded number with decimal and integer part.
 * @param  c         8 bits  - processed character
 * @param  cmd_reg   32 bits - command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 */
static void process_cmd(char *, uint32_t *);

static void process_cmd_read(char *);

static void process_cmd_turn(char *, uint32_t *);

static void process_cmd_home(uint32_t *);

static void process_cmd_pulse(char *, uint32_t *);

static void process_cmd_set(char *, uint32_t *);

static void process_cmd_ncmd(char *, uint32_t *);

static void process_cmd_autoread(char *, uint32_t *);

static void process_cmd_reset(uint32_t *);

static void process_cmd_set_read(char *, uint32_t *);

static void process_cmd_set_auto(char *);

static void process_cmd_set_time(char *);

static void process_cmd_set_angle(char *);

static void process_cmd_set_home(char *);

#endif /* INC_CMD_PROCESSING_DEF_H_ */
