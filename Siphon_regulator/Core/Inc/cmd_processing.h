/*
 * Command processing library
 * (c) Antonin_Putala 2026
 *
 * Developed using STM32CubeIDE
 * Tested on BluePill board and STM32F103C8T6, 32 MHz.
 */

/**
 * @file      Command processing library <cmd_processing.h>
 * @defgroup  Commands
 * @code #include <cmd_processing.h> @endcode
 *
 * @brief Process commands send to MCU through serial port.
 *
 * The library contains functions that enables the recognition
 * of the commands HELLO, STATUS, RESET, TURN, PULSE, HOME, SET,
 * READ, AUTOREAD and CMD. Its contents are adapted to the
 * Siphon Regulator project.
 *
 * @{
 */

#ifndef __CMD_PROCESSING_H
#define __CMD_PROCESSING_H

/* Includes -----------------------------------------------*/
#include "main.h"

/* Defines ------------------------------------------------*/
//#define CMD_BUFFER_LEN 256

/* Extern global variables --------------------------------*/
extern sensor_data_t measured_data;
extern settings_t    set_data;

/* Function prototypes ------------------------------------*/
/**
 * @brief  Recognizes commands in the received characters and
 * 	       sets the cmd register accordingly.
 * @param  c         processed character
 * @param  cmd_reg   command register, involves
 * 			         information about system state and
 * 			         switching time of valves
 * The processed character is written into the buffer step by step.
 * The terminating characters '\\n' and '\r' are checked. After that,
 * the function that processes the received command is called.
 *
 * Available commands:\n
 * | Command     | Parameters         	   | Description 					  		 |
 * | :---        | :---               	   | :---        					  		 |
 * |**HELLO**    |                     	   | test communication 			 		 |
 * |**STATUS**   | 	    	           	   | read command register	   		  		 |
 * |**RESET**    |                   	   | clear command register 		 		 |
 * |**HOME**     |                   	   | return to home position 		  		 |
 * |**TURN**     | (RIGHT/LEFT)            | rotates by a defined angle 	  		 |
 * |**PULSE**    | (RIGHT/LEFT)     <time> | generates a pulse of a defined duration |
 * |**READ**     | (ACC/GYR/POS/ALL)       | read sensor data 			  	  		 |
 * |**AUTOREAD** |      (ON/OFF)           | repeated reading from the sensors 		 |
 * |**CMD**      |       (0/1/2/3)	       | reserved 					       		 |
 * |**SET ANGLE**|                 <angle> | set angle of TURN command 		  		 |
 * |**SET TIME** | (RIGHT/LEFT)     <time> | set valve switching time 		  		 |
 * |**SET AUTO** |                  <time> | set period of autoreading 		  		 |
 * |**SET READ** | (ACC/GYR/POS)  (ON/OFF) | set content of autoreading 	   		 |
 * |**SET HOME** |                 <angle> | set default orientation 		  		 |
 */
void uart_byte_available(uint8_t c, uint32_t * p_reg);

/** @} */
#endif /* __CMD_PROCCESING_H */
