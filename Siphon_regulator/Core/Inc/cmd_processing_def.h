/*
 * cmd_processing library
 * (c) Antonin_Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/**
 * @file      Command processing library definition <cmd_processing_def.h>
 * @defgroup  Commands
 * @code #include <cmd_processing_def.h> @endcode
 *
 * Necessary definition and function declaration.
 *
 * @copyright (c) Antonin Putala 2026
 * @{
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
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * It processes the first word of the command. It handles the HELLO
 * and STATUS commands directly. For the remaining commands, it calls
 * a dedicated function.
 */
static void process_cmd(char * cmd, uint32_t * p_reg);

/**
 * @brief  Sends sensors data based on the command.
 * @param  cmd       pointer to the string with processed command
 *
 * Available arguments:\n
 * | Argument    | Description 					  		 |
 * | :---        |:---        					  		 |
 * |**ACC**      | accelerometer			 		     |
 * |**GYR**      | gyroscope - angular velocity 	     |
 * |**POS**      | gyroscope - orientation 		  		 |
 * |**ALL**      | all available data    		  		 |
 */
static void process_cmd_read(char * cmd);

/**
 * @brief  Triggers a rotation by a defined angle.
 * @param  cmd       pointer to the string with processed command
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * It forwards the rotation request by writing it into the command
 * register. It sets the RUN_BIT and TURN_BIT flag. Depending
 * on the direction, it sets the DIR_BIT flag. It also updates
 * the timing configuration in the command register, using
 * the time value from the global variable settings.
 *
 * Available arguments:\n
 * | Argument    | Description 				     |   DIR_BIT |
 * | :---        |:---        				     | :---      |
 * |**LEFT**     | rotation to the left       	 | 0         |
 * |**RIGHT**    | rotation to the right   		 | 1         |
 */
static void process_cmd_turn(char * cmd, uint32_t * p_reg);

/**
 * @brief  Triggers a home command.
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * It forwards the home request by writing it into the command
 * register. It sets the RUN_BIT and HOME_BIT flag. It also updates
 * the timing configuration in the command register, using
 * the time value from the global variable settings.
 */
static void process_cmd_home(uint32_t * p_reg);

/**
 * @brief  Triggers a pulse with a defined duration.
 * @param  cmd       pointer to the string with processed command
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 * @warning It does not work together with the regulation system.
 * 		    Intended for testing only. The regulation must be disabled
 * 		    (CMD 0).
 *
 * It forwards the pulse request by writing it into the command
 * register. It sets the RUN_BIT and PULSE_BIT flag. Depending
 * on the direction, it sets the DIR_BIT flag. It also updates
 * the timing configuration in the command register.
 *
 * The pulse duration can be specified by the user using the <time>
 * argument. The time is given in milliseconds. If it is not specified,
 * the value from the global variable *settings* is used.
 *
 * Available arguments:\n
 * | Argument    | Description 				     |   DIR_BIT |
 * | :---        |:---        				     | :---      |
 * |**LEFT**     | rotation to the left       	 | 0         |
 * |**RIGHT**    | rotation to the right   		 | 1         |
 */
static void process_cmd_pulse(char * cmd, uint32_t * p_reg);

/**
 * @brief  Allows modifying the settings global variable and
 *         changing the contents of the autoread report.
 * @param  cmd       pointer to the string with processed command
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * Processes the second arguments of the command. Calls the
 * corresponding function.
 *
 * * Available arguments:\n
 * | Argument    | Description 				     |  Function              |
 * | :---        |:---        				     | :---                   |
 * |**ANGLE**    | set angle of TURN command   	 | process_cmd_set_angle  |
 * |**TIME**     | set valve switching time 	 | process_cmd_set_time   |
 * |**AUTO**     | set period of autoreading 	 | process_cmd_set_auto   |
 * |**HOME**     | set default orientation   	 | process_cmd_set_home   |
 * |**READ**     | set content of autoreading    | process_cmd_set_read   |
 */
static void process_cmd_set(char * cmd, uint32_t * p_reg);

/**
 * @brief  Reserved.
 * @param  cmd       pointer to the string with processed command
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 * @note   Setting CMD0_BIT disables regulation.
 *
 * Allow arguments are 0 to 3. Based on the argument, it
 * toggles the bits CMD0_BIT to CMD3_BIT.
 *
 */
static void process_cmd_ncmd(char * cmd, uint32_t * p_reg);

/**
 * @brief  Enables and disables autoread.
 * @param  cmd       pointer to the string with processed command
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * Allows setting and clearing the AUTOREAD_BIT flag.
 *
 *  * Available arguments:\n
 * | Argument    | Description 				     | AUTOREAD_BIT |
 * | :---        |:---        				     | :---         |
 * |**OFF**      | disable autoread           	 | 0            |
 * |**ON**       | enable autoread   	     	 | 1            |
 */
static void process_cmd_autoread(char * cmd, uint32_t * p_reg);

/**
 * @brief  Clears command register. Closes valves.
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * Emergency command. Reset to defaut settings.
 * Interrupts any running commands. Stop the valves.
 */
static void process_cmd_reset(uint32_t * p_reg);

/**
 * @brief  Enables turn on/off autoreport from these sensors.
 * @param  cmd       pointer to the string with processed command
 * @param  p_reg     command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 *
 * Sets (ON) or clear (OFF) the corresponding flag in the command register.
 *
 * Available arguments:\n
 * | Argument    | Description 			     		 | Affected bit     |
 * | :---        |:---        					 	 | :---             |
 * |**ACC**      | accelerometer			 	     | AUTOREAD_ACC_BIT |
 * |**GYR**      | gyroscope - angular velocity      | AUTOREAD_GYR_BIT |
 * |**POS**      | gyroscope - orientation   		 | AUTOREAD_POS_BIT |
 */
static void process_cmd_set_read(char * cmd, uint32_t * p_reg);

/**
 * @brief  Enables change the period of autoreading.
 * @param  cmd       pointer to the string with processed command
 *
 * The autoread period is stored in the global variable settings Its value
 * is determined by the <time> argument. It is an integer within the specified
 * limits and is interpreted as tenths of a second.
 */
static void process_cmd_set_auto(char * cmd);

/**
 * @brief  Allows adjusting the valve-opening time settings.
 * @param  cmd       pointer to the string with processed command
 *
 * Stores the specified time value <time> into the global variable
 * settings. The time value must be an unsinged integer within
 * the defined range and is interpreted as a duration
 * in milliseconds.
 *
 * Available arguments:\n
 * | Argument    |  Affected variables  		       |
 * | :---        |:---        					 	   |
 * |**RIGHT**    | set_data.time_r			 	       |
 * |**LEFT**     | set_data.time_l          	       |
 * |**ALL**      | set_data.time_r, set_data.time_l    |
 */
static void process_cmd_set_time(char * cmd);

/**
 * @brief  Allows setting the rotation angle for the TURN command.
 * @param  cmd       pointer to the string with processed command
 *
 * The <angle> argument is stored in the global variable settings.
 * It must be an unsigned integer value within the defined limits
 * and is interpreted as degrees.
 *
 */
static void process_cmd_set_angle(char * cmd);

/**
 * @brief  Allows setting the default orientation for the HOME command.
 * @param  cmd       pointer to the string with processed command
 *
 * The <angle> argument is stored in the global variable settings.
 * It must be an signed integer value within the defined limits
 * and is interpreted as degrees.
 *
 */
static void process_cmd_set_home(char * cmd);

/** @} */
#endif /* INC_CMD_PROCESSING_DEF_H_ */
