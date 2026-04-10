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
/** Maximum length of a received command is 256 characters. */
#define CMD_BUFFER_LEN    256

/** Minimum open valve time is 10 ms. */
#define PULSE_LEN_MIN      10

/** Maximum  open valve time is 100 ms. */
#define PULSE_LEN_MAX     100

/** Maximum period of autoreading is 25.5 s. */
#define MAX_TIME          255

/** Minimum angle of TURN command is 10°. */
#define TURN_ANGLE_MIN     10

/** Maximum angle of TURN command is 90°. */
#define TURN_ANGLE_MAX     90

/** Minimum angle of a default orietation is -180°. */
#define HOME_ANGLE_MIN   -180

/** Maximum angle of a default orietation is 180°. */
#define HOME_ANGLE_MAX    180


/* Static functions declaration ---------------------------*/
/* Function prototypes ------------------------------------*/
/**
 * @brief  Executes the corresponding commands are stores the result
 *         in the command register.
 * @param  cmd       pointer to the string with processed command
 * @param  cmd_reg   32 bits - command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * It processes the first word of the command. It handles the HELLO
 * and STATUS commands directly. For the remaining commands, it calls
 * a dedicated function.
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
