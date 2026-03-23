/*
 * cmd_processing.h
 *
 *  Created on: Mar 9, 2026
 *      Author: Antonin Putala
 */

#ifndef __CMD_PROCESSING_H
#define __CMD_PROCESSING_H

/* Includes */
#include "main.h"

/* Defines */
#define CMD_BUFFER_LEN 256

/* Extern global variables */
extern sensor_data_t measured_data;
extern settings_t    set_data;

/* Function prototypes */
void uart_byte_available(uint8_t, uint32_t *);

#endif /* __CMD_PROCCESING_H */
