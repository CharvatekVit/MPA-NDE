/*
 * loop_fcn.h
 *
 *  Created on: Mar 12, 2026
 *      Author: Antonin Putala
 */

#ifndef INC_LOOP_FCN_H_
#define INC_LOOP_FCN_H_

/* Includes */
#include "main.h"

/* Extern global variables */
extern sensor_data_t measured_data;
extern settings_t    set_data;

/* Function prototypes */
void autoread_fcn(uint32_t);

#endif /* INC_LOOP_FCN_H_ */
